// --------------------------------------------------------------------------
// DSRenderManager.cpp
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera manager object implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 29.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#include "stdafx.h"
#include "DSRenderManager.h"
#include "ExceptionInfo.h"
#include <strmif.h>
#include <initguid.h>
#include <stdio.h>

#include "SimpleLogger.h"

// --------------------------------------------------------------------------
// Supported cameras
// --------------------------------------------------------------------------

// The original camera
#define ORIGINAL_CAMERA_NAME                L"UI122xLE-C"

// The second camera
#define SECOND_CAMERA_VERSION_NAME          L"UI125xLE-C"

// Laptop or web camera used to debug the scaling
#define DEBUG_CAMERA_NAME                   L"HP HD Camera"
#define DEBUG_CAMERA_WIDTH                  1280
#define DEBUG_CAMERA_HEIGHT                 720
#define DEBUG_CAMERA_WIDTH_MM               85
#define DEBUG_CAMERA_HEIGHT_MM              48

// --------------------------------------------------------------------------

// NAme of our filter/input pin in the DS graph builder
#define SPOTLIGHT_RENDERER_NAME             L"SpotLightVideoRenderer"

// NAme of the camera source filter
#define VIDEO_SOURCE_NAME                   L"SpotlightVideoFeed"

// The max attempt we wait the DS event listener to close
#define MAX_DSELISTENER_CLOSE_ATTEMPT       10

// The time (milliseconds) (x MAX_DSELISTENER_CLOSE_ATTEMPT) that we wait DS event listener to close
#define DSELISTENER_CLOSE_WAIT_INTERVALL    250

// Value which is returned from the DS event listener thread when exception is caught, and the
// GetLastError-method returns 0
#define DSELISTENER_EXCEPTION_OCCURED       98765

// Time in milliseconds that we wait events DS event listener thread
#define DSELISTENER_WAIT_INTERVAL           50

// Class name, used as a param to exception methods
_CLASSNAME(_T("DSRenderManager"));


// --------------------------------------------------------------------------

// Flag used to close the DS graph event listener thread
BYTE bClosing = FALSE;

// Thread proc for DS graph event listener
DWORD DSGraphEventListener(LPVOID lpParameter) {

  _METHODNAME(_T("DSGraphEventListener"));

  DWORD result = 0;
  try {

    // Get the event listener
    IMediaEvent* pEvents = (IMediaEvent*)lpParameter;

    long eCode;
    LONG_PTR param1;
    LONG_PTR param2;

    while(!bClosing) {

      while(SUCCEEDED(pEvents->GetEvent(&eCode, &param1, &param2, 0))) {

        pEvents->FreeEventParams(eCode, param1, param2);
        if( eCode == EC_DEVICE_LOST && param2 == 0) {
          // Camera was disconnected
          g_pPunchPosManager->SetCameraDriverException(new ExceptionInfo(
            _T(""), METHODNAME, ERR_DSRENDERMANAGER_DEVICE_LOST));

          // Close
          bClosing = TRUE;
          break;
        }

      }

      if(!bClosing) {
        ::Sleep(DSELISTENER_WAIT_INTERVAL);
      }

    }

  } catch (...) {

    result = ::GetLastError();
    if(result == 0) {
      result = DSELISTENER_EXCEPTION_OCCURED;
    }

    g_pPunchPosManager->SetCameraDriverException(new ExceptionInfo(
      _T(""), METHODNAME, ERR_DSRENDERMANAGER_DSELISTENER_EXCEPTION));

  }

  return result;

}



DSRenderManager::DSRenderManager() {

	m_bInitialized          = FALSE;

	m_bInSimulationMode     = FALSE;
	m_pszSimulationFile     = NULL;
	m_cameraType            = NULL;

	m_pRenderer             = NULL;

	m_pGraph                = NULL;
	m_pPlayer               = NULL;
	m_pGraphEvents          = NULL;

	m_hDSMediaEventListener = NULL;
	m_pCameraFilter         = NULL;

	m_cameraImageWidth      = 0;
	m_cameraImageHeight     = 0;
}

DSRenderManager::~DSRenderManager() {

	if (m_bInitialized) {
		Release();
	}
}

void DSRenderManager::InitializeCamera() {

	_METHODNAME(_T("InitializeCamera"));
	ThrowAlreadyInitializedException(METHODNAME);

	// Get camera filter. First valid camera is returned
	m_pCameraFilter = FindCamera();
	if (m_pCameraFilter == NULL) {
		ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAMERAFILTER_IS_NULL);
	}
}

void DSRenderManager::Initialize(PDSRENDERMANAGERSETTINGS pSettings) {

    _METHODNAME(_T("Initialize"));
    LOG_INFO(CLASSNAME, METHODNAME, _T("Initialize method started"));
    ThrowAlreadyInitializedException(METHODNAME);

    CAMERARENDERERSETTINGS crs;
    crs.pBarcodeReader = NULL;
    crs.pPunchDetector = NULL;
    crs.pBloodDetector = NULL;

    try {

        GetRendererSettings(pSettings, &crs);

        crs.UiImageSize = pSettings->UiImageSize;

        crs.pBarcodeReader = CreateBarcodeReader(pSettings);
        if (crs.pBarcodeReader == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create barcode reader - returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED);
        }

        crs.pPunchDetector = CreatePunchDetector(pSettings);
        if (crs.pPunchDetector == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create punch detector - returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEPUNCHDETECTOR_FAILED);
        }

        crs.pBloodDetector = CreateBloodDetector(FALSE, pSettings);
        if (crs.pBloodDetector == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create first blood detector - returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBLOODDETECTOR_FAILED);
        }

        crs.pBloodDetector2 = CreateBloodDetector(TRUE, pSettings);
        if (crs.pBloodDetector2 == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create second blood detector - returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBLOODDETECTOR_FAILED);
        }

        LOG_INFO(CLASSNAME, METHODNAME, _T("Setting camera image dimensions"));
        crs.CameraImageWidth = m_cameraImageWidth;
        crs.CameraImageHeight = m_cameraImageHeight;

        LOG_INFO(CLASSNAME, METHODNAME, _T("Configuring simulation mode settings"));
        m_bInSimulationMode = pSettings->InSimulationMode;
        ReleaseString(m_pszSimulationFile);
        CopyString(pSettings->pszSimulationFile, m_pszSimulationFile);

        if (m_bInSimulationMode) {
            LOG_INFO(CLASSNAME, METHODNAME, _T("Simulation mode enabled"));
        }
        else {
            LOG_INFO(CLASSNAME, METHODNAME, _T("Live camera mode enabled"));
        }

        // Create and initialize the actual image renderer
        HRESULT hr = S_OK;
        m_pRenderer = new DSCameraRenderer(0, &hr);
        if (m_pRenderer == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create DSCameraRenderer - new returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_INITIALIZE_FAILED);
        }
        if (!SUCCEEDED(hr)) {
            LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("DSCameraRenderer constructor failed"), hr);
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_INITIALIZE_FAILED, hr);
        }

        m_pRenderer->InitializeRenderer(&crs);

        // Connect the image renderer to DirectShow runtime
        InitializeDSConnection();

        m_bInitialized = TRUE;
        LOG_INFO(CLASSNAME, METHODNAME, _T("DSRenderManager initialization completed successfully"));

    }
    catch (ExceptionInfo* pEX) {
        LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("ExceptionInfo caught during initialization"), pEX->GetErrorCode());
        throw pEX;
    }
    catch (...) {
        LOG_ERROR(CLASSNAME, METHODNAME, _T("Unknown exception caught during initialization"));
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_INITIALIZE_FAILED);
    }
    finally {

        if (!m_bInitialized) {
            LOG_WARNING(CLASSNAME, METHODNAME, _T("Initialization failed, cleaning up resources"));

            if (crs.pBarcodeReader != NULL) {
                try {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Cleaning up barcode reader"));
                    crs.pBarcodeReader->Release();
                    delete crs.pBarcodeReader;
                }
                catch (...) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception during barcode reader cleanup"));
                }
            }

            if (crs.pPunchDetector != NULL) {
                try {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Cleaning up punch detector"));
                    crs.pPunchDetector->Release();
                    delete crs.pPunchDetector;
                }
                catch (...) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception during punch detector cleanup"));
                }
            }

            if (crs.pBloodDetector != NULL) {
                try {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Cleaning up first blood detector"));
                    crs.pBloodDetector->Release();
                    delete crs.pBloodDetector;
                }
                catch (...) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception during first blood detector cleanup"));
                }
            }

            if (crs.pBloodDetector2 != NULL) {
                try {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Cleaning up second blood detector"));
                    crs.pBloodDetector2->Release();
                    delete crs.pBloodDetector2;
                }
                catch (...) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception during second blood detector cleanup"));
                }
            }

            LOG_INFO(CLASSNAME, METHODNAME, _T("Resource cleanup completed"));
        }

    }

}

void DSRenderManager::Release() {

  _METHODNAME(_T("Release"));

  try {
    
    try {
      // Stop playback
      Stop();
    } catch (...) {
    }


    CloseDSEventListenerThread();

    // Release objects.
    SafeRelease(&m_pGraphEvents);
    SafeRelease(&m_pPlayer);
    SafeRelease(&m_pGraph);

    // We don´t need to delete m_pRenderer: its released by the
    // graph builder when its release is called

    CoUninitialize();
    m_bInitialized = FALSE;

  } catch (ExceptionInfo* pEX) {
    throw pEX;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_RELEASE_FAILED);
  }

}

void DSRenderManager::Start() {

  _METHODNAME(_T("Start"));
  ThrowNotInitializedException(METHODNAME);

  HRESULT hr = S_OK;
  try {

    // TODO: täällEpitää ehkEresetoida skip time
    hr = m_pPlayer->Run();
    m_pRenderer->RestartImaging();

  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_PLAYER_RUN);
  }

  if(!SUCCEEDED(hr)) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_PLAYER_RUN_ERROR, hr);
  }

}

void DSRenderManager::Stop() {

  _METHODNAME(_T("Stop"));
  ThrowNotInitializedException(METHODNAME);

  HRESULT hr = S_OK;
  try {
    hr = m_pPlayer->Stop();
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_PLAYER_STOP);
  }

  if(!SUCCEEDED(hr)) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_PLAYER_STOP_ERROR, hr);
  }

}

void DSRenderManager::GetCurrentSettings(PDSRENDERMANAGERSETTINGS pSettings) {

  _METHODNAME(_T("GetCurrentSettings"));
  ThrowNotInitializedException(METHODNAME);


  CAMERARENDERERSETTINGS crs;

  BARCODEREADERSETTINGS brs;
  brs.pszReaderBinaryLocation = NULL;

  BLOODDETECTORSETTINGS bds;
  PUNCHDETECTORSETTINGS pds;

  try {

    m_pRenderer->GetCurrentSettings(&crs);

    pSettings->SearchAreaAllowedInvalidPixelPercent = crs.SearchAreaAllowedInvalidPixelPercent;
    pSettings->HotspotAllowedInvalidPixelPercent = crs.HotspotAllowedInvalidPixelPercent;

    pSettings->FlipHorizontally                 = crs.FlipHorizontally;
    pSettings->FlipVertically                   = crs.FlipVertically;
    pSettings->CalibrationImage                 = crs.CalibrationImage;
    pSettings->DrawPunchPositions               = crs.DrawPunchPositions;
    pSettings->DrawSearchAreaBorder             = crs.DrawSearchAreaBorder;
    pSettings->DrawingSurface                   = crs.DrawingSurface;
    pSettings->SearchAreaColorOffsets           = crs.SearchAreaColorOffsets;
    pSettings->SearchAreaBorderColor            = crs.SearchAreaBorderColor;
    pSettings->SearchAreaBorderWidth            = crs.SearchAreaBorderWidth;
    pSettings->PunchDetectionAccuracyX          = crs.PunchDetectionAccuracyX;
    pSettings->PunchDetectionAccuracyY          = crs.PunchDetectionAccuracyY;
    pSettings->FrameRenderInterval              = crs.FrameRenderInterval;
    pSettings->FramesSkippedNotificationLimit   = crs.FramesSkippedNotificationLimit;
    pSettings->BloodColor                       = crs.BloodColor;
    pSettings->DrawBlood                        = crs.DrawBlood;
    pSettings->SearchAreaDetectionAccuracyX     = crs.SearchAreaDetectionAccuracyX;
    pSettings->SearchAreaDetectionAccuracyY     = crs.SearchAreaDetectionAccuracyY;
    pSettings->DrawHotspots                     = crs.DrawHotspots;
    pSettings->HotspotColor                     = crs.HotspotColor;
    pSettings->HotspotCount                     = crs.HotspotCount;
    pSettings->HotspotOffsetY                   = crs.HotspotOffsetY;
    pSettings->HotspotSize                      = crs.HotspotSize;
    pSettings->HotspotColorOffsets              = crs.HotspotColorOffsets;
    pSettings->HotspotDetectionAccuracyX        = crs.HotspotDetectionAccuracyX;
    pSettings->HotspotDetectionAccuracyY        = crs.HotspotDetectionAccuracyY;
    pSettings->AllHotspotsMustBeInvalid         = crs.AllHotspotsMustBeInvalid;
    pSettings->UseSecondBloodDetection          = crs.UseSecondBloodDetection;


    crs.pBarcodeReader->GetCurrentSettings(&brs);

    pSettings->barcodeReaderSettings.BcOrientation						= brs.BcOrientation;
    pSettings->barcodeReaderSettings.maxBarcodeCount                    = brs.maxBarcodeCount;
    pSettings->barcodeReaderSettings.usBarcodeReadingInterval1          = brs.usBarcodeReadingInterval1;
    pSettings->barcodeReaderSettings.usBarcodeReadingInterval2          = brs.usBarcodeReadingInterval2;
    CopyString(brs.pszReaderBinaryLocation, pSettings->barcodeReaderSettings.pszReaderBinaryLocation);
	pSettings->barcodeReaderSettings.enableCode128						= brs.enableCode128;
	pSettings->barcodeReaderSettings.enableInterleaved2of5				= brs.enableInterleaved2of5;
	pSettings->barcodeReaderSettings.enableCode39						= brs.enableCode39;
	pSettings->barcodeReaderSettings.enableDataMatrix					= brs.enableDataMatrix;


    crs.pBloodDetector->GetCurrentSettings(&bds);

    pSettings->BloodDetectionMethod             = bds.bdmDetectionMethod;
    pSettings->RequiredBloodPercent             = bds.bloodDetectionPercent;

    pSettings->BloodColorMin                    = bds.BloodColorMin;
    pSettings->BloodColorMax                    = bds.BloodColorMax;

    pSettings->InputImage                       = bds.ImageType;
    pSettings->SearchAreaBottomRight            = bds.SearchAreaBottomRight;
    pSettings->SearchAreaCenter                 = bds.SearchAreaCenter;
    pSettings->SearchAreaTopLeft                = bds.SearchAreaTopLeft;
    pSettings->SearchAreaRadius                 = bds.SearchAreaRadius;
    
    if(crs.pBloodDetector2 != NULL) {
      crs.pBloodDetector2->GetCurrentSettings(&bds);

      pSettings->BloodColorMin2 = bds.BloodColorMin;
      pSettings->BloodColorMax2 = bds.BloodColorMax;
    } else {
      pSettings->BloodColorMin2 = RGB(0, 0, 0);
      pSettings->BloodColorMax2 = RGB(0, 0, 0);
    }


    crs.pPunchDetector->GetCurrentSettings(&pds);

    pSettings->PunchBorder = pds.PunchBorder;

  } catch (ExceptionInfo* pEX) {
    throw pEX;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_GETSETTINGS_FAILED);
  }

}

void DSRenderManager::UpdateSettings(PDSRENDERMANAGERSETTINGS pSettings) {

	// Note: camSettings uses UI pixels and pRMSettings uses camera pixels

  _METHODNAME(_T("UpdateSettings"));
  ThrowNotInitializedException(METHODNAME);

  try {


    m_pRenderer->GetBarcodeReader()->UpdateSettings(&(pSettings->barcodeReaderSettings));


    BLOODDETECTORSETTINGS bds;
    bds.bdmDetectionMethod            = pSettings->BloodDetectionMethod;
    bds.bloodDetectionPercent         = pSettings->RequiredBloodPercent;
    bds.BloodColorMin                 = pSettings->BloodColorMin;
    bds.BloodColorMax                 = pSettings->BloodColorMax;
    bds.ImageType                     = pSettings->InputImage;
    bds.SearchAreaBottomRight         = pSettings->SearchAreaBottomRight;
    bds.SearchAreaCenter              = pSettings->SearchAreaCenter;
    bds.SearchAreaTopLeft             = pSettings->SearchAreaTopLeft;
    bds.SearchAreaRadius              = pSettings->SearchAreaRadius;
	bds.ImageWidth                    = GetCameraImageWidth();

    m_pRenderer->GetBloodDetector(TRUE)->UpdateSettings(&bds);


    bds.BloodColorMin                 = pSettings->BloodColorMin2;
    bds.BloodColorMax                 = pSettings->BloodColorMax2;

    m_pRenderer->GetBloodDetector(FALSE)->UpdateSettings(&bds);

    
    PUNCHDETECTORSETTINGS pds;
    pds.PunchBorder                   = pSettings->PunchBorder;

    m_pRenderer->GetPunchDetector()->UpdateSettings(&pds);



    CAMERARENDERERSETTINGS crs;
    GetRendererSettings(pSettings, &crs);

    crs.pBarcodeReader = NULL;
    crs.pBloodDetector = NULL;
    crs.pPunchDetector = NULL;

    m_pRenderer->UpdateSettings(&crs);

  } catch (ExceptionInfo* pEX) {
    throw pEX;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_UPDATESETTINGS_FAILED);
  }

}

BloodDetector* DSRenderManager::CreateBloodDetector(BYTE useSecondBloodColors, PDSRENDERMANAGERSETTINGS pSettings) {

  _METHODNAME(_T("CreateBloodDetector"));

  BloodDetector* pBD = NULL;
  BYTE errorOccured = TRUE;

  try {

    BLOODDETECTORSETTINGS bds;

    bds.bdmDetectionMethod            = DetectBlood;  // camSettings->BloodDetection

    if(!useSecondBloodColors) {
      bds.BloodColorMin               = pSettings->BloodColorMin;
      bds.BloodColorMax               = pSettings->BloodColorMax;
    } else {
      bds.BloodColorMin               = pSettings->BloodColorMin2;
      bds.BloodColorMax               = pSettings->BloodColorMax2;
    }

    bds.ImageType                     = pSettings->InputImage;
    bds.SearchAreaCenter              = pSettings->SearchAreaCenter;
    bds.SearchAreaTopLeft             = pSettings->SearchAreaTopLeft;
    bds.SearchAreaBottomRight         = pSettings->SearchAreaBottomRight;
    bds.SearchAreaRadius              = pSettings->SearchAreaRadius;
	bds.ImageWidth                    = (USHORT)GetCameraImageWidth();
	bds.bloodDetectionPercent         = pSettings->RequiredBloodPercent;

    pBD = new BloodDetector();
    pBD->Initialize(&bds);

    errorOccured = FALSE;
    return pBD;

  } catch (ExceptionInfo* pEX) {
    throw pEX;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBLOODDETECTOR_FAILED);
  } finally {

    if(errorOccured) {
      try {
        pBD->Release();
        delete pBD;
      } catch (...) {
      }
    }

  }

  return NULL;

}


PunchDetector* DSRenderManager::CreatePunchDetector(PDSRENDERMANAGERSETTINGS pSettings) {

  _METHODNAME(_T("CreatePunchDetector"));

  PunchDetector* pPD = NULL;
  BYTE errorOccured = TRUE;

  try {

    PUNCHDETECTORSETTINGS pds;
    pds.PunchBorder             = pSettings->PunchBorder;

    pds.ptSearchAreaCenter = pSettings->SearchAreaCenter;
    pds.searchAreaRadius = pSettings->SearchAreaRadius;

    pPD = new PunchDetector();
    pPD->Initialize(&pds);

    errorOccured = FALSE;
    return pPD;

  } catch (ExceptionInfo* pEX) {
    throw pEX;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEPUNCHDETECTOR_FAILED);
  } finally {
    if(errorOccured) {
      try {
        pPD->Release();
        delete pPD;
      } catch (...) {
      }
    }
  }

  return NULL;

}

BarcodeReader* DSRenderManager::CreateBarcodeReader(PDSRENDERMANAGERSETTINGS pSettings) {

    _METHODNAME(_T("CreateBarcodeReader"));
    LOG_INFO(CLASSNAME, METHODNAME, _T("CreateBarcodeReader method started"));

    BarcodeReader* pBCR = NULL;
    BYTE errorOccured = TRUE;

    try {
        LOG_INFO(CLASSNAME, METHODNAME, _T("Validating barcode reader settings"));
        if (pSettings == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Settings parameter is NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED);
        }

        if (pSettings->barcodeReaderSettings.pszReaderBinaryLocation == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Barcode reader binary location is NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED);
        }

        LOG_INFO(CLASSNAME, METHODNAME, _T("Settings validated successfully"));

        LOG_INFO(CLASSNAME, METHODNAME, _T("Creating BarcodeReader instance"));
        pBCR = new BarcodeReader();
        if (pBCR == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create BarcodeReader - new returned NULL"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED);
        }
        LOG_INFO(CLASSNAME, METHODNAME, _T("BarcodeReader instance created successfully"));

        LOG_INFO(CLASSNAME, METHODNAME, _T("Initializing BarcodeReader"));
        pBCR->Initialize(&(pSettings->barcodeReaderSettings));
        LOG_INFO(CLASSNAME, METHODNAME, _T("BarcodeReader initialized successfully"));

        errorOccured = FALSE;
        LOG_INFO(CLASSNAME, METHODNAME, _T("CreateBarcodeReader completed successfully"));
        return pBCR;

    }
    catch (ExceptionInfo* pEX) {
        LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("ExceptionInfo caught in CreateBarcodeReader"), pEX->GetErrorCode());
        throw pEX;
    }
    catch (...) {
        LOG_ERROR(CLASSNAME, METHODNAME, _T("Unknown exception caught in CreateBarcodeReader"));
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CREATEBARCODEREADER_FAILED);
    }
    finally {

        if (errorOccured) {
            LOG_WARNING(CLASSNAME, METHODNAME, _T("Error occurred, cleaning up BarcodeReader"));
            if (pBCR != NULL) {
                try {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Releasing BarcodeReader"));
                    pBCR->Release();
                    delete pBCR;
                    LOG_INFO(CLASSNAME, METHODNAME, _T("BarcodeReader cleanup completed"));
                }
                catch (...) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception during BarcodeReader cleanup"));
                }
            }
        }

    }

    return NULL;

}

// Helper to print GUIDs
void PrintGUID(const GUID& guid, const char* label) {
	char buf[128];
	sprintf(buf, "%s: {%08lX-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X}\n",
		label,
		guid.Data1, guid.Data2, guid.Data3,
		guid.Data4[0], guid.Data4[1], guid.Data4[2], guid.Data4[3],
		guid.Data4[4], guid.Data4[5], guid.Data4[6], guid.Data4[7]);
	OutputDebugStringA(buf);
}

// Helper to log all media types for a pin
void LogPinMediaTypes(IPin* pPin, const char* pinLabel) {
    if (!pPin) {
        char buf[128];
        sprintf(buf, "%s: Pin is NULL\n", pinLabel);
        OutputDebugStringA(buf);
        return;
    }

    IEnumMediaTypes* pEnum = nullptr;
    HRESULT hr = pPin->EnumMediaTypes(&pEnum);
    if (!SUCCEEDED(hr)) {
        char buf[128];
        sprintf(buf, "%s: Failed to enumerate media types (HRESULT: 0x%08X)\n", pinLabel, hr);
        OutputDebugStringA(buf);
        return;
    }

    AM_MEDIA_TYPE* pmt = nullptr;
    int idx = 0;
    while (pEnum->Next(1, &pmt, nullptr) == S_OK) {
        char label[64];
        sprintf(label, "%s media type %d major", pinLabel, idx);
        PrintGUID(pmt->majortype, label);
        sprintf(label, "%s media type %d subtype", pinLabel, idx);
        PrintGUID(pmt->subtype, label);

        if (pmt->cbFormat != 0) {
            CoTaskMemFree((PVOID)pmt->pbFormat);
        }
        if (pmt->pUnk != nullptr) {
            pmt->pUnk->Release();
        }
        CoTaskMemFree(pmt);
        ++idx;
    }

    if (idx == 0) {
        char buf[128];
        sprintf(buf, "%s: No media types found\n", pinLabel);
        OutputDebugStringA(buf);
    }
    else {
        char buf[128];
        sprintf(buf, "%s: Found %d media types\n", pinLabel, idx);
        OutputDebugStringA(buf);
    }

    pEnum->Release();
}

void DSRenderManager::InitializeDSConnection() {

  _METHODNAME(_T("InitializeDSConnection"));

  LOG_INFO(CLASSNAME, METHODNAME, _T("InitializeDSConnection method started"));

  IPin* pRendererInputPin                 = NULL;
  IPin* pCameraOutputPin                  = NULL;
  ICaptureGraphBuilder2* pCaptureBuilder  = NULL;

  m_pGraph  = NULL;
  m_pPlayer = NULL;


  try {

    // Init COM
    HRESULT hr = CoInitialize(0);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("COM initialization failed"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_COM_INIT_FAILED, hr);
    }

    // Get camera output connection
	hr = FindUnconnectedPin(m_pCameraFilter, PINDIR_OUTPUT, &pCameraOutputPin);
	if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to find camera output pin"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CANT_FIND_OUTPUT_PIN, hr);
    }

    if(pCameraOutputPin == NULL) {
      LOG_ERROR(CLASSNAME, METHODNAME, _T("Camera output pin is NULL"));
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CANTOUTPUTPIN_IS_NULL);
    }

	// Find our own input pin
	hr = FindUnconnectedPin(m_pRenderer, PINDIR_INPUT, &pRendererInputPin);
	if(!SUCCEEDED(hr)) {
		ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CANT_FIND_INPUTPIN, hr);
	}

	if(pRendererInputPin == NULL) {
		ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_INPUTPIN_IS_NULL);
	}




	// Create video capture graph builder. In order for Edmund camera to operate succesfully, we must use this
	// to do some special initialization/connection calls between output and input pins
    hr = CoCreateInstance(CLSID_CaptureGraphBuilder2, NULL, CLSCTX_INPROC, IID_ICaptureGraphBuilder2, (void**)&pCaptureBuilder);
    if(!SUCCEEDED(hr)) {
	  // TODO: Lisää kunnon koodi
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to create capture graph builder"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAPTUREBUILDER_CREATION_ERROR);
    }

    if(pCaptureBuilder == NULL) {
      LOG_ERROR(CLASSNAME, METHODNAME, _T("Capture graph builder is NULL"));
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAPTUREBUILDER_IS_NULL);
    }

	// Create DS graph builder. This is used to hold and manage the connection between camera (output pin)
	// and our code - the renderer (input pin)
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC, IID_IGraphBuilder, (void**)&m_pGraph);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to create DirectShow graph builder"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_GRAPHBUILDER_CREATION_ERROR, hr);
    }

    if(m_pGraph == NULL) {
      LOG_ERROR(CLASSNAME, METHODNAME, _T("DirectShow graph builder is NULL"));
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_GRAPHBUILDER_IS_NULL);
    }

    // Specify filter graph
    hr = pCaptureBuilder->SetFiltergraph(m_pGraph);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to set filter graph"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAPTUREBUILDER_SETFILTERGRAP);
    }

    // Create player for controlling the data flow in the filter graph
    hr = m_pGraph->QueryInterface(IID_IMediaControl, (void**)&m_pPlayer);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to create media control interface"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_MEDIACONTROL_CREATION_ERROR, hr);
    }

    if(m_pPlayer == NULL) {
      LOG_ERROR(CLASSNAME, METHODNAME, _T("Media control interface is NULL"));
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_MEDIACONTROL_IS_NULL);
    }

    // Add camera (output pin) to filter graph
	hr = m_pGraph->AddFilter(m_pCameraFilter, VIDEO_SOURCE_NAME);
	if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to add camera filter to graph"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_FAILED_TO_ADD_CAMERA_FILTER, hr);
    }

    // Add our input pin (our hook) to graph
    hr = m_pGraph->AddFilter(m_pRenderer, SPOTLIGHT_RENDERER_NAME);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to add custom renderer to graph"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_FAILED_TO_ADD_SPR_TO_CHAIN, hr);
    }

    // Connect pins
	hr = pCaptureBuilder->RenderStream(&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video, m_pCameraFilter, NULL, m_pRenderer);
	if(!SUCCEEDED(hr)) {
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_FAILED_TO_CONNECT_PINS, hr);
    }

    // Create graph event listener - currently used only to detect camera disconnection
    hr = m_pGraph->QueryInterface(IID_IMediaEvent, (void**)&m_pGraphEvents);
    if(!SUCCEEDED(hr)) {
      LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Failed to create graph event listener"), hr);
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_MEDIAEVENTS_CREATION_ERROR, hr);
    }

    if(m_pGraphEvents == NULL) {
      LOG_ERROR(CLASSNAME, METHODNAME, _T("Graph event listener is NULL"));
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_MEDIAEVENTS_IS_NULL);
    }

    // Create and start DS event listener
    CreateDSEventListenerThread();

  } catch (ExceptionInfo* pEI) {
    LOG_ERROR(CLASSNAME, METHODNAME, _T("ExceptionInfo caught during DirectShow connection initialization"));
    // Forward
    throw pEI;
  } catch (...) {
    LOG_ERROR(CLASSNAME, METHODNAME, _T("Unknown exception caught during DirectShow connection initialization"));
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_DSCONNECTION_INIT_FAILED);
  } finally {

    SafeRelease(&pRendererInputPin);
    SafeRelease(&pCameraOutputPin);
    SafeRelease(&pCaptureBuilder);

  }

}

HRESULT DSRenderManager::IsPinConnected(IPin *pPin, BYTE *pResult) {
	IPin *pTmp  = NULL;
	HRESULT hr  = pPin->ConnectedTo(&pTmp);

	if(SUCCEEDED(hr)) {
		*pResult = TRUE;
	} else if (hr == VFW_E_NOT_CONNECTED) {
		// The pin is not connected. This is not an error for our purposes.
		*pResult = FALSE;
		hr = S_OK;
	}

	SafeRelease(&pTmp);
	return hr;
}

HRESULT DSRenderManager::IsPinDirection(IPin *pPin, PIN_DIRECTION dir, BYTE *pResult) {

	// Query whether a pin has a specified direction (input / output)
	PIN_DIRECTION pinDir;
	HRESULT hr = pPin->QueryDirection(&pinDir);
	if(SUCCEEDED(hr)) {
		*pResult = (pinDir == dir);
	}

	return hr;

}

HRESULT DSRenderManager::MatchPin(IPin *pPin, PIN_DIRECTION direction, BYTE bShouldBeConnected, BYTE *pResult) {

	// Match a pin by pin direction and connection state.
	BYTE bMatch         = FALSE;
	BYTE bIsConnected   = FALSE;

	HRESULT hr = IsPinConnected(pPin, &bIsConnected);
	if(SUCCEEDED(hr)) {
		if(bIsConnected == bShouldBeConnected) {
			hr = IsPinDirection(pPin, direction, &bMatch);

			if(SUCCEEDED(hr)) {
				*pResult = bMatch;
			}
		}
	}

	return hr;

}

HRESULT DSRenderManager::FindUnconnectedPin(IBaseFilter *pFilter, PIN_DIRECTION PinDir, IPin **ppPin) {
	// Return the first unconnected input pin or output pin.
	IEnumPins *pEnum    = NULL;
	IPin *pPin          = NULL;
	BYTE bFound         = FALSE;

	// Get pin enumerator
	HRESULT hr = pFilter->EnumPins(&pEnum);
	if(SUCCEEDED(hr)) {

		while(S_OK == pEnum->Next(1, &pPin, NULL)) {

			hr = MatchPin(pPin, PinDir, FALSE, &bFound);
			if(FAILED(hr)) {
				break;
			}

			if(bFound) {
				*ppPin = pPin;
				(*ppPin)->AddRef();
				break;
			}

			SafeRelease(&pPin);
		}

		if(!bFound && !FAILED(hr)) {
			// Cant find required pin
			hr = VFW_E_NOT_FOUND;
		}
	}

	SafeRelease(&pPin);
	SafeRelease(&pEnum);

	return hr;

}

void DSRenderManager::ThrowNotInitializedException(const PTCHAR pszMethod) {
	if(!m_bInitialized) {
		ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_DSRENDERMANAGER_NOT_INITIALIZED);
	}
}

void DSRenderManager::ThrowAlreadyInitializedException(const PTCHAR pszMethod) {
	if(m_bInitialized) {
		ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_DSRENDERMANAGER_ALREADY_INITIALIZED);
	}
}

void DSRenderManager::GetRendererSettings(const PDSRENDERMANAGERSETTINGS pSettings, PCAMERARENDERERSETTINGS pCRSettings) {

	pCRSettings->FlipHorizontally                 = pSettings->FlipHorizontally;
	pCRSettings->FlipVertically                   = pSettings->FlipVertically;
	pCRSettings->CalibrationImage                 = pSettings->CalibrationImage;

	pCRSettings->SearchAreaAllowedInvalidPixelPercent = pSettings->SearchAreaAllowedInvalidPixelPercent;
	pCRSettings->HotspotAllowedInvalidPixelPercent = pSettings->HotspotAllowedInvalidPixelPercent;

	pCRSettings->ImageType                        = pSettings->InputImage;
	pCRSettings->SearchAreaDetectionAccuracyX     = pSettings->SearchAreaDetectionAccuracyX;
	pCRSettings->SearchAreaDetectionAccuracyY     = pSettings->SearchAreaDetectionAccuracyY;

	pCRSettings->DrawingSurface                   = pSettings->DrawingSurface;
	pCRSettings->SearchAreaColorOffsets           = pSettings->SearchAreaColorOffsets;
	pCRSettings->DrawPunchPositions               = pSettings->DrawPunchPositions;
	pCRSettings->DrawSearchAreaBorder             = pSettings->DrawSearchAreaBorder;
	pCRSettings->SearchAreaBorderColor            = pSettings->SearchAreaBorderColor;
	pCRSettings->SearchAreaBorderWidth            = pSettings->SearchAreaBorderWidth;
	pCRSettings->PunchDetectionAccuracyX          = pSettings->PunchDetectionAccuracyX;
	pCRSettings->PunchDetectionAccuracyY          = pSettings->PunchDetectionAccuracyY;
	pCRSettings->FrameRenderInterval              = pSettings->FrameRenderInterval;
	pCRSettings->FramesSkippedNotificationLimit   = pSettings->FramesSkippedNotificationLimit;
	pCRSettings->DrawBlood                        = pSettings->DrawBlood;
	pCRSettings->BloodColor                       = pSettings->BloodColor;

	pCRSettings->DrawHotspots                     = pSettings->DrawHotspots;
	pCRSettings->HotspotColor                     = pSettings->HotspotColor;
	pCRSettings->HotspotCount                     = pSettings->HotspotCount;
	pCRSettings->HotspotOffsetY                   = pSettings->HotspotOffsetY;
	pCRSettings->HotspotSize                      = pSettings->HotspotSize;
	pCRSettings->HotspotColorOffsets              = pSettings->HotspotColorOffsets;
	pCRSettings->HotspotDetectionAccuracyX        = pSettings->HotspotDetectionAccuracyX;
	pCRSettings->HotspotDetectionAccuracyY        = pSettings->HotspotDetectionAccuracyY;
	pCRSettings->AllHotspotsMustBeInvalid         = pSettings->AllHotspotsMustBeInvalid;

	pCRSettings->UseSecondBloodDetection          = pSettings->UseSecondBloodDetection;

}

void DSRenderManager::CloseDSEventListenerThread() {

	_METHODNAME(_T("CloseDSEventListenerThread"));

	if(m_hDSMediaEventListener != NULL) {

		// Request closing
		bClosing = TRUE;

		BYTE waitCounter = 0;
		while(waitCounter < MAX_DSELISTENER_CLOSE_ATTEMPT) {

			switch(::WaitForSingleObject(m_hDSMediaEventListener, DSELISTENER_CLOSE_WAIT_INTERVALL)) {

			case WAIT_TIMEOUT:
				waitCounter++;
				break;


			default:
				waitCounter = MAX_DSELISTENER_CLOSE_ATTEMPT;
				break;

			}
		}

		if(::WaitForSingleObject(m_hDSMediaEventListener, 0) == WAIT_TIMEOUT) {
			// Ops - failed to close thread
			ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_DSELISTENER_FAILED_TO_CLOSE);
		}

		::CloseHandle(m_hDSMediaEventListener);
		m_hDSMediaEventListener = NULL;

	}

}

void DSRenderManager::CreateDSEventListenerThread() {

	_METHODNAME(_T("CreateDSEventListenerThread"));

	if(m_hDSMediaEventListener == NULL) {

		DWORD threadID = 0;
		::SetLastError(0);
		m_hDSMediaEventListener = ::CreateThread(
			NULL,                                           // Ignored, must be NULL
			(DWORD)NULL,                                    // Ignored in our case - we use default stack size
			(LPTHREAD_START_ROUTINE)DSGraphEventListener,   // Threads entry
			m_pGraphEvents,                                 // Event listener
			(DWORD)NULL,                                    // No params
			&threadID);

		DWORD dwLastError = ::GetLastError();
		if(m_hDSMediaEventListener == NULL || dwLastError != 0) {
			// Failed
			ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_DSELISTENER_FAILED_TO_CREATE);
		}

	}

}


IBaseFilter* DSRenderManager::FindCamera() {

  _METHODNAME(_T("FindCamera"));

  IBaseFilter* _out                 = NULL;
  IMoniker* pCamera                 = NULL;
  ICreateDevEnum* pSDEnumerator     = NULL;
  IEnumMoniker* pCameraEnumerator   = NULL;


  try {

    // Create device filter enumerator. This will hold all the installed/registered filters, such as video capture 
    // devices and audio capture devices
    HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum, 0, CLSCTX_INPROC, IID_ICreateDevEnum, (void**)&pSDEnumerator);
    if(!SUCCEEDED(hr)) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_DEVENUMERATOR_CREATION_ERROR, hr);
    }

    if(pSDEnumerator == NULL) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_DEVENUMERATOR_IS_NULL);
    }

    // Create video capture filter enumerator
    hr = pSDEnumerator->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pCameraEnumerator, 0);
    if(!SUCCEEDED(hr)) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAMENUMERATOR_CREATION_ERROR, hr);
    }

    if(pCameraEnumerator == NULL) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_CAMENUMERATOR_IS_NULL);
    }

	// Select the instrument camera if found. 
	// Use the laptop camera for debugging purposes if the instrument camera is not found.
	bool instrumentCameraFound = false;
	bool debugCameraFound = false;
	while (pCameraEnumerator->Next(1, &pCamera, NULL) == S_OK) {
		IPropertyBag* pPropBag;
		hr = pCamera->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (SUCCEEDED(hr)) {

			// Get the name of the camera
			VARIANT var;
			VariantInit(&var);
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr)) {
				SafeRelease(&pPropBag);

				// Get the camera information
				ReleaseString(m_cameraType);
				CopyString(var.bstrVal, m_cameraType);
				VariantClear(&var);

				if (wcsncmp(m_cameraType, ORIGINAL_CAMERA_NAME, wcslen(ORIGINAL_CAMERA_NAME)) == 0) {
					// The original camera found
					instrumentCameraFound = true;

					// Get the camera settings
					GetCameraSettings("Software\\IDS\\uEye\\DirectShow\\Device1\\Settings\\UI122xLE-C\\ImageSize\\");
					break;
				} else if (wcsncmp(m_cameraType, SECOND_CAMERA_VERSION_NAME, wcslen(SECOND_CAMERA_VERSION_NAME)) == 0) {
					// The second camera version found
					instrumentCameraFound = true;

					// Get the camera settings
					GetCameraSettings("Software\\IDS\\uEye\\DirectShow\\Device1\\Settings\\UI125xLE-C\\ImageSize\\");
					break;
				}

				if (!debugCameraFound) {
					debugCameraFound = wcsncmp(m_cameraType, DEBUG_CAMERA_NAME, wcslen(DEBUG_CAMERA_NAME)) == 0;
				}
				ReleaseString(m_cameraType);
			}
		}

		SafeRelease(&pCamera);
	}
#ifdef DEBUG
	if (!instrumentCameraFound && debugCameraFound) {
		CopyString(DEBUG_CAMERA_NAME, m_cameraType);
		m_cameraImageWidth = DEBUG_CAMERA_WIDTH;
		m_cameraImageHeight = DEBUG_CAMERA_HEIGHT;
		m_cameraImageWidth_mm = DEBUG_CAMERA_WIDTH_MM;
		m_cameraImageHeight_mm = DEBUG_CAMERA_HEIGHT_MM;
	}
#endif

	// Enumerate all installed cameras and find our camera
	pCameraEnumerator->Reset();
	while(pCameraEnumerator->Next(1, &pCamera, NULL) == S_OK) {
		IPropertyBag* pPropBag;
		hr = pCamera->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (SUCCEEDED(hr)) {

			// Search for our camera
			VARIANT var;
			VariantInit(&var);
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
			if (SUCCEEDED(hr)) {
				SafeRelease(&pPropBag);
				bool cameraFound = m_cameraType != NULL && wcsncmp(var.bstrVal, m_cameraType, wcslen(m_cameraType)) == 0;
				VariantClear(&var);

				if (cameraFound)  {

					// Try to initialize it and get its filter
					hr = pCamera->BindToObject(0, 0, IID_IBaseFilter, (void**)&_out);
					if(SUCCEEDED(hr) && _out != NULL) {
						// Found it!
						break;
					}
				}
			}
		}

      SafeRelease(&pCamera);
    }


  } catch (ExceptionInfo* pEI) {
    // Forward
    throw pEI;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_DSRENDERMANAGER_FINDCAMERA_FAILED);
  } finally {

    SafeRelease(&pCamera);
    SafeRelease(&pCameraEnumerator);
    SafeRelease(&pSDEnumerator);

  }

  return _out;

}

void DSRenderManager::GetCameraSettings(LPCSTR keyPath) {

    // Get the camera settings
    DWORD regValue;
    DWORD dataSize = sizeof(regValue);
    if (ERROR_SUCCESS == RegGetValueA(HKEY_CURRENT_USER, keyPath, "Width", RRF_RT_DWORD, nullptr, &regValue, &dataSize)) {
        m_cameraImageWidth = regValue;
    }
    if (ERROR_SUCCESS == RegGetValueA(HKEY_CURRENT_USER, keyPath, "Height", RRF_RT_DWORD, nullptr, &regValue, &dataSize)) {
        m_cameraImageHeight = regValue;
    }
    if (ERROR_SUCCESS == RegGetValueA(HKEY_CURRENT_USER, keyPath, "Width_mm", RRF_RT_DWORD, nullptr, &regValue, &dataSize)) {
        m_cameraImageWidth_mm = regValue;
    }
    if (ERROR_SUCCESS == RegGetValueA(HKEY_CURRENT_USER, keyPath, "Height_mm", RRF_RT_DWORD, nullptr, &regValue, &dataSize)) {
        m_cameraImageHeight_mm = regValue;
    }
}

void DSRenderManager::SetPunchBorder(BYTE border) {
  m_pRenderer->SetPunchBorder(border);
}

BYTE DSRenderManager::GetPunchBorder() {
  return m_pRenderer->GetPunchBorder();
}

