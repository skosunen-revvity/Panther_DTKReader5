// --------------------------------------------------------------------------
// DSCamera.h
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera driver definition. This is managed C++ class
// that will houde the actual image renderer manager m_pRenderManager. This
// layer was used because the current implementation of the image renderer is
// using Microsft DirectShow library classes that are written in unmanaged C++
// and COM, and it was too difficult to get them working in managed C++.
//
// All data to actual renderer is passed via the global data storage (g_pPunchPosManager)
// which is created when this class is created.
//
// TODO: - Fix so that when settings are set/get, we dont need to place them into
//         different STRUCTs, we could use only 1 STRUCT/Class
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 27.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once

#using <system.dll>
using namespace System;

#using <system.drawing.dll>
using namespace System::Drawing;

// Interface definition
#using "..\\..\\..\\..\\Common\\Bin\\OnSpotLibrary.dll"
using namespace PKI::Puncher::Library;

#include "DSRenderManager.H"
#include "ExceptionInfo.h"

namespace PKI {

  namespace Puncher {
    namespace DSCameraDriver {

      public ref class DSCamera : public IPuncherDSCameraDriver {

        public:
          DSCamera();
          virtual ~DSCamera();

          // Initialize camera driver. Settings are validated, image renderer manager is initialized and 
          // global data storage (g_pPunchPosManager) is created. This storage is used to pass/get information 
          // to actual image renderer
          virtual void Initialize(IPuncherCameraSettings^ camSettings);

          // Reelase camera driver. Releases image renderer manager and the global data storage.
          virtual void Release();

          // Start image processing/"playback"
          virtual void Start();

          // Stop image processing
          virtual void Stop();

          // Parform camera calibration, using next image, or, if specified, using the specified image.
          // Camera calibration means that so called hotspots are calculated from the next image. That
          // data is used later to detect whether receaved image is changed (= contains blood card) or
          // not.
          virtual void Calibrate(Bitmap^ calibrationImage);

          // Returns grabbed image, or if one does not exist, captures next image and returns it
          virtual Bitmap^ TakePicture();

          // Grabs next image to be returned later by TakePicture. Returns true, if grabbing was succesful.
          virtual bool GrabPicture();

          // Get/set punch positions. When pattern is used, this will return those positions
          virtual property IPunchPositions^ PunchPositions {
            IPunchPositions^  get();
            void              set(IPunchPositions^ value);
          }

          // Get/set settings.
          virtual property IPuncherCameraSettings^ Settings {
            IPuncherCameraSettings^ get();
            void                    set(IPuncherCameraSettings^ value);
          }

          // Get the native windows handle (Wind32 HEVENT) for the puch count changed event.
          // This can be used to detect when the number of found punch positions have changed
          virtual property IntPtr PunchPositionCountChangedEventHandle {
            IntPtr get();
          }

          // Get native event handle (Wind32 HEVENT) for blood card detected event. This can be
          // used to detect when the blood card is detected in the image
          virtual property IntPtr BloodCardDetectedEventHandle {
            IntPtr get();
          }

          // Get native event handle (Wind32 HEVENT) for blood card removed event. This can be
          // used to detect when the blood card is removed
          virtual property IntPtr BloodCardRemovedEventHandle {
            IntPtr get();
          }

          // Get native event handle (Wind32 HEVENT) for barcodes updated event. This can be
          // used to detect when barcode info has been updated. When blood card is removed, all
          // barcodes will be removed but this event isn´t raised.
          virtual property IntPtr BarcodesUpdatedEventHandle {
            IntPtr get();
          }

          // Get native event handle (Wind32 HEVENT) for camera exception occured event. This can be
          // used to detect when exception has occured in camera driver. The first exception, that
          // signaled this event, can be get using the CameraDriverException-property. When retreiving
          // it, the exception is cleared.
          virtual property IntPtr CameraDriverExceptionEventHandle {
            IntPtr get();
          }

          // Get the native event handle for frames skipped notification event. This event is raised
          // when too much time have elapsed without camera driver processing the frames. Camera driver
          // can´t process frames if it can´t gain acces to shared data. The interval is defined in
          // seetings, FramesSkippedNotificationLimit-property
          virtual property IntPtr FramesSkippedNotificationEventHandle {
            IntPtr get();
          }

          // Gets the most recently read barcode. The barcode is returned as two element string array, first element being the
          // actual contents of the code, and the second one the detected code type.
          virtual property array<String^>^ Barcode {
            array<String^>^ get();
          }


          // Get exception that has occured in camera driver. This will also remove the exception
          virtual property PuncherCameraException^ CameraDriverException {
            PuncherCameraException^ get();
          }

          // Get/Set barcode reading status. When true, barcodes are read from the image
          virtual property bool ReadBarcodes {
            bool get();
            void set(bool value);
          }

          // Whether the blood detection is on or off. 
          virtual property bool BloodDetectionOn {
            bool get();
            void set(bool value);
          }

          // Returns true when card has been detected
          virtual property bool CardDetected {
            bool get();
          }

          // Force camera to display specified image. In order to switch back to live image,
          // this method must be called with null argument.
          virtual void DisplayImage(Bitmap^ imageToShow);


          /// Update camera drawing surface. Driver must be initialized in order to update the drawing 
          /// surface. Drawing surface must be a native window handle (Win32 HWND)
          virtual void SetDrawingSurface(IntPtr newDrawingSurface);

          // Punch Border, the area outside the actual punch area, that is reserved for punch position inaccuracies, the
          // border area needs to be in blood too (assuming that the punch area needs to be in blood), and no other punches
          // may be even partially positioned on this border area.
          virtual property BYTE PunchBorder {
            BYTE get();
            void set(BYTE value);
          }
          
          // Blood Border, the area in the outskirts of detected blood, that is reserved for punchhead-to-camera position inaccuracies.
          virtual property BYTE BloodBorder {
            BYTE get();
            void set(BYTE value);
          }

          
          // Filtered measured camera frame interval since instrument startup, expressed in milliseconds.
          virtual property unsigned int FilteredFrameInterval {
                unsigned int get();
          }
          
          // Maximum detected camera frame interval since instrument startup (or last call to ResetMaxFramInterval), expressed in milliseconds.
          virtual property unsigned int MaxFrameInterval {
                unsigned int get();
          }
          
          // Resets maximum detected frame interval to 0.
          virtual void ResetMaxFrameInterval();
        
          // Enter/ Leave standby. Currently, in standby the only difference to normal operating mode is that the barcode 
          // reading interval is high thus preventing barcode reader module license complaining problems.
         virtual void EnterStandby();
         virtual void LeaveStandby();

         // Maximum intensity for blood
		 virtual property BYTE BloodDetectionColorMax {
			 BYTE get();
			 void set(BYTE value);
		 }

        private:

          // Actual camera driver manager
          DSRenderManager* m_pRenderManager;

          // Helpers for converting co-ordinates to image co-ordinates. In the received image,
          // the 0, 0 location is in the bottom left corner.
          ImageZeroPointLocation m_CameraZPL;
          ImageZeroPointLocation m_InterfaceZPL;

                  // Convert ExceptionInfo to PuncherCameraException (.NET exception object)
          void    ConvertToNETException(ExceptionInfo* pException);

                  // Validate settings. Throws exception (ExceptionInfo*) if they are invalid
          void    ValidateSettings(IPuncherCameraSettings^ camSettings);

                  // Copy/convert settings
          void    CopySettings(IPuncherCameraSettings^ camSettings, PDSRENDERMANAGERSETTINGS pRMSettings);

                  // Convert String to PTCHAR
          PTCHAR  StringToPTCHAR(String^ str);

                  // Convert PTCHAR to String
          String^ PTCHARToString(const PTCHAR psz);
          
                  // Throw exception is camera isn´t initialized. If throwNet is TRUE, method will throw
                  // PuncherCameraException otherwise it throws ExceptionInfo*
          void    ThrowNotInitializedException(BYTE throwNet, const PTCHAR pszMethod);

                  // Convert bitmap to format that is received from camera
          PBYTE   CreateCameraImage(Bitmap^ imageToConvert, LONG &rSize);

                  // UI image width/height
		  int     m_UIImageWidth;
		  int     m_UIImageHeight;
		};

    }
  }

}
