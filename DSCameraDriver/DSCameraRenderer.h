// --------------------------------------------------------------------------
// DSCameraRenderer.h
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera renderer. This code is called by the DirectShow 
// runtime, after the connection is created in DSRenderManager, when its time
// to render/process image.
//
// This class will use hotspots (m_pHotspots/m_pSearchAreaHotspot) to initiate
// the actual image processing. Hotspots are calibrated during initialization
// (if specific calibration image is specified) or automatically, when the first
// image is received from the camera. Area covered by the hotspot is cached and
// later we will compare it to received image, to detect whether the image has
// changed. When image is changed, the actual image processing is started.
//
// This calss will use BloodDetector (m_pBloodDetector/m_pBloodDetectorEx) for 
// detecting blood from the received image. PunchDetector (m_pPunchDetector) is
// used to find the requested punches from the area with is covered with blood.
// BarcodeReader (m_pBarcodeReader) is used to reade barcodes from the image.
// Barcodes are read from the image when the reading is turned on - the reading
// status is stored into global data storage -PunchPositionManager.
//
// When the connection to DirectShow runtime is created. CheckMediaType-method is 
// called by the DIrectShow runtime to detect which kind of image we accept. After
// the initial connection is created, ShouldDrawSampleNow-method is called to query
// if we want draw the image. We use this method to calc if enough time has passed
// from the last frame (m_FrameRenderInterval). DoRenderSample-method is called
// when its time to do the actual drawing, processing and drawing in ou case.
//
// Class inherits from the CBaseVideoRenderer, which is defined in the Microsft
// Windows SDK. It holds all the dirty COM related code and the basic functionality
// for the DirectShow renderer.
//
// TODO: - Fix so that that this class is only used for drawing the image, and place
//         the image processing to some other class
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 29.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once

#include "BloodDetector.h"
#include "BarcodeReader.h"
#include "PunchDetector.h"
#include "hotspot.h"


struct __declspec(uuid("{8D5D1A5D-F1B0-4d79-87F3-A47B5563531D}")) CLSID_DSCameraRenderer;


// Renderer settings
typedef struct _CAMERARENDERERSETTINGS {

    HWND              DrawingSurface;                   // Handle (HANDLE) to window where image is drawn

    BYTE              DrawPunchPositions;               // When TRUE, found punch positions are drawn
    BYTE              DrawSearchAreaBorder;             // When TRUE, search area border is drawn
    COLORREF          SearchAreaBorderColor;            // Color for search area border
    BYTE              SearchAreaBorderWidth;            // Search area border width

    INDEXVALUERANGE   SearchAreaColorOffsets;           // Min/max values which are used in search area hotspot activation

    BYTE              SearchAreaDetectionAccuracyX;     // X Accuracy for search area hotspot creation. This value specifies 
                                                        // how many pixel is skipped when collecting search area hotspot data

    BYTE              SearchAreaDetectionAccuracyY;     // Y Accuracy for search area hotspot creation. This value specifies 
                                                        // how many pixel is skipped when collecting search area hotspot data

    BloodDetector*    pBloodDetector;                   // Blood detector
    BarcodeReader*    pBarcodeReader;                   // Barcode reader
    PunchDetector*    pPunchDetector;                   // Punch detector

    BYTE              UseSecondBloodDetection;          // When TRUE, second blood detection (pBloodDetector2) is used for 
                                                        // punch positions that was found earlier. If FALSE, the normal blood 
                                                        // detection (pBloodDetector) is used

    BloodDetector*    pBloodDetector2;                  // Second blood detector that is used for pucnh positions that was 
                                                        // found earlier. Second blood detector is used when UseSecondBloodDetection
                                                        // is TRUE

	SIZE              UiImageSize;                      // Size of the UI image.

	USHORT            CameraImageWidth;                 // Actual camera image width
	USHORT            CameraImageHeight;                // Actual camera image height

	BYTE              PunchDetectionAccuracyX;          // Punch detection increment, X-axis. Number of pixels that is skipped,
                                                        // in X axis, when detecting punch positions

    BYTE              PunchDetectionAccuracyY;          // Punch detection increment, Y-axis. Number of pixels that is skipped,
                                                        // in Y axis, when detecting punch positions


    USHORT            FrameRenderInterval;              // Number of milliseconds that must pass between each drawn frame. 
                                                        // Camera can produce more images that we want draw. Lovering this value
                                                        // wil increase the UI experience, but will also increase the CPU usage.

    UINT              FramesSkippedNotificationLimit;   // Time in milliseconds that must elapse before error is send 
                                                        // that frames processing is skipped because camere cant access 
                                                        // shared data

    BYTE              DrawBlood;                        // When TRUE, found blood is also drawn
    COLORREF          BloodColor;                       // Color which is used to draw the blood

    eInputImageType   ImageType;                        // Type of the image that is get from the camera

    BYTE              DrawHotspots;                     // When TRUE, hotspots are drawn

    COLORREF          HotspotColor;                     // Color for drawing hotspots (borders)
    BYTE              HotspotCount;                     // Hotspot count

    USHORT            HotspotOffsetY;                   // Hotspot offset from the bottom of the image (row 0).
                                                        // Used when placing the center of the hotspots

    SIZE              HotspotSize;                      // Size of one hotspot

    INDEXVALUERANGE   HotspotColorOffsets;              // Min/max pixel values that is used in hotspot activation

    BYTE              HotspotDetectionAccuracyX;        // Increment in X axis that is used when calculating hotspot
    // index values
    BYTE              HotspotDetectionAccuracyY;        // Increment in Y axis that is used when calculating hotspot
    // index values

    BYTE              AllHotspotsMustBeInvalid;         // When TRUE, all hotspot area must contain something in order

    BYTE            SearchAreaAllowedInvalidPixelPercent;   // Number of pixel (percent) that needs to be invalid in
                                                            // search area hotspot before it is considered invalid. If this is
                                                            // 0, one invalid pixel will activate hotspot

    BYTE              HotspotAllowedInvalidPixelPercent;    // Number of pixel (percent) that needs to be invalid before
                                                            // hotspot is activated

    PBYTE             CalibrationImage;                 // Image that can be used to calibrate the camera. If this is
                                                        // NULL, automatic calibration is used, which uses the next
                                                        // image that is received from the camera

    BYTE              FlipHorizontally;                 // When TRUE, received image is flipped horizontally
    BYTE              FlipVertically;                   // When TRUE, received image is flipped vertically

} CAMERARENDERERSETTINGS, *PCAMERARENDERERSETTINGS;



class DSCameraRenderer : public CBaseVideoRenderer {

    public:
        DSCameraRenderer(IUnknown* unk, HRESULT *hr);
        ~DSCameraRenderer();

        // Initialize renderer
        void                  InitializeRenderer(const PCAMERARENDERERSETTINGS pSettings);

        // Get current renderer settings
        void                  GetCurrentSettings(PCAMERARENDERERSETTINGS pSettings);

        // Update current settings
        void                  UpdateSettings(const PCAMERARENDERERSETTINGS pSettings);

        // Called by DirectShow runtime when it is detecting which format we accept.
        // This is called when the camera output and our input pin are connected
        virtual HRESULT       CheckMediaType(const CMediaType *media);

        // Called by DirectShow runtime when wee need to draw image
        virtual HRESULT       DoRenderSample(IMediaSample *pMediaSample);

        // Called by DirectShow runtime to detect whether we want draw the next frame or not
        virtual HRESULT       ShouldDrawSampleNow(IMediaSample *sample, REFERENCE_TIME *start, REFERENCE_TIME *stop);

        // Release all hotspots. Hotspots are used when detecting if image contains card or not
        void                  ReleaseHotspots();

        // Create hotspot which are used to detect whether picture contains card or not
        void                  CreateHotspots(const PBYTE pImage);

        // Initialize image capturing. Next image that is received from the DirectShow runtime,
        // is cached, and notification event is sent (GetImageCapturedEvent)
        void                  InitiateImageCapturing();

        // Release data in display image buffer
        void                  ReleaseDisplayImageBuffer();

        // Set data in display image buffer. When this is != NULL, live image is replaced with this
        void                  SetDisplayImageBuffer(const PBYTE pImage, LONG imageSize);

        // Release previously captured image
        void                  ReleaseCapturedImage();

        // Whether the module contains a previously captured image
        bool                  CapturedImageExists() {return (m_CapturedImage != NULL) ? true: false;}

        // Update drawing surface. In order to call this safely, renderer must be initialized and you
        // must first lock the global data and then pause the image processing. This method will 
        // release all the currently reserved GDI objects, and then recreate them so that they are 
        // associated to the new drawing surface.
        void                  UpdateDrawingSurface(HWND hDrawingSurface);

        // Returns TRUE when image contains card (= hotspots are invalid)
        inline BYTE           IsCardDetected()                { return m_BloodcardDetected; }

        // Get barcode reader
        inline BarcodeReader* GetBarcodeReader()              { return m_pBarcodeReader; }

        // Get punch detector
        inline PunchDetector* GetPunchDetector()              { return m_pPunchDetector; }

        // Get blood detector. When getMain is TRUE, main detector (m_pBloodDetector) is returned.
        // When getMain is FALSE, secondary detector (m_pBloodDetectorEx) is returned.
        inline BloodDetector* GetBloodDetector(BYTE getMain)  { return getMain ? m_pBloodDetector : m_pBloodDetectorEx; }

        // Get event handle for the image captured event
        inline HANDLE         GetImageCapturedEvent()         { return m_ImageCapturedEventHAndle; }

        // Get captured image
        inline const PBYTE    GetCapturedImage()              { return m_CapturedImage; }

        // Returns TRUE if display image (= still image( is used
        inline BYTE           IsDisplayImageUsed()            { return (m_DisplayImageBuffer != NULL); }

        // Punch Border, the area outside the actual punch area, that is reserved for punch position inaccuracies, the
        // border area needs to be in blood too (assuming that the punch area needs to be in blood), and no other punches
        // may be even partially positioned on this border area.
        void                  SetPunchBorder(BYTE border);
        BYTE                  GetPunchBorder();

        // Restarts imaging after a pause
        void                  RestartImaging();

        // Resets maximum detected frame interval to 0.
        void                  ResetMaxFrameInterval()       { m_imageIntervalMax = 0;}

        // Maximum detected camera frame interval since instrument startup (or last call to ResetMaxFramInterval), expressed in milliseconds.
        DWORD                 GetMaxFrameInterval() {return m_imageIntervalMax;}

        // Filtered measured camera frame interval since instrument startup, expressed in milliseconds.
        DWORD                 GetFilteredFrameInterval() {return m_imageIntervalFiltered;}

        // Whether the blood detection is on or off. 
        void            SetBloodDetectionOn(bool value);
        bool            GetBloodDetectionOn();

    private:
        // Pointer to output image data
        PBYTE           m_outputBits;

        // Whether the blood detection is on or off. 
        bool            m_bloodDetectionIsOn;

        // Time in ticks, of previous image. Value 0 indicates not initialized (the value 0 could also occur naturally, so
        // the code must be written respecting that)
        DWORD             m_previousImageTime;

        // Worst case image interval (not counting the times the imaging has been stopped)
        DWORD             m_imageIntervalMax;

        // Low-pass filtered image interval (not counting the times the imaging has been stopped)
        DWORD             m_imageIntervalFiltered;

        // Lock for image processing. When locked, the image processing is skipped in all the 
        // DirectShow "callbacks." The image isn´t even drawn to the drawing surface. Used when
        // updating drawing surface
        CRITICAL_SECTION  m_ImageProcessingLock;

        // True when renderer is initialized
        BYTE              m_bInitialized;

        // Buffer to hold display image. When this is != NULL, live image is replaced with this
        PBYTE             m_DisplayImageBuffer;

        // When m_DisplayImageBuffer != NULL, it is used to replace the live image. The content
        // of the m_DisplayImageBuffer is copied to this buffer, and this is used in actual drawing
        // process: some of the drawing is done without GDI, by changing the actual pixel values
        // in this buffer, so we need to keep the orginal image intact.
        PBYTE             m_DisplayImageBufferTmp;

        // Size of the m_DisplayImageBuffer/m_DisplayImageBufferTmp buffer
        LONG              m_DisplayImageSize;

        // When TRUE, received image is flipped horizontally
        BYTE              m_FlipHorizontally;

        // When TRUE, received image is flipped vertically
        BYTE              m_FlipVertically;

        // When image flipping is in use, this buffer is used to store the flipped image
        PBYTE             m_ImageFlipBuffer;

        // Handle to even twhich is used to notify listener that image has been captured,
        // and can bet get using the GetCapturedImage-method
        HANDLE            m_ImageCapturedEventHAndle;

        // Buffer that will hold the captured image
        PBYTE             m_CapturedImage;

        // When TRUE, next received image is stored to buffer (m_CapturedImage) and event
        // (m_ImageCapturedEventHAndle) is signaled
        BYTE              m_ImageRequested;

        // When TRUE, found blood is also drawn
        BYTE              m_DrawBlood;

        // Color which is used to draw the blood (R, G, B)
        BYTE              m_BloodColorR;
        BYTE              m_BloodColorG;
        BYTE              m_BloodColorB;

        // Number of pixel (percent) that needs to be invalid in search area hotspot before 
        // it is considered invalid. If this is 0, one invalid pixel will activate hotspot
        BYTE              m_SearchAreaAllowedInvalidPixelPercent;

        // Number of pixel (percent) that needs to be invalid before hotspot is activated
        BYTE              m_HotspotAllowedInvalidPixelPercent;

        // Type of the image that is received from the camera
        eInputImageType   m_ImageType;

        // Punch detection increment, X-axis. Number of pixels that is skipped, in X axis,
        // when detecting punch positions
        BYTE              m_PunchDetectionAccuracyX;        

        // Punch detection increment, Y-axis. Number of pixels that is skipped, in Y axis,
        // when detecting punch positions
        BYTE              m_PunchDetectionAccuracyY;

        // Number of milliseconds that must pass between each drawn frame. Camera can 
        // produce more images that we want draw. Lovering this value wil increase the UI 
        // experience, but will also increase the CPU usage.
        USHORT            m_FrameRenderInterval;

        // When TRUE, found punch positions are drawn
        BYTE              m_DrawPunchPositions;

        // Blood detector
        BloodDetector*    m_pBloodDetector;

        // When TRUE, second blood detection (pBloodDetector2) is used for punch positions 
        // that was found earlier. If FALSE, the normal blood detection (pBloodDetector) is used
        BYTE              m_UseSecondBloodDetection;

        // Second blood detector that is used for pucnh positions that was found earlier. Second 
        // blood detector is used when UseSecondBloodDetection is TRUE
        BloodDetector*    m_pBloodDetectorEx;

        // Barcode reader
        BarcodeReader*    m_pBarcodeReader;

        // Punch detector
        PunchDetector*    m_pPunchDetector;

        // HANDLE to window where we need to draw the images
        HWND              m_hDrawingSurface;

        // DC handle to drawing surface
        HDC               m_hDC;

        // Memory DC. Actual drawing is done to memory mapped bitmap, which is then
        // flipped to actual DC (m_hDC)
        HDC               m_hMemoryDC;

        // Memory bitmap. Actual drawing is done to this and then its drawn to
        // actual drawing surface
        HBITMAP           m_hMemoryBMP;

        // Color used to draw the search area border
        COLORREF          m_SearchAreaBorderColor;

        // When TRUE search area border is drawn
        BYTE              m_DrawSearchAreaBorder;

        // Search area border width
        BYTE              m_SearchAreaBorderWidth;

        // Drawing area
        RECT              m_rcClientRect;

        // DC and BMP for barcode reader
        HDC               m_hDCForBCReader;
        HBITMAP           m_hBMPForBCReader;

		// Camera image width/height
		int               m_ImageWidth;
        int               m_ImageHeight;

        // Image information, used when drawing image
        BITMAPINFO        m_BitmapInfo;

        // When TRUE, blood card has been detected. Used when notifying UI
        // whether blood card was detected/removed
        BYTE              m_BloodcardDetected;

        // Min/max values which are used in search area hotspot activation
        INDEXVALUERANGE   m_SearchAreaColorOffsets;


        // X Accuracy for search area hotspot creation. This value specifies 
        // how many pixel is skipped when collecting search area hotspot data
        BYTE              m_SearchAreaDetectionAccuracyX;

        // Y Accuracy for search area hotspot creation. This value specifies 
        // how many pixel is skipped when collecting search area hotspot data
        BYTE              m_SearchAreaDetectionAccuracyY;

        // Used when detecting whether we should draw next frame or not
        DWORD             m_LastRenderTime;

        // Time in milliseconds that must elapse from the first skipped frame before
        // error notification is send
        DWORD             m_FrameSkippedNotificationLimit;

        // Time when frame skipping started. Used when frame processing is skipped because 
        // we cant gain acces to data and frames  are skipped - they are just drawn without
        // any processing
        DWORD             m_FrameSkippingStartTime;

        // When TRUE, hotspots, which are used to initiate barcode reading, and detect 
        // whether card is present or not, is also drawn
        BYTE              m_DrawHotspots;

        // Color for drawing hotspots (borders)
        COLORREF          m_HotspotColor;

        // Hotspot count
        BYTE              m_HotspotCount;

        // Hotspot offset from the bottom of the image (row 0). Used when placing the center of the hotspots
        USHORT            m_HotspotOffsetY;

        // Size of one hotspot
        SIZE              m_HotspotSize;

        // Min/max pixel values that is used in hotspot activation
        INDEXVALUERANGE   m_HotspotColorOffsets;

        // Increment in X axis that is used when calculating hotspot index values
        BYTE              m_HotspotDetectionAccuracyX;

        // Increment in Y axis that is used when calculating hotspot index values
        BYTE              m_HotspotDetectionAccuracyY;

        // When TRUE, all hotspot area must contain something in order
        BYTE              m_AllHotspotsMustBeInvalid;

        // Hotspots. Used when detecting whether barcode reading should be started
        Hotspot*          m_pHotspots;



        // Return TRUE when display image is set and the InPunchProgress flag is set
        // (g_pPunchPosManager->GetInPunchProgressMode)
        BYTE    InPunchProgressMode();

        // If image is requested (m_ImageRequested = TRUE), passed image is copied and event is signaled
        void    HandleImageCapturing(const PBYTE pImage);

        // Draw hotspots
        void    DrawHotspots();

        // Cache settings to local variables
        void    CopySettings(BYTE update, const PCAMERARENDERERSETTINGS pSettings);

        // Paint punch holes
        void    PaintPunchMap();

        // Throw exception if renderer is already initialized
        void    ThrowAlreadyInitializedException(const PTCHAR pszMethod);

        // Throw exception if renderer isn´t initialized
        void    ThrowNotInitializedException(const PTCHAR pszMethod);

        // Search are doesn´t contain card
        void    OnEmpty(PBYTE pImageData);

        // Card detected, detect blood and punch positions
        void    OnCard(PBYTE pImageData, bool disablePunchSearch);

        // Detect punch positions and raise event if their count/position is changed
        void    DetectPunchPositions(PBYTE pImageData);

        // Check if passed punch positions are valid, using the passed detectors. Returns the
        // number of valid/good positions.
        BYTE    CheckPunchPositions(BloodDetector* pBloodDetector, PunchDetector* pPunchDetector,
            BYTE count, PUNCHPOSITION* pPositions);

        // Detect punch positions. Return the number of found positions
        BYTE    DetectPunchPositions(BloodDetector* pBloodDetector, PunchDetector* pPunchDetector,
            BYTE detectionAccuracyX, BYTE detectionAccuracyY, BYTE count, PUNCHPOSITION* pPositions);

        // Releas punch positions (pPositions) memory allocation
        void    ReleasePunchPositions(PPUNCHPOSITION& pPositions);

        // Store punch positions to global data manager
        void    StorePositions(DWORD detectionTime, BYTE count, PUNCHPOSITION* pPositions);

        // Reset punch position info
        void    ResetPunchPositions(BYTE count, PUNCHPOSITION* pPositions);

        // Convert from camera to UI co-ordinates
        void    ConvertToGDIZpl(const POINT& ptIn, POINT& ptOut);

        // Draw punch position numbers over the punch holes
        void    DrawPunchNumbers();

        // Draw search area border
        void    DrawSearchArea();

        // Create various GDI objects that are used for drawing
        void    CreateGDIObjects();

        // Release used GDI objects
        void    ReleaseGDIObjects();

        // Frame processing was skipped because we can´t gain access to shared data.
        // Detect whether we should notify client about that
        void    FrameProcessingSkipped();

        // When m_DrawBlood is TRUE, found blood is also drawn
        void    DrawBlood(PBYTE pImageData);

        // Read barcodes from the image. Barcodes are read if the reading is turned on -
        // PunchPositionManager.GetReadBarcodesState-method and if enought time have
        // elapsed from the last read operation BarcodeReader.IsReadRequired
        bool   ReadBarcodes(const PBYTE pImage);

        // Return TRUE if hotspot(s) contain something
        BYTE    CheckHotspots(const PBYTE pImage);

        // Flip image horizontally and vertically, if flipping is used
        PBYTE   FlipImage(const PBYTE pImage);

        // Allocate buffer that is using when image flipping is used
        void    AllocateFlipBuffer();

        // Release buffer that is used when image flipping is used
        void    ReleaseFlipBuffer();

};