// --------------------------------------------------------------------------
// DSCamera.cpp
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera driver implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 27.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#include "stdafx.h"
#include <Vcclr.h>

#include "DSCamera.h"
#include "BloodDetector.h"
#include "ExceptionInfo.h"
#include "ErrorCodes.h"

#using "..\\..\\..\\..\\Common\\Bin\\OnSpotLibrary.dll"
using namespace PKI::Puncher::Library;
using namespace PKI::Puncher::DSCameraDriver;

// --------------------------------------------------------------------------

// Used in exception if class name is NULL or missing
const PTCHAR STR_ERR_NULL_CLASSNAME               = _T("(Classname is NULL)");

// Used in exception if method name is NULL or missing
const PTCHAR STR_ERR_NULL_METHODNAME              = _T("(Methodname is NULL)");

// Max allowed detection percent
#define BLOODDETECTOR_MAX_DETECTION_PERCENT       100

// Class name, used as a param to exception methods
_CLASSNAME(_T("DSCamera"));

// --------------------------------------------------------------------------

DSCamera::DSCamera() {

  m_pRenderManager  = NULL;

  m_CameraZPL       = ImageZeroPointLocation::BottomLeft; 
  m_InterfaceZPL    = ImageZeroPointLocation::TopLeft;

  m_UIImageWidth = 0;
  m_UIImageHeight = 0;
}

DSCamera::~DSCamera() {

  Release();

}

void DSCamera::Initialize(IPuncherCameraSettings^ camSettings) {

  _METHODNAME(_T("Initialize"));
  if(m_pRenderManager == NULL) {

    BYTE errorOccured = TRUE;
    DSRENDERMANAGERSETTINGS rms;
    ::ZeroMemory(&rms, sizeof(DSRENDERMANAGERSETTINGS));

    try {

      ValidateSettings(camSettings);

      // Create the actual camera manager and find the camera
      m_pRenderManager = new DSRenderManager();
      m_pRenderManager->InitializeCamera();

      // Get camera properties
      camSettings->CameraType = PTCHARToString(m_pRenderManager->GetCameraType());
      camSettings->CameraImageWidth = m_pRenderManager->GetCameraImageWidth();
      camSettings->CameraImageHeight = m_pRenderManager->GetCameraImageHeight();
      camSettings->CameraImageWidth_mm = m_pRenderManager->GetCameraImageWidth_mm();
      camSettings->CameraImageHeight_mm = m_pRenderManager->GetCameraImageHeight_mm();

      // Initialize global data storage. This helper class is used to pass
      // data to actual image renderer
      g_pPunchPosManager = new PunchPositionManager();
      g_pPunchPosManager->Initialize();

      // Convert settings
      CopySettings(camSettings, &rms);

      // Initialize the actual camera manager
      m_pRenderManager->Initialize(&rms);

      errorOccured = FALSE;

    } catch (ExceptionInfo* pEX) {
      ConvertToNETException(pEX);
    } catch (...) {
      ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_INITIALIZE_FAILED));
    } finally {

      ReleaseString(rms.pszSimulationFile);
      ReleaseString(rms.barcodeReaderSettings.pszReaderBinaryLocation);

      if(rms.CalibrationImage != NULL) {
        delete []rms.CalibrationImage;
      }

      if(errorOccured) {
        if(m_pRenderManager != NULL) {
          try {
            delete m_pRenderManager;
          } catch (...) {
          }

          m_pRenderManager = NULL;
        }
      }
    }
  }

}

void DSCamera::Release() {

  _METHODNAME(_T("Release"));

  try {

    if(m_pRenderManager != NULL) {
      // Release actual camera manager. This will disconnect the DirectShow connection
      m_pRenderManager->Release();

      delete m_pRenderManager;
      m_pRenderManager = NULL;
    }

    if(g_pPunchPosManager != NULL) {
      // Release global data storage
      g_pPunchPosManager->Release();

      delete g_pPunchPosManager;
      g_pPunchPosManager = NULL;
    }

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_RELEASE_FAILED));
  }

}

void DSCamera::Start() {

  _METHODNAME(_T("Start"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    m_pRenderManager->Start();

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_START_FAILED));
  }

}

void DSCamera::Stop() {

  _METHODNAME(_T("Stop"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    m_pRenderManager->Stop();

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_STOP_FAILED));
  }

}

void DSCamera::SetDrawingSurface(IntPtr newDrawingSurface) {

  _METHODNAME(_T("SetDrawingSurface"));
  ThrowNotInitializedException(TRUE, METHODNAME);
  BYTE locked = FALSE;


  try {

    HWND hNewSurface = (HWND)newDrawingSurface.ToInt32();
    if(!IsWindow(hNewSurface)) {
      // Error - Invalid drawing surface
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_DRAWINGSURFACE_IS_INVALID);
    }

    // Lock global data - just in vase
    g_pPunchPosManager->Lock();
    locked = TRUE;

    // Update!
    m_pRenderManager->GetCameraRenderer()->UpdateDrawingSurface(hNewSurface);

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_SET_DRAWING_SURFACE_FAILED));
  } finally {
    if(locked) {
      g_pPunchPosManager->Unlock();
    }
  }

}


BYTE DSCamera::PunchBorder::get() {
  return m_pRenderManager->GetPunchBorder();
}

void DSCamera::PunchBorder::set(BYTE value) {

	CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
		m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
		m_UIImageWidth, m_UIImageHeight,
		m_CameraZPL, m_InterfaceZPL);
	m_pRenderManager->SetPunchBorder(converter->Scale(value));
}

BYTE DSCamera::BloodBorder::get() {
  return m_pRenderManager->GetCameraRenderer()->GetPunchDetector()->GetBloodBorder();
}

void DSCamera::BloodBorder::set(BYTE value) {

  CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
    m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
    m_UIImageWidth, m_UIImageHeight,
    m_CameraZPL, m_InterfaceZPL);
  m_pRenderManager->GetCameraRenderer()->GetPunchDetector()->SetBloodBorder(converter->Scale(value));
}


void DSCamera::BloodDetectionOn::set(bool value) {
    m_pRenderManager->GetCameraRenderer()->SetBloodDetectionOn(value);
}


bool DSCamera::BloodDetectionOn::get() {
    return m_pRenderManager->GetCameraRenderer()->GetBloodDetectionOn();
}


unsigned int DSCamera::FilteredFrameInterval::get() {
    return m_pRenderManager->GetCameraRenderer()->GetFilteredFrameInterval();
}

unsigned int DSCamera::MaxFrameInterval::get() {
    return m_pRenderManager->GetCameraRenderer()->GetMaxFrameInterval();
}


void DSCamera::ResetMaxFrameInterval() {
    m_pRenderManager->GetCameraRenderer()->ResetMaxFrameInterval();
}


void DSCamera::DisplayImage(Bitmap^ imageToShow) {

  _METHODNAME(_T("DisplayImage"));
  ThrowNotInitializedException(TRUE, METHODNAME);
  BYTE locked = FALSE;

  try {

    if(imageToShow != nullptr) {
      if(imageToShow->Width != m_pRenderManager->GetCameraImageWidth() ||
        imageToShow->Height != m_pRenderManager->GetCameraImageHeight()) {
        // Invalid size
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_DISPLAY_IMAGE_INVALID_SIZE);
      }
    }

    // Lock data
    g_pPunchPosManager->Lock();
    locked = TRUE;

    // Release
    m_pRenderManager->GetCameraRenderer()->ReleaseDisplayImageBuffer();
    if(imageToShow != nullptr) {

      // Store image
      LONG imageSize  = 0;
      PBYTE pImg      = CreateCameraImage(imageToShow, imageSize);

      m_pRenderManager->GetCameraRenderer()->SetDisplayImageBuffer(pImg, imageSize);

      // NOTE: we don´t need to release pImg, it will be released when camera is released,
      //       or when new image is specified
    
    }    

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_DISPLAY_IMAGE_FAILED));
  } finally {
    if(locked) {
      g_pPunchPosManager->Unlock();
    }
  }

}

void DSCamera::Calibrate(Bitmap^ calibrationImage) {

  _METHODNAME(_T("Calibrate"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  PBYTE pImg = NULL;

  try {

    if(calibrationImage != nullptr) {
      if(calibrationImage->Width != m_pRenderManager->GetCameraImageWidth() ||
        calibrationImage->Height != m_pRenderManager->GetCameraImageHeight()) {
        // Invalid size
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_CALIBRATION_IMAGE_INVALID_SIZE);
      }
    }

    g_pPunchPosManager->Lock();

    // Release previous calibration info
    m_pRenderManager->GetCameraRenderer()->ReleaseHotspots();
    if(calibrationImage != nullptr) {
      LONG dummy;
      pImg = CreateCameraImage(calibrationImage, dummy);
      m_pRenderManager->GetCameraRenderer()->CreateHotspots(pImg);
    }

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_CALIBRATE_FAILED));
  } finally {

    if(pImg != NULL) {
      delete []pImg;
    }

    g_pPunchPosManager->Unlock();
  }

}

IPuncherCameraSettings^ DSCamera::Settings::get() {

  // Get the camera settings in UI pixels

  _METHODNAME(_T("Settings::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  DSRENDERMANAGERSETTINGS rms;
  rms.barcodeReaderSettings.pszReaderBinaryLocation = NULL;
  rms.pszSimulationFile = NULL;

  try {

    g_pPunchPosManager->Lock();
    m_pRenderManager->GetCurrentSettings(&rms);

    CameraSettings^ camSettings                   = gcnew CameraSettings();

    CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
      camSettings->UiImageSize.Width, camSettings->UiImageSize.Height,
      m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
      m_InterfaceZPL, m_CameraZPL);

    camSettings->FlipHorizontally                 = rms.FlipHorizontally ? true : false;
    camSettings->FlipVertically                   = rms.FlipVertically ? true : false;

    camSettings->SearchAreaAllowedInvalidPixelPercent = rms.SearchAreaAllowedInvalidPixelPercent;
    camSettings->HotspotAllowedInvalidPixelPercent = rms.HotspotAllowedInvalidPixelPercent;

    camSettings->DrawBlood                        = rms.DrawBlood ? true : false;
    camSettings->BloodColor                       = Color::FromArgb( GetRValue(rms.BloodColor),
                                                                     GetGValue(rms.BloodColor),
                                                                     GetBValue(rms.BloodColor) );

    camSettings->PunchDetectionAccuracyX          = rms.PunchDetectionAccuracyX - 1;
    camSettings->PunchDetectionAccuracyY          = rms.PunchDetectionAccuracyY - 1;
    camSettings->FrameRenderInterval              = rms.FrameRenderInterval;
    camSettings->FramesSkippedNotificationLimit   = rms.FramesSkippedNotificationLimit;

    camSettings->BarcodeCount                     = rms.barcodeReaderSettings.maxBarcodeCount;
    camSettings->BarcodeReader                    = BarcodeReaderType::DTK;
    camSettings->BloodDetection                   = BloodDetectionMethod::DetectBlood;
    camSettings->CameraConnection                 = PuncherCameraConnectionType::DirectShowCamera;

    camSettings->BarcodeReadingInterval1          = rms.barcodeReaderSettings.usBarcodeReadingInterval1;
    camSettings->BarcodeReadingInterval2          = rms.barcodeReaderSettings.usBarcodeReadingInterval2;

    camSettings->EnableScanRect1 = rms.barcodeReaderSettings.enableScanRect1 != 0 ? TRUE : FALSE;
    camSettings->ScanRect1Left = converter->ScaleX(rms.barcodeReaderSettings.scanRect1Left);
    camSettings->ScanRect1Top = converter->ScaleY(rms.barcodeReaderSettings.scanRect1Top);
    camSettings->ScanRect1Right = converter->ScaleX(rms.barcodeReaderSettings.scanRect1Right);
    camSettings->ScanRect1Bottom = converter->ScaleY(rms.barcodeReaderSettings.scanRect1Bottom);

    camSettings->EnableScanRect2 = rms.barcodeReaderSettings.enableScanRect2 != 0 ? TRUE : FALSE;
    camSettings->ScanRect2Left = converter->ScaleX(rms.barcodeReaderSettings.scanRect2Left);
    camSettings->ScanRect2Top = converter->ScaleY(rms.barcodeReaderSettings.scanRect2Top);
    camSettings->ScanRect2Right = converter->ScaleX(rms.barcodeReaderSettings.scanRect2Right);
    camSettings->ScanRect2Bottom = converter->ScaleY(rms.barcodeReaderSettings.scanRect2Bottom);

    camSettings->EnableScanRect3 = rms.barcodeReaderSettings.enableScanRect3 != 0 ? TRUE : FALSE;
    camSettings->ScanRect3Left = converter->ScaleX(rms.barcodeReaderSettings.scanRect3Left);
    camSettings->ScanRect3Top = converter->ScaleY(rms.barcodeReaderSettings.scanRect3Top);
    camSettings->ScanRect3Right = converter->ScaleX(rms.barcodeReaderSettings.scanRect3Right);
    camSettings->ScanRect3Bottom = converter->ScaleY(rms.barcodeReaderSettings.scanRect3Bottom);

    camSettings->BarcodeRedundancy = rms.barcodeReaderSettings.barcodeRedundancy;
    camSettings->MinimumBarcodeHeight = converter->ScaleY(rms.barcodeReaderSettings.minimumBarcodeHeight);

    camSettings->SearchAreaDetectionAccuracyX     = converter->ScaleX(rms.SearchAreaDetectionAccuracyX - 1);
    camSettings->SearchAreaDetectionAccuracyY     = converter->ScaleY(rms.SearchAreaDetectionAccuracyY - 1);

    camSettings->BloodColorMin                    = Color::FromArgb( GetRValue(rms.BloodColorMin),
                                                                     GetGValue(rms.BloodColorMin),
                                                                     GetBValue(rms.BloodColorMin) );

    camSettings->BloodColorMax                    = Color::FromArgb( GetRValue(rms.BloodColorMax),
                                                                     GetGValue(rms.BloodColorMax),
                                                                     GetBValue(rms.BloodColorMax) );

    camSettings->UseSecondBloodDetection          = rms.UseSecondBloodDetection ? true : false;

    camSettings->BloodColorMin2                   = Color::FromArgb( GetRValue(rms.BloodColorMin2),
                                                                     GetGValue(rms.BloodColorMin2),
                                                                     GetBValue(rms.BloodColorMin2) );

    camSettings->BloodColorMax2                   = Color::FromArgb( GetRValue(rms.BloodColorMax2),
                                                                     GetGValue(rms.BloodColorMax2),
                                                                     GetBValue(rms.BloodColorMax2) );

    camSettings->DrawingSurface                   = (IntPtr)gcnew IntPtr((int)rms.DrawingSurface);
    camSettings->DrawPunchPositions               = rms.DrawPunchPositions ? true : false;
    camSettings->DrawSearchAreaBorder             = rms.DrawSearchAreaBorder ? true : false;
    camSettings->SearchAreaDetectionColorMinOffset  = rms.SearchAreaColorOffsets.dwMax;
    camSettings->SearchAreaDetectionColorMaxOffset  = rms.SearchAreaColorOffsets.dwMin;
    camSettings->InSimulationMode                 = rms.InSimulationMode ? true : false;
    camSettings->SimulationFile                   = PTCHARToString(rms.pszSimulationFile);
    camSettings->PunchBorder                      = converter->Scale(rms.PunchBorder);

    camSettings->RequiredBloodPercent             = rms.RequiredBloodPercent;
    camSettings->SearchAreaBorderColor            = Color::FromArgb( GetRValue(rms.SearchAreaBorderColor),
                                                                     GetGValue(rms.SearchAreaBorderColor),
                                                                     GetBValue(rms.SearchAreaBorderColor) );

    camSettings->SearchAreaBorderWidth            = converter->Scale(rms.SearchAreaBorderWidth);

    Point pt = (Point)gcnew Point(rms.SearchAreaCenter.x, rms.SearchAreaCenter.y);


    camSettings->SearchAreaCenter                 = converter->Convert(pt);
    camSettings->SearchAreaRadius                 = converter->Scale(rms.SearchAreaRadius);
    camSettings->BarcodeReaderLocation            = PTCHARToString(rms.barcodeReaderSettings.pszReaderBinaryLocation);

    camSettings->DrawHotspots                     = rms.DrawHotspots ? true : false;
    camSettings->HotspotColor                     = Color::FromArgb( GetRValue(rms.HotspotColor),
                                                                     GetGValue(rms.HotspotColor),
                                                                     GetBValue(rms.HotspotColor) );

    camSettings->HotspotCount                     = rms.HotspotCount;


    pt.Y = rms.HotspotOffsetY;
    pt = converter->Convert(pt);

    camSettings->HotspotOffsetY                   = pt.Y;

    camSettings->HotspotSize                      = (Size)gcnew Size(converter->ScaleX(rms.HotspotSize.cx), converter->ScaleY(rms.HotspotSize.cy));
    camSettings->HotspotDetectionColorMinOffset   = rms.HotspotColorOffsets.dwMin;
    camSettings->HotspotDetectionColorMaxOffset   = rms.HotspotColorOffsets.dwMax;
    camSettings->HotspotDetectionAccuracyX        = converter->ScaleX(rms.HotspotDetectionAccuracyX - 1);
    camSettings->HotspotDetectionAccuracyY        = converter->ScaleY(rms.HotspotDetectionAccuracyY - 1);
    camSettings->AllHotspotsMustBeInvalid         = rms.AllHotspotsMustBeInvalid ? true : false;



    switch(rms.InputImage) {

      case GrayscaleImage:
        camSettings->InputImage = InputImageType::Grayscale;
        break;

      case ColorImage:
        camSettings->InputImage = InputImageType::Color;
        break;

    }


    BYTE bcReadDir = 0;
    if(rms.barcodeReaderSettings.BcOrientation & BCR_RD_LEFT_TO_RIGHT) { bcReadDir |= (BYTE)BarcodeOrientation::LeftToRight; }
    if(rms.barcodeReaderSettings.BcOrientation & BCR_RD_RIGHT_TO_LEFT) { bcReadDir |= (BYTE)BarcodeOrientation::RightToLeft; }
    if(rms.barcodeReaderSettings.BcOrientation & BCR_RD_TOP_TO_BOTTOM) { bcReadDir |= (BYTE)BarcodeOrientation::TopToBottom; }
    if(rms.barcodeReaderSettings.BcOrientation & BCR_RD_BOTTOM_TO_TOP) { bcReadDir |= (BYTE)BarcodeOrientation::BottomToTop; }
    camSettings->BarcodeReadingDirection = bcReadDir;


    // This is set in upper level
    //camSettings->DriverImplementationFile = nullptr;

    // Get camera properties
    camSettings->CameraType = PTCHARToString(m_pRenderManager->GetCameraType());
    camSettings->CameraImageWidth = m_pRenderManager->GetCameraImageWidth();
    camSettings->CameraImageHeight = m_pRenderManager->GetCameraImageHeight();
    camSettings->CameraImageWidth_mm = m_pRenderManager->GetCameraImageWidth_mm();
    camSettings->CameraImageHeight_mm = m_pRenderManager->GetCameraImageHeight_mm();

    return camSettings;

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_SETTINGS_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();

    ReleaseString(rms.barcodeReaderSettings.pszReaderBinaryLocation);
    ReleaseString(rms.pszSimulationFile);
  }

  return nullptr;

}

void DSCamera::Settings::set(IPuncherCameraSettings^ value) {

  // Set the camera settings in UI pixels. This is called from the service program

  _METHODNAME(_T("Settings::set"));
  ThrowNotInitializedException(TRUE, METHODNAME);
  BYTE locked = FALSE;

  DSRENDERMANAGERSETTINGS rms;
  ::ZeroMemory(&rms, sizeof(DSRENDERMANAGERSETTINGS));

  try {

    // Image size cant be changed without performing reinit
    if(value->UiImageSize.Width != m_UIImageWidth ||
      value->UiImageSize.Height != m_UIImageHeight) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_CANT_CHANGE_IMAGESIZE);
    }

    ValidateSettings(value);

    // Lock renderer
    g_pPunchPosManager->Lock();
    locked = TRUE;


    CopySettings(value, &rms);

    // Update!
    m_pRenderManager->UpdateSettings(&rms);

  } catch (ExceptionInfo* pEX) {
    ConvertToNETException(pEX);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_SETTINGS_SET_FAILED));
  } finally {

    if(rms.CalibrationImage != NULL) {
      delete []rms.CalibrationImage;
    }

    if(locked) {
      g_pPunchPosManager->Unlock();
    }
  }

}

IPunchPositions^ DSCamera::PunchPositions::get() {

  _METHODNAME(_T("PunchPositions::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  PKI::Puncher::Library::PunchPositions^ pp = nullptr;
  BYTE locked = FALSE;

  BYTE count = 0;

  try {

    BYTE cElements;
    BYTE i;
    PunchPosition^ pos;

    pp = gcnew PKI::Puncher::Library::PunchPositions();

    pp->DrawId                    = g_pPunchPosManager->GetDrawId() ? true : false;
    pp->DrawIdOnlyInBlood         = g_pPunchPosManager->GetDrawIdOnlyInBlood() ? true : false;
    pp->FillBackground            = g_pPunchPosManager->GetFillBkg() ? true : false;
    pp->DrawBorder                = g_pPunchPosManager->GetDrawBorder() ? true : false;
    pp->DrawBorderIfLocked        = g_pPunchPosManager->GetDrawBorderIfLocked() ? true : false;
    pp->InPunchProgressMode       = g_pPunchPosManager->GetInPunchProgressMode() ? true : false;

    // Lock renderer
    g_pPunchPosManager->Lock();
    locked = TRUE;

    PUNCHPOSITION ppInfo;

    // Create co-ordinate converter- this will convert from camera to UI co-ordinates
    CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
      m_UIImageWidth, m_UIImageHeight,
      m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
      m_InterfaceZPL, m_CameraZPL);

    Point pt;


    cElements = g_pPunchPosManager->GetPunchPositionCount();
    for(i = 0; i < cElements; i++) {

      pos = gcnew PunchPosition();
      g_pPunchPosManager->GetPunchPositionInfo(i, &ppInfo);

      pt = (Point)gcnew Point(ppInfo.X, ppInfo.Y);

      pos->ID           = ppInfo.ID;
      pos->Position     = converter->Convert(pt);
      if (ppInfo.exactPosValid) {
          pos->ExactPosition = gcnew DoublePoint(ppInfo.exactX, ppInfo.exactY);
      } else {
          pos->ExactPosition = nullptr;
      }
      pos->Used         = ppInfo.Used ? true : false;
	  pos->Radius       = ppInfo.Radius;
	  pos->Diameter     = ppInfo.diameter;
	  pos->BorderWidth  = ppInfo.BorderWidth;
	  pos->InBlood      = ppInfo.InBlood ? true : false;
      pos->Locked       = ppInfo.Locked ? true : false;

      pos->BorderColor  = Color::FromArgb( GetRValue(ppInfo.BorderColor),
                                           GetGValue(ppInfo.BorderColor),
                                           GetBValue(ppInfo.BorderColor) );

      pos->FillColor    = Color::FromArgb( GetRValue(ppInfo.FillColor),
                                           GetGValue(ppInfo.FillColor),
                                           GetBValue(ppInfo.FillColor) );

      pos->TextColor    = Color::FromArgb( GetRValue(ppInfo.TextColor),
                                           GetGValue(ppInfo.TextColor),
                                           GetBValue(ppInfo.TextColor) );

      pp->Add(pos);



      if(ppInfo.InBlood) {
        count++;
      }


    }

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_PUNCHPOSITIONS_GET_FAILED));
  } finally {
    if(locked) {
      g_pPunchPosManager->Unlock();
    }
  }

  return pp;
  
}

void DSCamera::PunchPositions::set(IPunchPositions^ value) {

  _METHODNAME(_T("PunchPositions::set"));
  ThrowNotInitializedException(TRUE, METHODNAME);
  BYTE locked = FALSE;

  if(value == nullptr) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_PUNCHPOSITIONS_SET_NULL_ARG));
  } else {

    try {

      BYTE cPunchPositionCount = (BYTE)value->Count;
      g_pPunchPosManager->Lock();
      locked = TRUE;

      
      for(BYTE i = 0; i < cPunchPositionCount; i++) {
        IPunchPosition^ pos = (IPunchPosition^)value[i];
        if(pos->Used && pos->Radius == 0) {
          // Too small punch radius
          ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_TOO_SMALL_PUNCH_RADIUS);
        }
      }

      // Collect flags
      DWORD flags = 0;

      value->DrawId               ? flags |= PPM_DRAWID : 0;
      value->DrawIdOnlyInBlood    ? flags |= PPM_DRAWID_ONLY_IN_BLOOD : 0;
      value->FillBackground       ? flags |= PPM_FILLBKG : 0;
      value->DrawBorder           ? flags |= PPM_DRAWBORDER : 0;
      value->DrawBorderIfLocked   ? flags |= PPM_DRAWBORDER_IF_LOCKED : 0;
      value->InPunchProgressMode  ? flags |= PPM_IN_PUNCH_PROGRESS_MODE : 0;


      // Create converter, that will convert from UI to camera co-ordinates
      CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
        m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
        m_UIImageWidth, m_UIImageHeight,
        m_CameraZPL, m_InterfaceZPL);


      g_pPunchPosManager->StartPunchPositionsUpdate(cPunchPositionCount, flags);

      Point pt;

      // InBlood-property can be set only when still image is used and PunchProgressMode is on - camera is
      // used to display the current punch progress
      BYTE allowInBloodSet = m_pRenderManager->GetCameraRenderer()->IsDisplayImageUsed() && value->InPunchProgressMode;

      if(cPunchPositionCount > 0) {

        PUNCHPOSITION info;
        for(BYTE i = 0; i < cPunchPositionCount; i++) {

          PunchPosition^ pos = (PunchPosition^)value[i];
          pt = converter->Convert(pos->Position);

          info.dwTime       = UNKNOWN_TIME;
          info.ID           = pos->ID;
          info.Used         = pos->Used ? TRUE : FALSE;
          info.X            = pt.X;
          info.Y            = pt.Y;
          if (pos->ExactPositionDefined) {
              info.exactPosValid = 1;
              info.exactX = pos->ExactPosition->X;
              info.exactY = pos->ExactPosition->Y;
          } else {
              info.exactPosValid = 0;
          }
          info.Radius       = converter->Scale(pos->Radius); 
          info.diameter     = pos->Diameter;
          info.BorderWidth  = converter->Scale(pos->BorderWidth);
          info.InBlood      = allowInBloodSet ? ( pos->InBlood ? TRUE : FALSE ) : FALSE;
          info.Locked       = pos->Locked ? TRUE : FALSE;

          info.BorderColor  = RGB( pos->BorderColor.R,
                                   pos->BorderColor.G,
                                   pos->BorderColor.B );

          info.FillColor    = RGB( pos->FillColor.R,
                                   pos->FillColor.G,
                                   pos->FillColor.B );

          info.TextColor    = RGB( pos->TextColor.R,
                                   pos->TextColor.G,
                                   pos->TextColor.B );

          g_pPunchPosManager->SetPunchPositionInfo(i, &info);

        }
      }

    } catch (ExceptionInfo* pEI) {
      ConvertToNETException(pEI);
    } catch (...) {
      ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_PUNCHPOSITIONS_SET_FAILED));
    } finally {
      if(locked) {
        g_pPunchPosManager->Unlock();
      }
    }
  }

}

IntPtr DSCamera::PunchPositionCountChangedEventHandle::get() {

  _METHODNAME(_T("PunchPositionCountChangedEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetPunchPositionCountChangedNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_PPCOUNTCHANGEDEVENTHANDLE_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

IntPtr DSCamera::BloodCardDetectedEventHandle::get() {

  _METHODNAME(_T("BloodCardDetectedEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetBloodCardDetectedNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_BLOODCARDDETECTEDEVENTHANDLE_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

IntPtr DSCamera::BloodCardRemovedEventHandle::get() {

  _METHODNAME(_T("BloodCardRemovedEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetBloodCardRemovedNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_BLOODCARDREMOVEDEVENTHANDLE_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

IntPtr DSCamera::BarcodesUpdatedEventHandle::get() {

  _METHODNAME(_T("BarcodesUpdatedEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetBarcodesUpdatedNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_BARCODESUPDATEDEVENTHANDLE_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

IntPtr DSCamera::CameraDriverExceptionEventHandle::get() {

  _METHODNAME(_T("CameraDriverExceptionEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetCameraExceptionOccuredNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_CAMERADRIVEREXCEPTIONEVENTHANDLE_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

IntPtr DSCamera::FramesSkippedNotificationEventHandle::get() {

  _METHODNAME(_T("FramesSkippedNotificationEventHandle::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    IntPtr^ ptr = gcnew IntPtr(g_pPunchPosManager->GetFramesSkippedNotification());
    return (IntPtr)ptr;

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_FRAMESSKIPPEDNOTIFICATIONEVENT_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return IntPtr::Zero;

}

array<String^>^ DSCamera::Barcode::get() {

  _METHODNAME(_T("Barcode::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    PTCHAR barcode = m_pRenderManager->GetCameraRenderer()->GetBarcodeReader()->GetBarcode();

    if(barcode != nullptr) {
      array<String^>^ retVal = gcnew array<String^>(2);
  
      String^ bc = gcnew String(barcode);
      retVal[0] = bc;

      PTCHAR barcodeType = m_pRenderManager->GetCameraRenderer()->GetBarcodeReader()->GetBarcodeType();
      retVal[1] = gcnew String(barcodeType);

      return retVal;
    }

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_BARCODES_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return nullptr;
}



PuncherCameraException^ DSCamera::CameraDriverException::get() {

  _METHODNAME(_T("CameraDriverException::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {

    g_pPunchPosManager->Lock();

    // Get exception info
    ExceptionInfo* pEX = g_pPunchPosManager->GetCameraDriverException();
    if(pEX != NULL) {

      PuncherCameraException^ pce = gcnew PuncherCameraException();

      pce->ClassName      = gcnew String(pEX->GetClass());
      pce->MethodName     = gcnew String(pEX->GetMethod());

      pce->ErrorCode      = pEX->GetErrorCode();
      pce->ErrorCodeExt   = (int)pEX->GetHResult();

      // Remove info...so that new exception can be stored :)
      g_pPunchPosManager->ClearCameraDriverException();
      return pce;

    }

  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_CAMERADRIVEREXCEPTION_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return nullptr;
}

bool DSCamera::ReadBarcodes::get() {

  _METHODNAME(_T("ReadBarcodes::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {
    g_pPunchPosManager->Lock();
    return g_pPunchPosManager->GetReadBarcodesState() ? true : false;
  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_READBARCODES_GET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

  return false;
}

void DSCamera::ReadBarcodes::set(bool value) {

  _METHODNAME(_T("ReadBarcodes::set"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {
    g_pPunchPosManager->Lock();
    g_pPunchPosManager->SetReadBarcodesState(value ? TRUE : FALSE);
  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_READBARCODES_SET_FAILED));
  } finally {
    g_pPunchPosManager->Unlock();
  }

}

bool DSCamera::CardDetected::get() {

  _METHODNAME(_T("CardDetected::get"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  try {
    return m_pRenderManager->GetCameraRenderer()->IsCardDetected() ? true : false;
  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_CARDDETECTED_SET_FAILED));
  }

  return false;

}

bool DSCamera::GrabPicture() {
    _METHODNAME(_T("GrabPicture"));
    ThrowNotInitializedException(TRUE, METHODNAME);

    Bitmap^ picture = nullptr;
    bool retVal = false;
    try {

        PSPOTLIGHTCOLOR pCol  = NULL;
        PSPOTLIGHTCOLOR p     = NULL;
        PBYTE pImage          = NULL;
        
        if (m_pRenderManager->GetCameraRenderer()->CapturedImageExists()) {
            m_pRenderManager->GetCameraRenderer()->ReleaseCapturedImage();
        }

        // Start grabbing
        m_pRenderManager->GetCameraRenderer()->InitiateImageCapturing();

        // Get image captured event
        HANDLE capturedEvent = m_pRenderManager->GetCameraRenderer()->GetImageCapturedEvent();

        // Wait
        // TODO: How long we should wait
        switch(::WaitForSingleObject(capturedEvent, 1000)) {

        case WAIT_OBJECT_0:
            // Image captured
            retVal = true;
            break;

        default:
            // Failed to capture image
            break;
        }

    } catch (ExceptionInfo* pEI) {
        ConvertToNETException(pEI);
    } catch (...) {
        ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_TAKEPICTURE_FAILED));
    }

    return retVal;
}

Bitmap^ DSCamera::TakePicture() {

  _METHODNAME(_T("TakePicture"));
  ThrowNotInitializedException(TRUE, METHODNAME);

  Bitmap^ picture = nullptr;

  try {

    int w;
    int h;
    int usedY;

    PSPOTLIGHTCOLOR pCol  = NULL;
    PSPOTLIGHTCOLOR p     = NULL;
    PBYTE pImage          = NULL;

    if (!(m_pRenderManager->GetCameraRenderer()->CapturedImageExists())) {
        if (!GrabPicture()) {
            // No image existed, and grabbing a new one failed, return null
            return picture;
        }
    }
    // Image captured
    pImage = m_pRenderManager->GetCameraRenderer()->GetCapturedImage();
    if(pImage != NULL) {
        w = m_pRenderManager->GetCameraImageWidth();
        h = m_pRenderManager->GetCameraImageHeight();
        picture = gcnew Bitmap(w, h);

        pCol = (PSPOTLIGHTCOLOR)pImage;;
        usedY = h;

        // Copy data
        for(int y = 0; y < h; y++) {
        usedY--;
        for(int x = 0; x < w; x++) {
            p = &pCol[(y * w) + x];
            picture->SetPixel(x, usedY, Color::FromArgb(p->r, p->g, p->b));
        }
        }
    }

    // Release image cache
    m_pRenderManager->GetCameraRenderer()->ReleaseCapturedImage();


  } catch (ExceptionInfo* pEI) {
    ConvertToNETException(pEI);
  } catch (...) {
    ConvertToNETException(new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_TAKEPICTURE_FAILED));
  }

  return picture;

}

void DSCamera::ValidateSettings(IPuncherCameraSettings^ camSettings) {

  _METHODNAME(_T("ValidateSettings"));
  if(camSettings == nullptr) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_SETTINGS_IS_NULL);
  }

  if(camSettings->InSimulationMode && (camSettings->SimulationFile == nullptr || camSettings->SimulationFile->Length == 0)) {
    // When in simulation mode, we need to have the simulation file
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_SIMULATIONFILE_MISSING);
  }

  if(camSettings->BarcodeReaderLocation == nullptr || camSettings->BarcodeReaderLocation->Length == 0) {
    // Barcode reader location is missing
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_BCREADER_LOCATION_MISSING);
  }

  if( camSettings->BloodColorMin.R > camSettings->BloodColorMax.R ||
      camSettings->BloodColorMin.G > camSettings->BloodColorMax.G ||
      camSettings->BloodColorMin.B > camSettings->BloodColorMax.B ) {
    // Min color greater than max
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_BLOODCOLOR_MIN_GREATER_THEN_MAX);
       
  }

  if(!IsWindow((HWND)camSettings->DrawingSurface.ToInt32())) {
    // Invalid drawing surface
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_DRAWINGSURFACE_IS_INVALID);
  }

  if(camSettings->SearchAreaDetectionColorMinOffset > camSettings->SearchAreaDetectionColorMaxOffset) {
    // Min index is greater than max index
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_EMPTYIMAGEINDEX_MIN_GREATER);
  }

  if(camSettings->UiImageSize.Width <= 0 || camSettings->UiImageSize.Height <= 0) {
    // Invalid image size
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_IMAGESIZE_INVALID);
  }

  if(camSettings->RequiredBloodPercent > BLOODDETECTOR_MAX_DETECTION_PERCENT) {
    // Too big percent value
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_REQBLOOD_IS_TOO_BIG);
  }

  if( camSettings->SearchAreaCenter.X < 0 ||
      camSettings->SearchAreaCenter.Y < 0 ||
      camSettings->SearchAreaCenter.X >= camSettings->UiImageSize.Width ||
      camSettings->SearchAreaCenter.Y >= camSettings->UiImageSize.Height) {

      // Invalid search area
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_SACENTER_IS_INVALID);
  }

  if(camSettings->SearchAreaRadius <= 0) {
    // Search area radius is invalid
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_SARADIUS_IS_INVALID);
  }

  if(camSettings->FramesSkippedNotificationLimit <= camSettings->FrameRenderInterval) {
    // Limit must be grater than the render interval
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_FSNL_MUST_BE_GREATER_TAHN_FRI);
  }

  if(camSettings->HotspotSize.Width <= 0 || camSettings->HotspotSize.Height <= 0) {
    // Invalid size
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_HOTSPOTSIZE_IS_INVALID);
  }

  if(camSettings->HotspotDetectionColorMinOffset > camSettings->HotspotDetectionColorMaxOffset) {
    // Min value is greater then max
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_HSINDEX_MIN_GREATER_THAN_MAX);
  }

  if(camSettings->CalibrationImage != nullptr) {
    if(camSettings->CalibrationImage->Width != camSettings->CameraImageWidth ||
      camSettings->CalibrationImage->Height != camSettings->CameraImageHeight) {
      // Invalid size
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSCAMERA_CALIBRATION_IMAGE_INVALID_SIZE);
    }
  }

  // TODO: lisää koodi joka heittää virheen jos calibration image on eri kokoa kuin kuva

}

void DSCamera::CopySettings(IPuncherCameraSettings^ camSettings, PDSRENDERMANAGERSETTINGS pRMSettings) {

  // Note: camSettings uses UI pixels and pRMSettings uses camera pixels

  m_UIImageWidth = camSettings->UiImageSize.Width;
  m_UIImageHeight = camSettings->UiImageSize.Height;

  Point^ pt;
  CameraCoOrdinateConverter^ converter = gcnew CameraCoOrdinateConverter(
    m_pRenderManager->GetCameraImageWidth(), m_pRenderManager->GetCameraImageHeight(),
    m_UIImageWidth, m_UIImageHeight,
      m_CameraZPL, m_InterfaceZPL);

  pRMSettings->SearchAreaAllowedInvalidPixelPercent = camSettings->SearchAreaAllowedInvalidPixelPercent;
  pRMSettings->HotspotAllowedInvalidPixelPercent = camSettings->HotspotAllowedInvalidPixelPercent;

  pRMSettings->DrawBlood                        = camSettings->DrawBlood ? TRUE : FALSE;
  pRMSettings->BloodColor                       = RGB( camSettings->BloodColor.R,
                                                       camSettings->BloodColor.G,
                                                       camSettings->BloodColor.B );

  pRMSettings->BloodColorMin                    = RGB( camSettings->BloodColorMin.R,
                                                       camSettings->BloodColorMin.G,
                                                       camSettings->BloodColorMin.B );

  pRMSettings->BloodColorMax                    = RGB( camSettings->BloodColorMax.R,
                                                       camSettings->BloodColorMax.G,
                                                       camSettings->BloodColorMax.B );

  pRMSettings->UseSecondBloodDetection          = camSettings->UseSecondBloodDetection ? TRUE : FALSE;
  pRMSettings->BloodColorMin2                   = RGB( camSettings->BloodColorMin2.R,
                                                       camSettings->BloodColorMin2.G,
                                                       camSettings->BloodColorMin2.B );

  pRMSettings->BloodColorMax2                   = RGB( camSettings->BloodColorMax2.R,
                                                       camSettings->BloodColorMax2.G,
                                                       camSettings->BloodColorMax2.B );

  pRMSettings->PunchDetectionAccuracyX          = camSettings->PunchDetectionAccuracyX + 1;
  pRMSettings->PunchDetectionAccuracyY          = camSettings->PunchDetectionAccuracyY + 1;
  pRMSettings->FrameRenderInterval              = camSettings->FrameRenderInterval;
  pRMSettings->FramesSkippedNotificationLimit   = camSettings->FramesSkippedNotificationLimit;

  //pRMSettings->BarcodeReaderType                = DTK;          // camSettings->BarcodeReader
  pRMSettings->BloodDetectionMethod             = DetectBlood;  // camSettings->BloodDetection

  pRMSettings->DrawPunchPositions               = camSettings->DrawPunchPositions ? TRUE : FALSE;
  pRMSettings->DrawSearchAreaBorder             = camSettings->DrawSearchAreaBorder ? TRUE : FALSE;
  pRMSettings->InSimulationMode                 = camSettings->InSimulationMode ? TRUE : FALSE;
  pRMSettings->PunchBorder                      = converter->Scale(camSettings->PunchBorder);
  pRMSettings->SearchAreaBorderWidth            = converter->ScaleCeiling(camSettings->SearchAreaBorderWidth);
  pRMSettings->SearchAreaBorderColor            = RGB( camSettings->SearchAreaBorderColor.R,
                                                       camSettings->SearchAreaBorderColor.G,
                                                       camSettings->SearchAreaBorderColor.B );

  pRMSettings->barcodeReaderSettings.maxBarcodeCount                    = camSettings->BarcodeCount;
  pRMSettings->barcodeReaderSettings.usBarcodeReadingInterval1          = camSettings->BarcodeReadingInterval1;
  pRMSettings->barcodeReaderSettings.usBarcodeReadingInterval2          = camSettings->BarcodeReadingInterval2;
  pRMSettings->barcodeReaderSettings.enableCode128						= camSettings->EnableCode128 ? TRUE : FALSE;
  pRMSettings->barcodeReaderSettings.enableInterleaved2of5				= camSettings->EnableInterleaved2of5 ? TRUE : FALSE;
  pRMSettings->barcodeReaderSettings.enableCode39						= camSettings->EnableCode39 ? TRUE : FALSE;
  pRMSettings->barcodeReaderSettings.enableDataMatrix					= camSettings->EnableDataMatrix ? TRUE : FALSE;

  pRMSettings->barcodeReaderSettings.enableScanRect1 = camSettings->EnableScanRect1 ? 1 : 0;
  pRMSettings->barcodeReaderSettings.scanRect1Left = converter->ScaleX(camSettings->ScanRect1Left);
  pRMSettings->barcodeReaderSettings.scanRect1Top = converter->ScaleY(camSettings->ScanRect1Top);
  pRMSettings->barcodeReaderSettings.scanRect1Right = converter->ScaleX(camSettings->ScanRect1Right);
  pRMSettings->barcodeReaderSettings.scanRect1Bottom = converter->ScaleY(camSettings->ScanRect1Bottom);

  pRMSettings->barcodeReaderSettings.enableScanRect2 = camSettings->EnableScanRect2 ? 1 : 0;
  pRMSettings->barcodeReaderSettings.scanRect2Left = converter->ScaleX(camSettings->ScanRect2Left);
  pRMSettings->barcodeReaderSettings.scanRect2Top = converter->ScaleY(camSettings->ScanRect2Top);
  pRMSettings->barcodeReaderSettings.scanRect2Right = converter->ScaleX(camSettings->ScanRect2Right);
  pRMSettings->barcodeReaderSettings.scanRect2Bottom = converter->ScaleY(camSettings->ScanRect2Bottom);

  pRMSettings->barcodeReaderSettings.enableScanRect3 = camSettings->EnableScanRect3 ? 1 : 0;
  pRMSettings->barcodeReaderSettings.scanRect3Left = converter->ScaleX(camSettings->ScanRect3Left);
  pRMSettings->barcodeReaderSettings.scanRect3Top = converter->ScaleY(camSettings->ScanRect3Top);
  pRMSettings->barcodeReaderSettings.scanRect3Right = converter->ScaleX(camSettings->ScanRect3Right);
  pRMSettings->barcodeReaderSettings.scanRect3Bottom = converter->ScaleY(camSettings->ScanRect3Bottom);

  pRMSettings->barcodeReaderSettings.barcodeRedundancy = camSettings->BarcodeRedundancy;
  pRMSettings->barcodeReaderSettings.minimumBarcodeHeight = converter->ScaleY(camSettings->MinimumBarcodeHeight);

  pRMSettings->SearchAreaColorOffsets.dwMin     = camSettings->SearchAreaDetectionColorMinOffset;
  pRMSettings->SearchAreaColorOffsets.dwMax     = camSettings->SearchAreaDetectionColorMaxOffset;
  
  pRMSettings->DrawingSurface                   = (HWND)camSettings->DrawingSurface.ToInt32();
  pRMSettings->SearchAreaRadius                 = converter->Scale(camSettings->SearchAreaRadius);
  pRMSettings->RequiredBloodPercent             = camSettings->RequiredBloodPercent;



  pt = converter->Convert(camSettings->SearchAreaCenter);
  pRMSettings->SearchAreaCenter.x               = pt->X;
  pRMSettings->SearchAreaCenter.y               = pt->Y;

  USHORT t = pRMSettings->SearchAreaRadius;
  pRMSettings->SearchAreaTopLeft.x              = pt->X - t;;
  pRMSettings->SearchAreaTopLeft.y              = pt->Y - t;;

  pRMSettings->SearchAreaBottomRight.x          = pt->X + t;;
  pRMSettings->SearchAreaBottomRight.y          = pt->Y + t;;

  if(pRMSettings->SearchAreaTopLeft.x < 0)      { pRMSettings->SearchAreaTopLeft.x = 0; }
  if(pRMSettings->SearchAreaTopLeft.y < 0)      { pRMSettings->SearchAreaTopLeft.y = 0; }

  t = (USHORT)m_pRenderManager->GetCameraImageWidth();
  if(pRMSettings->SearchAreaBottomRight.x > t)  { pRMSettings->SearchAreaBottomRight.x = t; }

  t = (USHORT)m_pRenderManager->GetCameraImageHeight();
  if(pRMSettings->SearchAreaBottomRight.y > t)  { pRMSettings->SearchAreaBottomRight.y = t; }




  pRMSettings->UiImageSize.cx                   = camSettings->UiImageSize.Width;
  pRMSettings->UiImageSize.cy                   = camSettings->UiImageSize.Height;

  pRMSettings->SearchAreaDetectionAccuracyX     = converter->ScaleX(camSettings->SearchAreaDetectionAccuracyX + 1);
  pRMSettings->SearchAreaDetectionAccuracyY     = converter->ScaleY(camSettings->SearchAreaDetectionAccuracyY + 1);

  pRMSettings->DrawHotspots                     = camSettings->DrawHotspots ? TRUE : FALSE;
  pRMSettings->HotspotColor                     = RGB( camSettings->HotspotColor.R,
                                                       camSettings->HotspotColor.G,
                                                       camSettings->HotspotColor.B );

  pRMSettings->HotspotCount                     = camSettings->HotspotCount;
  

  pt->Y = camSettings->HotspotOffsetY;
  pt = converter->Convert((Point)pt);
  pRMSettings->HotspotOffsetY                   = pt->Y;

  pRMSettings->HotspotSize.cx                   = converter->ScaleX(camSettings->HotspotSize.Width);
  pRMSettings->HotspotSize.cy                   = converter->ScaleY(camSettings->HotspotSize.Height);
  pRMSettings->HotspotColorOffsets.dwMin        = camSettings->HotspotDetectionColorMinOffset;
  pRMSettings->HotspotColorOffsets.dwMax        = camSettings->HotspotDetectionColorMaxOffset;
  pRMSettings->HotspotDetectionAccuracyX        = converter->ScaleX(camSettings->HotspotDetectionAccuracyX + 1);
  pRMSettings->HotspotDetectionAccuracyY        = converter->ScaleY(camSettings->HotspotDetectionAccuracyY + 1);
  pRMSettings->AllHotspotsMustBeInvalid         = camSettings->AllHotspotsMustBeInvalid ? TRUE : FALSE;


  switch(camSettings->InputImage) {

    case InputImageType::Color:
      pRMSettings->InputImage = ColorImage;
      break;

    case InputImageType::Grayscale:
      pRMSettings->InputImage = GrayscaleImage;
      break;

  }


  // Build barcode reading direction info
  BYTE bcReadDir = 0;
  BYTE tmp = camSettings->BarcodeReadingDirection;
  if(tmp & (BYTE)BarcodeOrientation::LeftToRight) { bcReadDir |= BCR_RD_LEFT_TO_RIGHT; }
  if(tmp & (BYTE)BarcodeOrientation::RightToLeft) { bcReadDir |= BCR_RD_RIGHT_TO_LEFT; }
  if(tmp & (BYTE)BarcodeOrientation::TopToBottom) { bcReadDir |= BCR_RD_TOP_TO_BOTTOM; }
  if(tmp & (BYTE)BarcodeOrientation::BottomToTop) { bcReadDir |= BCR_RD_BOTTOM_TO_TOP; }

  pRMSettings->barcodeReaderSettings.BcOrientation = bcReadDir;
  pRMSettings->barcodeReaderSettings.pszReaderBinaryLocation = StringToPTCHAR(camSettings->BarcodeReaderLocation);


  if(pRMSettings->InSimulationMode) {
    pRMSettings->pszSimulationFile = StringToPTCHAR(camSettings->SimulationFile);
  } else {
    pRMSettings->pszSimulationFile = NULL;
  }

  if(camSettings->CalibrationImage != nullptr) {
    // Calibration image is specified - copy the actual bits
    LONG dummy;
    pRMSettings->CalibrationImage = CreateCameraImage(camSettings->CalibrationImage, dummy);
  } else {
    pRMSettings->CalibrationImage = NULL;
  }

  pRMSettings->FlipHorizontally = camSettings->FlipHorizontally ? TRUE : FALSE;
  pRMSettings->FlipVertically = camSettings->FlipVertically ? TRUE : FALSE;

}

PBYTE DSCamera::CreateCameraImage(Bitmap^ imageToConvert, LONG &rSize) {

  _METHODNAME(_T("CreateCameraImage"));
  PBYTE pImage = NULL;
  BYTE errorOccured = TRUE;

  try {
    
    int w = imageToConvert->Width;
    int h = imageToConvert->Height;

    rSize   = (w * h) * sizeof(SPOTLIGHTCOLOR);
    pImage  = new BYTE[rSize];
    PSPOTLIGHTCOLOR p = (PSPOTLIGHTCOLOR)pImage;

    Color^ col;
    SPOTLIGHTCOLOR slcol;

    for(int x = 0; x < w; x++) {
      for(int y = 0; y < h; y++) {
        col = imageToConvert->GetPixel(x, y);

        slcol.r = col->R;
        slcol.g = col->G;
        slcol.b = col->B;

        // Bottom left corner is the position 0, 0.
        p[((h - 1 - y) * w) + x] = slcol;
      }
    }

    errorOccured = FALSE;

  } catch (...) {
    throw new ExceptionInfo(CLASSNAME, METHODNAME, ERR_DSCAMERA_FAILED_TO_CREATE_CALIBRATION_IMAGE);
  } finally {
    if(errorOccured && pImage != NULL) {
      delete []pImage;
    }
  }

  return pImage;

}

void DSCamera::ConvertToNETException(ExceptionInfo* pException) {

  _METHODNAME(_T("ConvertToNETException"));
  if(pException != NULL) {

    // Convert to PuncherCameraException
    String^ className   = gcnew String(pException->GetClass() != NULL ? pException->GetClass() : STR_ERR_NULL_CLASSNAME);
    String^ methodName  = gcnew String(pException->GetMethod() != NULL ? pException->GetMethod() : STR_ERR_NULL_METHODNAME);

    int errorCode       = pException->GetErrorCode();
    int errorCodeExt    = pException->GetHResult();

    delete pException;
    pException = NULL;

    throw gcnew PuncherCameraException(className, methodName, errorCode, errorCodeExt);

  } else {
    // Ops!
    throw gcnew PuncherCameraException(gcnew String(CLASSNAME), gcnew String(METHODNAME), ERR_EXCEPTIONINFO_IS_NULL, 0);
  }

}

PTCHAR DSCamera::StringToPTCHAR(String^ str) {

  PTCHAR psz = NULL;

  if(str != nullptr) {

    int len = str->Length;
    if(len > 0) {
      psz = new TCHAR[len + 1];
      for(int i = 0; i < len; i++) {
        psz[i] = (TCHAR)str[i];
      }
      psz[len] = '\0';
    }

  }

  return psz;

}

String^ DSCamera::PTCHARToString(const PTCHAR psz) {

  if(psz == NULL) {
    return String::Empty;
  } else {
    return gcnew String(psz);
  }

}

void DSCamera::ThrowNotInitializedException(BYTE throwNet, const PTCHAR pszMethod) {

  if(m_pRenderManager == NULL) {
    if(throwNet) {
      ConvertToNETException(new ExceptionInfo(CLASSNAME, pszMethod, ERR_DSCAMERA_NOT_INITIALIZED));
    } else {
      ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_DSCAMERA_NOT_INITIALIZED);
    }
  }
}

void DSCamera::EnterStandby() {
    m_pRenderManager->GetCameraRenderer()->GetBarcodeReader()->EnterStandby();
}

void DSCamera::LeaveStandby() {
    m_pRenderManager->GetCameraRenderer()->GetBarcodeReader()->LeaveStandby();
}



BYTE DSCamera::BloodDetectionColorMax::get() {
	return m_pRenderManager->GetCameraRenderer()->GetBloodDetector(1)->GetBloodDetectionColorMax();
}

void DSCamera::BloodDetectionColorMax::set(BYTE value) {
	m_pRenderManager->GetCameraRenderer()->GetBloodDetector(1)->SetBloodDetectionColorMax(value);
}
