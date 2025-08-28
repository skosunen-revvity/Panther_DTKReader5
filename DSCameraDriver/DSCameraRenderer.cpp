// --------------------------------------------------------------------------
// DSCameraRenderer.cpp
// --------------------------------------------------------------------------
//
// SpotLight DirectShow camera renderer
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// The actual camera resolution is used for the following operations:
//      Drawing on the image
//      Barcode reading
//      Blood spot searching
//
// $Header: $
// Created: 29.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#include "stdafx.h"
#include "DSCameraRenderer.h"
#include "ExceptionInfo.h"
#include "SimpleLogger.h"

#include <math.h>

// --------------------------------------------------------------------------

// NAme for the image captured event
#define EVENTNAME_IMAGE_CAPTURED      _T("SpotLightDSC_ImageCaptured")

// Class name, used as a param to exception methods
_CLASSNAME(_T("DSCameraRenderer"));

// --------------------------------------------------------------------------


DSCameraRenderer::DSCameraRenderer(IUnknown* unk, HRESULT *hr) 
	: CBaseVideoRenderer(__uuidof(CLSID_DSCameraRenderer), NAME("SpotLight Video Renderer"), unk, hr)
{

	m_FlipHorizontally                = FALSE;
	m_FlipVertically                  = FALSE;
	m_ImageFlipBuffer                 = NULL;
	m_bInitialized                    = FALSE;

	m_DisplayImageBuffer              = NULL;
	m_DisplayImageBufferTmp           = NULL;
	m_DisplayImageSize                = 0;

	m_ImageCapturedEventHAndle        = NULL;
	m_CapturedImage                   = NULL;
	m_ImageRequested                  = FALSE;

	m_SearchAreaAllowedInvalidPixelPercent = 0;
	m_HotspotAllowedInvalidPixelPercent = 0;

	m_ImageType                       = ColorImage;

	m_SearchAreaDetectionAccuracyX    = 0;
	m_SearchAreaDetectionAccuracyY    = 0;

	m_pHotspots                       = NULL;
	m_DrawHotspots                    = FALSE;
	m_HotspotColor                    = RGB(0, 0, 0);
	m_HotspotCount                    = 0;
	m_HotspotOffsetY                  = 0;

	m_HotspotDetectionAccuracyX       = 0;
	m_HotspotDetectionAccuracyY       = 0;
	m_AllHotspotsMustBeInvalid        = FALSE;

	m_DrawBlood                       = FALSE;
	m_BloodColorR                     = 0;
	m_BloodColorG                     = 0;
	m_BloodColorB                     = 0;

	m_PunchDetectionAccuracyX         = 1;        
	m_PunchDetectionAccuracyY         = 1;

	m_FrameSkippedNotificationLimit   = 0;
	m_FrameSkippingStartTime          = 0;

	m_FrameRenderInterval             = 0;
	m_LastRenderTime                  = 0;
	m_BloodcardDetected               = FALSE;

	m_hDrawingSurface                 = NULL;
	m_hDC                             = NULL;
	m_hDCForBCReader                  = NULL;
	m_hBMPForBCReader                 = NULL;

	m_pBloodDetector                  = NULL;
	m_pBarcodeReader                  = NULL;
	m_pPunchDetector                  = NULL;

	m_UseSecondBloodDetection         = FALSE;
	m_pBloodDetectorEx                = NULL;

	m_ImageWidth                      = 0;
	m_ImageHeight                     = 0;

	m_hMemoryDC                       = NULL;
	m_hMemoryBMP                      = NULL;
	m_SearchAreaBorderColor           = RGB(0, 0, 0);

	m_DrawSearchAreaBorder            = TRUE;
	m_SearchAreaBorderWidth           = 1;

	m_previousImageTime               = 0;
	m_imageIntervalMax                = 0;
	m_imageIntervalFiltered           = 0;

	m_bloodDetectionIsOn = true;

	ZeroMemory(&m_SearchAreaColorOffsets, sizeof(INDEXVALUERANGE));
	ZeroMemory(&m_rcClientRect, sizeof(RECT));
	ZeroMemory(&m_BitmapInfo, sizeof(BITMAPINFO));
	ZeroMemory(&m_HotspotSize, sizeof(SIZE));
	ZeroMemory(&m_HotspotColorOffsets, sizeof(INDEXVALUERANGE));
	ZeroMemory(&m_ImageProcessingLock, sizeof(CRITICAL_SECTION));

}

DSCameraRenderer::~DSCameraRenderer() {

	// Release image processing lock
	DeleteCriticalSection(&m_ImageProcessingLock);
	ZeroMemory(&m_ImageProcessingLock, sizeof(CRITICAL_SECTION));


	ReleaseGDIObjects();
	ReleaseHotspots();
	ReleaseFlipBuffer();
	ReleaseDisplayImageBuffer();

	if(m_ImageCapturedEventHAndle != NULL) {
		CloseHandle(m_ImageCapturedEventHAndle);
		m_ImageCapturedEventHAndle = NULL;
	}

	if(m_CapturedImage != NULL) {
		delete []m_CapturedImage;
		m_CapturedImage = NULL;
	}

	if(m_pBarcodeReader != NULL) {
		m_pBarcodeReader->Release();
		delete m_pBarcodeReader;
		m_pBarcodeReader = NULL;
	}

	if(m_pBloodDetector != NULL) {
		m_pBloodDetector->Release();
		delete m_pBloodDetector;
		m_pBloodDetector = NULL;
	}

	if(m_pBloodDetectorEx != NULL) {
		m_pBloodDetectorEx->Release();
		delete m_pBloodDetectorEx;
		m_pBloodDetectorEx = NULL;
	}

	if(m_pPunchDetector != NULL) {
		m_pPunchDetector->Release();
		delete m_pPunchDetector;
		m_pPunchDetector = NULL;
	}

}

void DSCameraRenderer::InitializeRenderer(const PCAMERARENDERERSETTINGS pSettings) {

	_METHODNAME(_T("InitializeRenderer"));
	ThrowAlreadyInitializedException(METHODNAME);

	m_bInitialized = FALSE;

	// Init drawing surface update lock
	InitializeCriticalSection(&m_ImageProcessingLock);

	// Create handle that is used to notify that image was captured succesfully
	m_ImageCapturedEventHAndle = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_IMAGE_CAPTURED);

	// Copy settings to local variables
	CopySettings(FALSE, pSettings);

	// Create GDI related objects that are needed when drawing the image
	CreateGDIObjects();

	// Allocate buffer which is used when image flipping is used
	AllocateFlipBuffer();

	if(pSettings->CalibrationImage != NULL) {
		// Createw hotspot info using the passed calibration image
		CreateHotspots(pSettings->CalibrationImage);
	}

	// When pSettings->CalibrationImage is NULL, hotspots are created when we recd the first image

	m_bInitialized = TRUE;

}

void DSCameraRenderer::ReleaseDisplayImageBuffer() {

	if(m_DisplayImageBuffer != NULL) {
		delete []m_DisplayImageBuffer;
		m_DisplayImageBuffer = NULL;
	}

	m_DisplayImageSize = 0;
	if(m_DisplayImageBufferTmp != NULL) {
		delete []m_DisplayImageBufferTmp;
		m_DisplayImageBufferTmp = NULL;
	}

}

void DSCameraRenderer::SetDisplayImageBuffer(const PBYTE pImage, LONG imageSize) {

	// TODO: Add argument validation - both pImage and imageSize must be valid

	m_DisplayImageBuffer = pImage;
	if(m_DisplayImageBufferTmp != NULL) {
		delete []m_DisplayImageBufferTmp;
	}

	if(imageSize > 0) {
		m_DisplayImageSize = imageSize;
		m_DisplayImageBufferTmp = new BYTE[imageSize];
	} else {
		m_DisplayImageBufferTmp = NULL;
		m_DisplayImageSize = 0;
	}  

}

void DSCameraRenderer::UpdateDrawingSurface(HWND hDrawingSurface) {

	_METHODNAME(_T("UpdateDrawingSurface"));
	ThrowNotInitializedException(METHODNAME);

	EnterCriticalSection(&m_ImageProcessingLock);

	ReleaseGDIObjects();
	m_hDrawingSurface = hDrawingSurface;
	CreateGDIObjects();

	// These two calls are exactly as in the method CheckMediaType. They are needed in order to get TakePicture
	// and GrabPicture to work in an environment were the drawing surface is repeatedly changed. More comments
	// in CheckMediaType.
	m_hMemoryBMP = CreateDIBSection(m_hDC, &m_BitmapInfo, DIB_RGB_COLORS, (void**)&m_outputBits, NULL, 0);
	SelectObject(m_hMemoryDC, m_hMemoryBMP);

	LeaveCriticalSection(&m_ImageProcessingLock);

}

void DSCameraRenderer::InitiateImageCapturing() {

	m_ImageRequested = TRUE;

}

void DSCameraRenderer::HandleImageCapturing(const PBYTE pImage) {

	if(m_ImageRequested) {
		m_ImageRequested = FALSE;

		// Calc the size of the buffer
		size_t imgSize = m_ImageWidth * m_ImageHeight * sizeof(SPOTLIGHTCOLOR);
		if(m_CapturedImage == NULL) {
			// Create buffer
			m_CapturedImage = new BYTE[imgSize];
		}

		// Copy image to buffer and raise event
		memcpy(m_CapturedImage, pImage, imgSize);

		SetEvent(m_ImageCapturedEventHAndle);
	}

}

void DSCameraRenderer::ReleaseCapturedImage() {

	if(m_CapturedImage != NULL) {
		delete []m_CapturedImage;
		m_CapturedImage = NULL;
	}

	ResetEvent(m_ImageCapturedEventHAndle);

}

void DSCameraRenderer::GetCurrentSettings(PCAMERARENDERERSETTINGS pSettings) {

	_METHODNAME(_T("GetCurrentSettings"));
	ThrowNotInitializedException(METHODNAME);

	pSettings->CalibrationImage                   = NULL;
	pSettings->FlipHorizontally                   = m_FlipHorizontally;
	pSettings->FlipVertically                     = m_FlipVertically;
	pSettings->ImageType                          = m_ImageType;

	pSettings->SearchAreaAllowedInvalidPixelPercent = m_SearchAreaAllowedInvalidPixelPercent;
	pSettings->HotspotAllowedInvalidPixelPercent = m_HotspotAllowedInvalidPixelPercent;

	pSettings->SearchAreaDetectionAccuracyX       = m_SearchAreaDetectionAccuracyX;
	pSettings->SearchAreaDetectionAccuracyY       = m_SearchAreaDetectionAccuracyY;
	pSettings->DrawHotspots                       = m_DrawHotspots;
	pSettings->HotspotColor                       = m_HotspotColor;
	pSettings->HotspotCount                       = m_HotspotCount;
	pSettings->HotspotOffsetY                     = m_HotspotOffsetY;
	pSettings->HotspotSize                        = m_HotspotSize;
	pSettings->HotspotColorOffsets                = m_HotspotColorOffsets;
	pSettings->HotspotDetectionAccuracyX          = m_HotspotDetectionAccuracyX;
	pSettings->HotspotDetectionAccuracyY          = m_HotspotDetectionAccuracyY;
	pSettings->AllHotspotsMustBeInvalid           = m_AllHotspotsMustBeInvalid;

	pSettings->DrawPunchPositions                 = m_DrawPunchPositions;
	pSettings->DrawSearchAreaBorder               = m_DrawSearchAreaBorder;
	pSettings->DrawingSurface                     = m_hDrawingSurface;
	pSettings->SearchAreaColorOffsets             = m_SearchAreaColorOffsets;
	pSettings->SearchAreaBorderColor              = m_SearchAreaBorderColor;
	pSettings->SearchAreaBorderWidth              = m_SearchAreaBorderWidth;
	pSettings->DrawBlood                          = m_DrawBlood;
	pSettings->BloodColor                         = RGB(m_BloodColorR, m_BloodColorG, m_BloodColorB);

	pSettings->PunchDetectionAccuracyX            = m_PunchDetectionAccuracyX;
	pSettings->PunchDetectionAccuracyY            = m_PunchDetectionAccuracyY;
	pSettings->FrameRenderInterval                = m_FrameRenderInterval;
	pSettings->FramesSkippedNotificationLimit     = m_FrameSkippedNotificationLimit;

	pSettings->pBarcodeReader                     = m_pBarcodeReader;
	pSettings->pBloodDetector                     = m_pBloodDetector;
	pSettings->pPunchDetector                     = m_pPunchDetector;

	pSettings->UseSecondBloodDetection            = m_UseSecondBloodDetection;
	pSettings->pBloodDetector2                    = m_pBloodDetectorEx;

}

void DSCameraRenderer::UpdateSettings(const PCAMERARENDERERSETTINGS pSettings) {

	_METHODNAME(_T("GetCurrentSettings"));
	ThrowNotInitializedException(METHODNAME);

	ReleaseGDIObjects();

	CopySettings(TRUE, pSettings);
	CreateGDIObjects();

	AllocateFlipBuffer();
	ReleaseHotspots();

	if(pSettings->CalibrationImage != NULL) {
		CreateHotspots(pSettings->CalibrationImage);
	}

}

void DSCameraRenderer::CopySettings(BYTE update, const PCAMERARENDERERSETTINGS pSettings) {

	m_FlipHorizontally                = pSettings->FlipHorizontally;
	m_FlipVertically                  = pSettings->FlipVertically;
	m_hDrawingSurface                 = pSettings->DrawingSurface;

	m_SearchAreaAllowedInvalidPixelPercent  = pSettings->SearchAreaAllowedInvalidPixelPercent;
	m_HotspotAllowedInvalidPixelPercent     = pSettings->HotspotAllowedInvalidPixelPercent;

	m_ImageType                       = pSettings->ImageType;
	m_SearchAreaDetectionAccuracyX    = pSettings->SearchAreaDetectionAccuracyX;
	m_SearchAreaDetectionAccuracyY    = pSettings->SearchAreaDetectionAccuracyY;

	m_DrawHotspots                    = pSettings->DrawHotspots;
	m_HotspotColor                    = pSettings->HotspotColor;
	m_HotspotCount                    = pSettings->HotspotCount;
	m_HotspotOffsetY                  = pSettings->HotspotOffsetY;
	m_HotspotSize                     = pSettings->HotspotSize;
	m_HotspotColorOffsets             = pSettings->HotspotColorOffsets;
	m_HotspotDetectionAccuracyX       = pSettings->HotspotDetectionAccuracyX;
	m_HotspotDetectionAccuracyY       = pSettings->HotspotDetectionAccuracyY;
	m_AllHotspotsMustBeInvalid        = pSettings->AllHotspotsMustBeInvalid;

	m_DrawBlood                       = pSettings->DrawBlood;

	m_BloodColorR                     = GetRValue(pSettings->BloodColor);
	m_BloodColorG                     = GetGValue(pSettings->BloodColor);
	m_BloodColorB                     = GetBValue(pSettings->BloodColor);

	m_PunchDetectionAccuracyX         = pSettings->PunchDetectionAccuracyX;        
	m_PunchDetectionAccuracyY         = pSettings->PunchDetectionAccuracyY;

	m_FrameRenderInterval             = pSettings->FrameRenderInterval;
	m_FrameSkippedNotificationLimit   = pSettings->FramesSkippedNotificationLimit;

	m_SearchAreaColorOffsets          = pSettings->SearchAreaColorOffsets;

	m_SearchAreaBorderColor           = pSettings->SearchAreaBorderColor;
	m_DrawSearchAreaBorder            = pSettings->DrawSearchAreaBorder;
	m_SearchAreaBorderWidth           = pSettings->SearchAreaBorderWidth;
	m_DrawPunchPositions              = pSettings->DrawPunchPositions;

	ReleaseHotspots();

	m_UseSecondBloodDetection         = pSettings->UseSecondBloodDetection;

	// When updating settings, we dont change detectors, BC reader and image size
	if(!update) {
		m_pBloodDetector                = pSettings->pBloodDetector;
		m_pBarcodeReader                = pSettings->pBarcodeReader;
		m_pPunchDetector                = pSettings->pPunchDetector;

		m_pBloodDetectorEx              = pSettings->pBloodDetector2;

		// Camera image settings
		m_ImageWidth                    = pSettings->CameraImageWidth;
		m_ImageHeight                   = pSettings->CameraImageHeight;

		// UI image settings
		m_rcClientRect.left             = 0;
		m_rcClientRect.top              = 0;
		m_rcClientRect.right            = pSettings->UiImageSize.cx;
		m_rcClientRect.bottom           = pSettings->UiImageSize.cy;
	}

}

void DSCameraRenderer::ReleaseGDIObjects() {

	if(m_hBMPForBCReader != NULL) {
		DeleteObject(m_hBMPForBCReader);
		m_hBMPForBCReader = NULL;
	}

	if(m_hDCForBCReader != NULL) {
		DeleteObject(m_hDCForBCReader);
		m_hDCForBCReader = NULL;
	}

	if(m_hMemoryBMP != NULL) {
		DeleteObject(m_hMemoryBMP);
		m_hMemoryBMP = NULL;
	}

	if(m_hMemoryDC != NULL) {
		DeleteObject(m_hMemoryDC);
		m_hMemoryDC = NULL;
	}

	if(m_hDC != NULL) {
		ReleaseDC(m_hDrawingSurface, m_hDC);
		m_hDC = NULL;
	}

}

void DSCameraRenderer::CreateGDIObjects() {

	// Get device contect to our drawing surface
	m_hDC = GetDC(m_hDrawingSurface);

	// Create temp DC where we first draw our image. When image is complete, image is copied
	// from this to actual DC - m_hDC
	m_hMemoryDC = CreateCompatibleDC(m_hDC);

	// We must have BMP before we can draw
	m_hMemoryBMP = CreateCompatibleBitmap(m_hDC, m_ImageWidth, m_ImageHeight);

	SelectObject(m_hMemoryDC, m_hMemoryBMP);

	// When detecting barcode, we use orginal image. DC and bitmap for barcode reader
	m_hDCForBCReader = CreateCompatibleDC(m_hDC);
	m_hBMPForBCReader = CreateCompatibleBitmap(m_hDC, m_ImageWidth, m_ImageHeight);
	SelectObject(m_hDCForBCReader, m_hBMPForBCReader);

}

HRESULT DSCameraRenderer::CheckMediaType(const CMediaType *media) {

	// Called by DS runtime when detecting what kind of image we are intrested at
	if(TryEnterCriticalSection(&m_ImageProcessingLock) == 0) {
		// Image processing is skipped
		return E_FAIL;
	}


	_METHODNAME(_T("CheckMediaType"));
	try {

		VIDEOINFO* vi;

		// TODO: Jos on grayscale niin ei kai mÅEtarvita 24 bit?!?!
		// We want 24 bit image
		if(!IsEqualGUID(*media->Subtype(), MEDIASUBTYPE_RGB24) || !(vi = (VIDEOINFO*)media->Format())) {
			return E_FAIL;
		}

		// And size must be...
		if(vi->bmiHeader.biWidth != m_ImageWidth || vi->bmiHeader.biHeight != m_ImageHeight) {
			return E_FAIL;
		}

		// Connect memory bitmap to drawing surface. Similar call of SelectObject in CreateGDIObjects does not the trick
		// since memoryBMP handle is created there using different method. BitmapInfo is not available there, and CreateDIBSection
		// cannot be used there.
		m_BitmapInfo.bmiHeader = vi->bmiHeader;
		m_hMemoryBMP = CreateDIBSection(m_hDC, &m_BitmapInfo, DIB_RGB_COLORS, (void**)&m_outputBits, NULL, 0); 
		SelectObject(m_hMemoryDC, m_hMemoryBMP);

		return S_OK;

	} catch (ExceptionInfo* pEX) {
		g_pPunchPosManager->SetCameraDriverException(pEX);
	} catch (...) {
		g_pPunchPosManager->SetCameraDriverException(new ExceptionInfo(
			CLASSNAME, METHODNAME, ERR_DSCAMERARENDERER_CHECKMEDIATYPE_FAILED));
	} finally {
		LeaveCriticalSection(&m_ImageProcessingLock);
	}

	return E_FAIL;

}

void DSCameraRenderer::RestartImaging() {
	m_previousImageTime = 0;
}

HRESULT DSCameraRenderer::DoRenderSample(IMediaSample *pMediaSample) {
	bool attemptedBarcodeRead = false;
	// Called by DS runtime when its time to render the image

	if(TryEnterCriticalSection(&m_ImageProcessingLock) == 0) {
		// Image processing is skipped
		return E_FAIL;
	}

	_METHODNAME(_T("DoRenderSample"));
	BYTE locked = FALSE;

	try {
		DWORD now = GetTickCount();
		if (m_previousImageTime != 0) {
			DWORD diff = now - m_previousImageTime;
			if (diff > m_imageIntervalMax) {
				m_imageIntervalMax = diff;
			}
			if (m_imageIntervalFiltered == 0) {
				// Non-initialized, initialize with first measurement data
				m_imageIntervalFiltered = diff;
			} else {
				// Calculate low-pass filtered value
				m_imageIntervalFiltered = (int)((9.0 * m_imageIntervalFiltered + diff + 5.0) / 10.0);
			}
		}
		m_previousImageTime = now;
		PBYTE pData;


		// We must have access to various data - this will fail when client is updating settings or
		// fetching punch positions or settings, in which case we just draw the image
		if(g_pPunchPosManager->TryToLock()) {
			locked = TRUE;


			BYTE inPunchProgressMode = InPunchProgressMode();

			if(m_DisplayImageBuffer != NULL) {
				// Still image used - copy data to buffer
				memcpy(m_DisplayImageBufferTmp, m_DisplayImageBuffer, m_DisplayImageSize);

				// TODO: If image was flipped when it was captured, and the flipping is turned on,
				//       it will be flipped here again, and the result is wrong image

				// Perform flipping
				pData = FlipImage(m_DisplayImageBufferTmp);
			} else {
				// Get image and perform flipping
				pMediaSample->GetPointer(&pData);
				pData = FlipImage(pData);
			}


			// Reset skip time
			m_FrameSkippingStartTime = 0;


			BYTE cardInSearchArea = FALSE;
			if(!inPunchProgressMode) {
				// Create card detection helpers - if hotspots are already created, this won¥t
				// do anything
				CreateHotspots(pData);

				// OUTCOMMENTED: Take HandleImageCapturing below into use, if capturing of camera seen image 
				// becomes of interest later
				/* 
				// Cache latest camera image if one is requested. Image is copied to buffer and event is 
				// raised to notify the listener that image is captured
				HandleImageCapturing(pData); */


				// Read barcodes, if reading is turned on. If barcodes change, event is raised
				attemptedBarcodeRead = ReadBarcodes(pData);

				if (CheckHotspots(pData)) {
					cardInSearchArea = TRUE;
					// Card detected - notify UI, but only once
					if(!m_BloodcardDetected) {
						OutputDebugStringA("*** Card entered ***");
						m_BloodcardDetected = TRUE;
						g_pPunchPosManager->NotifyBloodCardDetectedListener();
					}
				} else {
					// Card removed - notify UI, but only once
					if(m_BloodcardDetected) {
						OutputDebugStringA("*** Card removed ***");
						m_BloodcardDetected = FALSE;
						g_pPunchPosManager->NotifyBloodCardRemovedListener();
					}
				}
			} else {
				// We are in card mode, when displaying punch progress
				cardInSearchArea = TRUE;
			}


			if(cardInSearchArea) {
				// Card detected
				OnCard(pData, attemptedBarcodeRead);
			} else {
				// No card
				OnEmpty(pData);
			}


			// Draw search area border and hotspots
			DrawSearchArea();
			DrawHotspots();

			// Cache latest display image if one is requested. Image is copied to buffer and event is 
			// raised to notify the listener that image is captured
			HandleImageCapturing(m_outputBits);

		} else {

			// Failed to lock global data

			// Notify, if enough time has elapsed, that we were unable to process image
			FrameProcessingSkipped();

			// NOTE: We can¥t use m_DisplayImageBufferTmp - we were unable to lock g_pPunchPosManager
			pMediaSample->GetPointer(&pData);
			pData = FlipImage(pData);

			SetDIBitsToDevice(m_hMemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, 0, 0, 0,
				m_ImageHeight, pData, &m_BitmapInfo, DIB_RGB_COLORS);
		}

		// Draw! The processed/skipped image is drawn to surface that was specified in settings
		SetStretchBltMode(m_hDC, HALFTONE);
		StretchBlt(m_hDC, 0, 0, m_rcClientRect.right, m_rcClientRect.bottom, m_hMemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, SRCCOPY);

	} catch (ExceptionInfo* pEX) {
		g_pPunchPosManager->SetCameraDriverException(pEX);
	} catch (...) {
		g_pPunchPosManager->SetCameraDriverException(new ExceptionInfo(
			CLASSNAME, METHODNAME, ERR_DSCAMERARENDERER_DORENDERSAMPLE_FAILED));
	} finally {
		if(locked) {
			g_pPunchPosManager->Unlock();
		}
		LeaveCriticalSection(&m_ImageProcessingLock);
	}

	return S_OK;  
}

HRESULT DSCameraRenderer::ShouldDrawSampleNow(IMediaSample *sample, REFERENCE_TIME *start, REFERENCE_TIME *stop) {

	// Called by DS runtime, to check whether we want to draw the specified image or not

	if(TryEnterCriticalSection(&m_ImageProcessingLock) == 0) {
		// Image processing is skipped
		return E_FAIL;
	}


	_METHODNAME(_T("ShouldDrawSampleNow"));
	try {

		DWORD currentTime = GetTickCount();
		if(currentTime < m_LastRenderTime) {
			m_LastRenderTime = 0;
		} else {
			if((currentTime - m_LastRenderTime) < m_FrameRenderInterval) {
				// Not yet
				return E_FAIL;
			}

			m_LastRenderTime = currentTime;
		}

		// We want render this frame
		return S_OK;

	} catch (ExceptionInfo* pEX) {
		g_pPunchPosManager->SetCameraDriverException(pEX);
	} catch (...) {
		g_pPunchPosManager->SetCameraDriverException(new ExceptionInfo(
			CLASSNAME, METHODNAME, ERR_DSCAMERARENDERER_SHOULDDRAWSAMPLENOW_FAILED));
	} finally {
		LeaveCriticalSection(&m_ImageProcessingLock);
	}

	return E_FAIL;

}

void DSCameraRenderer::OnEmpty(PBYTE pImageData) {

	// TODO: Meid‰n pit‰is t‰‰llÅEvarmaan pist‰‰ punchi paikat viallisiksi
	//
	//       Jos simulaatio vaiheessa n‰ytet‰‰n kuvaa jossa on kortti ja verta, ja punchi paikat lˆydet‰‰n,
	//       ja jos seuraavaksi n‰ytet‰‰n kuvaa jossa ei ole korttia..ja jos t‰ssÅEvaiheessa kysyy puncheja
	//       palautetaan aikaisemman kuvan yhteydessÅElˆytyneet punchit

	SetDIBitsToDevice(m_hMemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, 0, 0, 0, m_ImageHeight, 
		pImageData, &m_BitmapInfo, DIB_RGB_COLORS);

	m_pBarcodeReader->ClearBarcodes();
}

void DSCameraRenderer::OnCard(PBYTE pImageData, bool disablePunchSearch) {

	// Do we have requested punch positions
	BYTE punchCount = g_pPunchPosManager->GetPunchPositionCount();

	if(punchCount > 0) {

		// Detect blood and punch positions only when in detection mode, and not displaying the 
		// punch progress
		if(!InPunchProgressMode()) {
			BYTE bloodDetected = m_pBloodDetector->Detect(pImageData);
			if (bloodDetected || !m_bloodDetectionIsOn) {
				// Blood detected - detect punch positions
				if (!disablePunchSearch) {
					DetectPunchPositions(pImageData);
				}
			} else {
				// No blood - clear all positions and notify listener if valid position count have changed
				PUNCHPOSITION* pPositions = NULL;
				BYTE punchCount = 0;
				BYTE validPositions = g_pPunchPosManager->GetCountOfPunchPositionsWithValidPosition();

				g_pPunchPosManager->GetPunchPositions(punchCount, pPositions);
				ResetPunchPositions(punchCount, pPositions);
				g_pPunchPosManager->SetPunchPositions(punchCount, pPositions);

				ReleasePunchPositions(pPositions);
				if(validPositions != g_pPunchPosManager->GetCountOfPunchPositionsWithValidPosition()) {
					// Count changed, notify listener
					g_pPunchPosManager->NotifyPunchPositionCountChangedListener();
				}
			}
		} 


		// Draw found blood. Blood is drawn to actual image, without GDI, by just changing the actual
		// image data
		DrawBlood(pImageData);

		if(m_DrawPunchPositions) {
			// We use GDI to draw the punch and their IDs, so we need to first draw the bitmap into DC...
			::SetDIBitsToDevice(m_hMemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, 0, 0, 0,
				m_ImageHeight, pImageData, &m_BitmapInfo, DIB_RGB_COLORS);

			// ...and then draw the punch positions and their IDs
			PaintPunchMap();
			DrawPunchNumbers();

			return;
		}

	}

	SetDIBitsToDevice(m_hMemoryDC, 0, 0, m_ImageWidth, m_ImageHeight, 0, 0, 0,
		m_ImageHeight, pImageData, &m_BitmapInfo, DIB_RGB_COLORS);

}

void DSCameraRenderer::DetectPunchPositions(PBYTE pImageData) {

	// At this point, blood was detected from the image, using the main blood detector (m_pBloodDetector)


	// Get the number of requested positions
	BYTE requestedPunchCount = g_pPunchPosManager->GetRequestedPunchPositionCount();

	// Get the number of positions that was found previously
	BYTE prevFoundPositions = g_pPunchPosManager->GetCountOfPunchPositionsWithValidPosition();


	BYTE punchCount;

	BYTE found                = 0;
	PUNCHPOSITION* pPositions = NULL;
	DWORD detectionTime       = GetTickCount();

	// Clear punch detection info
	m_pPunchDetector->Clear(requestedPunchCount);

	// Count changed?
	if(requestedPunchCount == prevFoundPositions) {

		// Get previously found positions and check if they are still valid
		g_pPunchPosManager->GetPunchPositions(punchCount, pPositions);

		// Are we using different values for the prev pos detection? The image contains noise, which may 
		// cause the position flickering if using same blood detection values when the position was found
		// in the first place. By using a bit looser values, vil prevent the flickering
		if(m_UseSecondBloodDetection) {
			if(m_pBloodDetectorEx->Detect(pImageData)) {
				// Blood detected - validate positions
				found = CheckPunchPositions(m_pBloodDetectorEx, m_pPunchDetector, punchCount, pPositions);
			}
		} else {
			// Validate positions using the main detector
			found = CheckPunchPositions(m_pBloodDetector, m_pPunchDetector, punchCount, pPositions);
		}


		if(found != requestedPunchCount) {
			// Not enough - start from scratch
			m_pPunchDetector->Clear(requestedPunchCount);

			// Try to find punch positions using the main detector
			DetectPunchPositions(m_pBloodDetector, m_pPunchDetector, m_PunchDetectionAccuracyX,
				m_PunchDetectionAccuracyY, punchCount, pPositions);
		}

		// Store positions
		StorePositions(detectionTime, punchCount, pPositions);

	} else {

		// Count changed - perform clean detection
		PUNCHPOSITION* pPositionsForCleanSearch = NULL;
		g_pPunchPosManager->GetPunchPositions(punchCount, pPositionsForCleanSearch);

		// Find punch positions using the main detector
		BYTE newPositions = 0;
		newPositions = DetectPunchPositions(m_pBloodDetector, m_pPunchDetector, m_PunchDetectionAccuracyX,
			m_PunchDetectionAccuracyY, punchCount, pPositionsForCleanSearch);


		if(prevFoundPositions > 0 && newPositions == prevFoundPositions) {

			// Same amount that last time. Check if prev positions are still valid
			g_pPunchPosManager->GetPunchPositions(punchCount, pPositions);
			m_pPunchDetector->Clear(requestedPunchCount);

			found = 0;


			// Are we using different values for the prev pos detection? The image contains noise, which may 
			// cause the position flickering if using same blood detection values when the position was found
			// in the first place. By using a bit looser values, vil prevent the flickering
			if(m_UseSecondBloodDetection) {
				if(m_pBloodDetectorEx->Detect(pImageData)) {
					// Blood detected - validate previously found positions
					found = CheckPunchPositions(m_pBloodDetectorEx, m_pPunchDetector, punchCount, pPositions);
				} else {
					// No blood - clear prev positions
					ResetPunchPositions(punchCount, pPositions);
				}
			} else {
				// Validate positions using the main detector
				found = CheckPunchPositions(m_pBloodDetector, m_pPunchDetector, punchCount, pPositions);
			}


			if(found == newPositions) {
				// Old positions are still valid - use them
				StorePositions(detectionTime, punchCount, pPositions);
			} else {
				// Some of the old positions are invalid - use new
				StorePositions(detectionTime, punchCount, pPositionsForCleanSearch);
			}

		} else {
			// Count have changed - use new
			StorePositions(detectionTime, punchCount, pPositionsForCleanSearch);
		}

		// Release temp object
		ReleasePunchPositions(pPositionsForCleanSearch);
	}

	// Release temp object
	ReleasePunchPositions(pPositions);

	if(prevFoundPositions != g_pPunchPosManager->GetCountOfPunchPositionsWithValidPosition()) {
		// Count with vcalid positions have changed - notify listener
		g_pPunchPosManager->NotifyPunchPositionCountChangedListener();
	}

}

void DSCameraRenderer::ResetPunchPositions(BYTE count, PUNCHPOSITION* pPositions) {

	for(BYTE i = 0; i < count; i++) {
		pPositions[i].InBlood = FALSE;

		// Clear position if its not locked
		if(!pPositions[i].Locked) {
			pPositions[i].X = UNKNOWN_COORD_X;
			pPositions[i].Y = UNKNOWN_COORD_Y;
		}
	}
}

void DSCameraRenderer::StorePositions(DWORD detectionTime, BYTE count, PUNCHPOSITION* pPositions) {

	// Store info about valid positions so we can re-arrange them
	POINT* pValidPositions = new POINT[count];
	BYTE validIndex = 0;
	for(BYTE i = 0; i < count; i++) {
		pValidPositions[i].x = UNKNOWN_COORD_X;
		pValidPositions[i].y = UNKNOWN_COORD_Y;

		if(pPositions[i].Used && !pPositions[i].Locked) {
			if(pPositions[i].InBlood) {
				pValidPositions[validIndex].x = pPositions[i].X;
				pValidPositions[validIndex].y = pPositions[i].Y;

				validIndex++;
			}
		}
	}

	validIndex = 0;
	for(BYTE i = 0; i < count; i++) {
		if(pPositions[i].Used) {

			if(pPositions[i].InBlood) {
				pPositions[i].dwTime = detectionTime;

				// TODO: Onko t‰rkeetÅEettÅElukitun position paikka on juuri se mikÅEon m‰‰r‰tty, 
				//       cai riitt‰‰kÅEettÅEpaikka on yksi lukituista...jos ei riitÅEniin t‰‰ ei 
				//       toimi

				if(!pPositions[i].Locked) {
					pPositions[i].X = (SHORT)pValidPositions[validIndex].x;
					pPositions[i].Y = (SHORT)pValidPositions[validIndex].y;

					validIndex++;
				}
			} else {
				pPositions[i].dwTime = 0;

				if(!pPositions[i].Locked) {
					pPositions[i].X = UNKNOWN_COORD_X;
					pPositions[i].Y = UNKNOWN_COORD_Y;
				}
			}

		} else {

			pPositions[i].dwTime = 0;
			pPositions[i].InBlood = FALSE;

			if(!pPositions[i].Locked) {
				pPositions[i].X = UNKNOWN_COORD_X;
				pPositions[i].Y = UNKNOWN_COORD_Y;
			}
		}
	}

	g_pPunchPosManager->SetPunchPositions(count, pPositions);
	delete []pValidPositions;

}

void DSCameraRenderer::ReleasePunchPositions(PPUNCHPOSITION& pPositions) {

	if(pPositions != NULL) {
		delete[]pPositions;
		pPositions = NULL;
	}

}

BYTE DSCameraRenderer::CheckPunchPositions(BloodDetector* pBloodDetector, PunchDetector* pPunchDetector, BYTE count, PUNCHPOSITION* pPositions) {
	BYTE validPositionCount = 0;
	for(BYTE i = 0; i < count; i++) {
		if(pPositions[i].Used) {
			if(!pPunchDetector->Detect(pPositions[i].X, pPositions[i].Y, pPositions[i].Radius, pPositions[i].Locked ? FALSE: TRUE, pBloodDetector)) {
				// Position is invalid
				pPositions[i].InBlood = FALSE;
				if(!pPositions[i].Locked) {
					pPositions[i].X = UNKNOWN_COORD_X;
					pPositions[i].Y = UNKNOWN_COORD_Y;
				}
			} else {
				pPositions[i].InBlood = TRUE;
				validPositionCount++;
				pPositions[i].ID = validPositionCount;
			}
		}
	}

	return validPositionCount;
}

BYTE DSCameraRenderer::DetectPunchPositions(BloodDetector* pBloodDetector, PunchDetector* pPunchDetector,
	BYTE detectionAccuracyX, BYTE detectionAccuracyY, BYTE count, PUNCHPOSITION* pPositions) {

		POINT tl = m_pBloodDetector->GetSearchAreaTopLeft();
		POINT br = m_pBloodDetector->GetSearchAreaBottomRight();

		BYTE foundPositions = 0;

		USHORT x = (USHORT)tl.x;
		USHORT y = (USHORT)tl.y;

		BYTE found;

		for(BYTE i = 0; i < count; i++) {
			if(pPositions[i].Used) {

				found = FALSE;
				if(pPositions[i].Locked) {
					// Punch is locked - just check if its position is valid
					if((!m_bloodDetectionIsOn) || pPunchDetector->Detect(pPositions[i].X, pPositions[i].Y, pPositions[i].Radius, FALSE, pBloodDetector)) {
						foundPositions++;
						pPositions[i].ID = foundPositions;
						found = TRUE;
					}

					x = pPositions[i].X;
					y = pPositions[i].Y;

				} else {
					// Find next available position

					// Searching will start from the last found position, or if we have reached the end already
					// it will be skipped completely
					for(; y < br.y; y += detectionAccuracyY) {
						for(; x < br.x; x += detectionAccuracyX) {

							if(pPunchDetector->Detect(x, y, pPositions[i].Radius, TRUE, pBloodDetector)) {
								pPositions[i].X = x;
								pPositions[i].Y = y;

								foundPositions++;
								pPositions[i].ID = foundPositions;
								found = TRUE;
								break;
							}
						}

						if(found) {
							break;
						}

						x = (USHORT)tl.x;
					}
				}

				pPositions[i].InBlood = found;

				if(found) {
					// Search for next punch will start here
					y -= (pPositions[i].Radius + 1);
					x += pPositions[i].Radius;
				} else {
					// Failed to find place for this punch...and the rest: we dont init X and Y so 
					// we will come here
					if(!pPositions[i].Locked) {
						pPositions[i].X = UNKNOWN_COORD_X;
						pPositions[i].Y = UNKNOWN_COORD_Y;
					}
				}
			}
		}

		return foundPositions;

}

void DSCameraRenderer::DrawSearchArea() {

	if(m_DrawSearchAreaBorder) {

		POINT _tl = m_pBloodDetector->GetSearchAreaTopLeft();
		POINT _br = m_pBloodDetector->GetSearchAreaBottomRight();
		POINT tl;
		POINT br;

		ConvertToGDIZpl(_tl, tl);
		ConvertToGDIZpl(_br, br);

		// Get currently selected
		HGDIOBJ hOldPen = ::GetCurrentObject(m_hMemoryDC, OBJ_PEN);
		HGDIOBJ hOldBrush = ::GetCurrentObject(m_hMemoryDC, OBJ_BRUSH);


		// Create pen and brush
		HPEN hPen = ::CreatePen(PS_SOLID, m_SearchAreaBorderWidth, m_SearchAreaBorderColor);
		::SelectObject(m_hMemoryDC, hPen);

		// Transparent brush for background
		LOGBRUSH lb;
		lb.lbColor = RGB(0, 0, 0);
		lb.lbHatch = 0;
		lb.lbStyle = BS_HOLLOW;

		HBRUSH hBrush = ::CreateBrushIndirect(&lb);
		::SelectObject(m_hMemoryDC, hBrush);

		// Draw search area border
		::Ellipse(m_hMemoryDC, tl.x, tl.y, br.x, br.y);

		// Restore and release
		::SelectObject(m_hMemoryDC, hOldPen);
		::SelectObject(m_hMemoryDC, hOldBrush);

		::DeleteObject(hPen);
		::DeleteObject(hBrush);

	}

}

void DSCameraRenderer::PaintPunchMap() {

	if(m_DrawPunchPositions && (g_pPunchPosManager->GetDrawBorder() || g_pPunchPosManager->GetFillBkg())) {

		BYTE punchCount = 0;
		PUNCHPOSITION* pPositions = NULL;
		g_pPunchPosManager->GetPunchPositions(punchCount, pPositions);

		if(punchCount > 0) {

			// Get currently selected
			HGDIOBJ hOldPen = ::GetCurrentObject(m_hMemoryDC, OBJ_PEN);
			HGDIOBJ hOldBrush = ::GetCurrentObject(m_hMemoryDC, OBJ_BRUSH);

			BYTE drawBorder;
			BYTE fillBackground;
			BYTE rad;
			POINT pt;
			POINT _pt;

			for(BYTE i = 0; i < punchCount; i++) {
				if(pPositions[i].Used) {

					// Should we draw border
					if(g_pPunchPosManager->GetDrawBorder()) {
						if(g_pPunchPosManager->GetDrawBorderIfLocked()) {
							if(pPositions[i].Locked) {
								// When locked, part of pattern, border is drawn
								drawBorder = TRUE;
							} else {
								// If contains bloode, draw it
								drawBorder = pPositions[i].InBlood;
							}
						} else {
							// Contains bloode, draw
							drawBorder = pPositions[i].InBlood;
						}
					} else {
						drawBorder = FALSE;
					}


					// Fill background?
					if(g_pPunchPosManager->GetFillBkg()) {
						// If contains bloode, draw it
						fillBackground = pPositions[i].InBlood;
					} else {
						fillBackground = FALSE;
					}



					if(drawBorder || fillBackground) {

						HPEN hPen;
						HBRUSH hBrush;

						if(drawBorder) {
							hPen = ::CreatePen(PS_SOLID, pPositions[i].BorderWidth, pPositions[i].BorderColor);
						} else {
							// When background is filled, border is also drawn, but the color is the same
							// as the background color
							hPen = ::CreatePen(PS_SOLID, pPositions[i].BorderWidth, pPositions[i].FillColor);
						}

						if(fillBackground) {
							hBrush = ::CreateSolidBrush(pPositions[i].FillColor);
						} else {
							// Transparent brush
							LOGBRUSH lb;
							lb.lbColor = RGB(0, 0, 0);
							lb.lbHatch = 0;
							lb.lbStyle = BS_HOLLOW;

							hBrush = ::CreateBrushIndirect(&lb);
						}

						::SelectObject(m_hMemoryDC, hPen);
						::SelectObject(m_hMemoryDC, hBrush);

						rad   = pPositions[i].Radius;
						pt.x  = pPositions[i].X;
						pt.y  = pPositions[i].Y;

						// From camera to UI co-ordinates
						ConvertToGDIZpl(pt, _pt);

						// Draw - border and background
						::Ellipse(m_hMemoryDC, _pt.x - rad, _pt.y - rad, _pt.x + rad, _pt.y + rad);

						::DeleteObject(hPen);
						::DeleteObject(hBrush);
					}
				}
			}

			// Restore
			::SelectObject(m_hMemoryDC, hOldPen);
			::SelectObject(m_hMemoryDC, hOldBrush);

		}

		// Release temp object
		ReleasePunchPositions(pPositions);
	}

}


void DSCameraRenderer::DrawPunchNumbers() {

	// Should we draw - check general settings and positions related
	if(m_DrawPunchPositions && g_pPunchPosManager->GetDrawId()) {

		BYTE punchCount = 0;
		PUNCHPOSITION* pPositions = NULL;

		// Get positions
		g_pPunchPosManager->GetPunchPositions(punchCount, pPositions);

		if(punchCount > 0) {

			TCHAR szID[5];
			RECT rc;
			BYTE rad;

			BYTE drawID;
			BYTE fillBackground;
			BYTE validPosition;

			POINT _pt;
			POINT pt;
			int t;

			// Get current
			int oldBkMode         = ::GetBkMode(m_hMemoryDC);
			COLORREF oldTextColor = ::GetTextColor(m_hMemoryDC);
			COLORREF oldBkColor   = ::GetBkColor(m_hMemoryDC);

			for(BYTE i = 0; i < punchCount; i++) {
				if(pPositions[i].Used) {

					// Is position valid
					validPosition = pPositions[i].X != UNKNOWN_COORD_X && pPositions[i].Y != UNKNOWN_COORD_Y ? TRUE : FALSE;

					// Draw ID if in blood, or if position is valid
					drawID = g_pPunchPosManager->GetDrawIdOnlyInBlood() ? pPositions[i].InBlood : validPosition;

					if(drawID) {

						if(g_pPunchPosManager->GetFillBkg()) {
							fillBackground = pPositions[i].InBlood;
						} else {
							fillBackground = FALSE;
						}

						if(fillBackground) {
							// When background is filled, we need to set text background to same as the background color
							::SetBkColor(m_hMemoryDC, pPositions[i].FillColor);
							::SetBkMode(m_hMemoryDC, OPAQUE);
						} else {
							// Background is transparent
							::SetBkMode(m_hMemoryDC, TRANSPARENT);
						}

						::SetTextColor(m_hMemoryDC, pPositions[i].TextColor);

						rad = pPositions[i].Radius;
						_stprintf(szID, _T("%d\0"), pPositions[i].ID);
						double sizeFactor = pPositions[i].ID < 10 ? 0.7 : 0.6;

						pt.x = 0;
						pt.y = pPositions[i].Y;
						ConvertToGDIZpl(pt, _pt);

						t = _pt.y - rad;

						rc.top      = t;
						rc.left     = pPositions[i].X - rad;
						rc.right    = pPositions[i].X + rad;
						rc.bottom   = t + rad + rad;

						// Adjust the font size
						int fontHeight = rad;
						if (m_ImageWidth > m_rcClientRect.right) {
							fontHeight = (int)round(rad * sizeFactor * (double)m_ImageWidth / (double)m_rcClientRect.right);
						}
						HFONT hFont = CreateFont(-fontHeight, 0, 0, 0, FW_NORMAL, false, false, false, DEFAULT_CHARSET,
							OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, NULL);
						HFONT oldFont = (HFONT) ::SelectObject(m_hMemoryDC, hFont);

						::DrawText(m_hMemoryDC, szID, -1, &rc, DT_SINGLELINE | DT_VCENTER | DT_CENTER);

						::SelectObject(m_hMemoryDC, oldFont);
						::DeleteObject(hFont);
					}
				}
			}

			::SetBkMode(m_hMemoryDC, oldBkMode);
			::SetTextColor(m_hMemoryDC, oldTextColor);
			::SetBkColor(m_hMemoryDC, oldBkColor);
		}

		// Release temp variaple
		ReleasePunchPositions(pPositions);
	}

}

void DSCameraRenderer::FrameProcessingSkipped() {

	DWORD dwCurrentTime = GetTickCount();
	if(m_FrameSkippingStartTime == 0) {
		// "First" frame
		m_FrameSkippingStartTime = dwCurrentTime;
	} else {
		if(dwCurrentTime < m_FrameSkippingStartTime) {
			// System timer wraps in 49.7 days..i don¥t think we will ever be here- but if we do
			// get here..we don¥t do anything: just start measuring from scratch
			m_FrameSkippingStartTime = dwCurrentTime;
		} else {
			if(dwCurrentTime - m_FrameSkippingStartTime >= m_FrameSkippedNotificationLimit) {
				// Notify client that too many frames has been skipped
				m_FrameSkippingStartTime = 0;
				g_pPunchPosManager->NotifyFramesSkippedListener();
			}
		}
	}

}

void DSCameraRenderer::DrawBlood(PBYTE pImageData) {

	if(m_DrawBlood) {
		POINT tl  = m_pBloodDetector->GetSearchAreaTopLeft();
		POINT br  = m_pBloodDetector->GetSearchAreaBottomRight();

		PSPOTLIGHTCOLOR pCol = (PSPOTLIGHTCOLOR)pImageData;
		PSPOTLIGHTCOLOR p;
		int tmp;

		for(int y = tl.y; y < br.y; y++) {

			tmp = y * m_ImageWidth;
			for(int x = tl.x; x < br.x; x++) {

				if(m_pBloodDetector->IsBlood(x,y)) {
					p = &(pCol[tmp + x]);
					p->b = m_BloodColorB;
					p->g = m_BloodColorG;
					p->r = m_BloodColorR;
				}
			}
		}
	}

}

void DSCameraRenderer::ThrowAlreadyInitializedException(const PTCHAR pszMethod)
{
	if(m_bInitialized) {
		throw new ExceptionInfo(CLASSNAME, pszMethod, ERR_DSCAMERARENDERER_ALREADY_INITIALIZED);
	}
}

void DSCameraRenderer::ThrowNotInitializedException(const PTCHAR pszMethod)
{
	if(!m_bInitialized) {
		throw new ExceptionInfo(CLASSNAME, pszMethod, ERR_DSCAMERARENDERER_NOT_INITIALIZED);
	}
}

void DSCameraRenderer::CreateHotspots(const PBYTE pImage) {

	// Create hotspots, which are used to initiate barcode reading, and to notify that image contains card
	// (= has changed)

	if(m_pHotspots == NULL) {
		if(m_HotspotCount > 0) {

			m_pHotspots = new Hotspot[m_HotspotCount];
			USHORT t    = (USHORT)(m_HotspotSize.cy / 2);

			POINT tl;
			tl.y = (m_HotspotOffsetY - t);;

			POINT br;
			br.y = (m_HotspotOffsetY + t);

			USHORT radius = (USHORT)(m_HotspotSize.cx / 2);

			USHORT xInc   = m_ImageWidth / m_HotspotCount;
			USHORT x      = xInc / 2;

			// TODO: Y koordinaatti on v‰‰rin - se on kuvan yl‰reunasta vaikka ideana oli ettÅEse 
			//       olis sen alareunasta

			for(t = 0; t < m_HotspotCount; t++) {
				tl.x = x - radius;
				br.x = x + radius;

				m_pHotspots[t].Create(&tl, &br, &m_HotspotColorOffsets, m_ImageType, m_HotspotAllowedInvalidPixelPercent,
					m_ImageWidth, m_HotspotDetectionAccuracyX, m_HotspotDetectionAccuracyY, pImage);

				x += xInc;
			}
		}
	}
}

void DSCameraRenderer::ReleaseHotspots() {

	if(m_pHotspots != NULL) {
		delete []m_pHotspots;
		m_pHotspots = NULL;
	}
}

void DSCameraRenderer::DrawHotspots() {

	// This wont draw searcharea hotspot

	if(m_DrawHotspots) {
		if(m_pHotspots != NULL) {

			// Get currently selected
			HGDIOBJ hOldPen = ::GetCurrentObject(m_hMemoryDC, OBJ_PEN);
			HGDIOBJ hOldBrush = ::GetCurrentObject(m_hMemoryDC, OBJ_BRUSH);


			// Create new
			int penWidth = (int)round((double)1 * (double)m_ImageWidth / (double)m_rcClientRect.right);
			HPEN hPen = ::CreatePen(PS_SOLID, penWidth, m_HotspotColor);
			::SelectObject(m_hMemoryDC, hPen);

			LOGBRUSH lb;
			lb.lbColor = RGB(0, 0, 0);
			lb.lbHatch = 0;
			lb.lbStyle = BS_HOLLOW;

			HBRUSH hBrush = ::CreateBrushIndirect(&lb);
			::SelectObject(m_hMemoryDC, hBrush);


			POINT tl;
			POINT br;

			POINT _tl;
			POINT _br;

			for(BYTE i = 0; i < m_HotspotCount; i ++) {
				tl = m_pHotspots[i].GetTopLeftCorner();
				br = m_pHotspots[i].GetBottomRightCorner();

				ConvertToGDIZpl(tl, _tl);
				ConvertToGDIZpl(br, _br);

				::Rectangle(m_hMemoryDC, _tl.x, _tl.y, _br.x, _br.y);
			}

			// Restore and release
			::SelectObject(m_hMemoryDC, hOldPen);
			::SelectObject(m_hMemoryDC, hOldBrush);

			::DeleteObject(hPen);
			::DeleteObject(hBrush);
		}
	}

}

bool DSCameraRenderer::ReadBarcodes(const PBYTE pImage) {
    _METHODNAME(_T("ReadBarcodes"));

    // Fast preconditions (unchanged behavior, just no verbose logging)
    if (!g_pPunchPosManager->GetReadBarcodesState())
        return false;
    if (m_pBarcodeReader == NULL || pImage == NULL)
        return false;
    if (m_hDCForBCReader == NULL || m_hBMPForBCReader == NULL)
        return false;

    BYTE  punchCount     = g_pPunchPosManager->GetPunchPositionCount();
    DWORD dwCurrentTime  = GetTickCount();

    // Check interval requirement
    if (!m_pBarcodeReader->IsReadRequired(dwCurrentTime, punchCount != 0))
        return false;

    // Draw image to the bitmap used for barcode decoding
    if (SetDIBitsToDevice(m_hDCForBCReader,
                          0, 0,
                          m_ImageWidth, m_ImageHeight,
                          0, 0, 0,
                          m_ImageHeight,
                          pImage,
                          &m_BitmapInfo,
                          DIB_RGB_COLORS) == 0) {
        // Keep failure log (essential)
        TCHAR msg[128];
        _stprintf(msg, _T("SetDIBitsToDevice failed (err=0x%08lX)"), GetLastError());
        LOG_WARNING(CLASSNAME, METHODNAME, msg);
        return false;
    }

    // Perform the read
    m_pBarcodeReader->Read(m_hBMPForBCReader, dwCurrentTime);

    // If barcodes changed, notify and (minimally) log the new state
    if (m_pBarcodeReader->IsBarcodesChanged()) {
        g_pPunchPosManager->NotifyBarcodesUpdatedListener();
        PTCHAR bc = m_pBarcodeReader->GetBarcode();
        if (bc && _tcslen(bc) > 0) {
            PTCHAR type = m_pBarcodeReader->GetBarcodeType();
            if (!type) type = (PTCHAR)_T("Unknown");
            TCHAR msg[256];
            _stprintf(msg, _T("Barcode changed: '%s' (type=%s)"), bc, type);
            LOG_INFO(CLASSNAME, METHODNAME, msg);
        } else {
            LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode cleared"));
        }
    }

    return true;
}

BYTE DSCameraRenderer::CheckHotspots(const PBYTE pImage) {

	if(m_pHotspots != NULL) {
		BYTE invalidHotspots = 0;
		for(BYTE i = 0; i < m_HotspotCount; i++) {
			if(!m_pHotspots[i].IsSame(pImage)) {
				if(!m_AllHotspotsMustBeInvalid) {
					return TRUE;
				}

				invalidHotspots++;
			}
		}

		return (invalidHotspots == m_HotspotCount);
	}

	return FALSE;

}

void DSCameraRenderer::ConvertToGDIZpl(const POINT& ptIn, POINT& ptOut) {

	ptOut.y = m_ImageHeight - ptIn.y;
	ptOut.x = ptIn.x;

}

PBYTE DSCameraRenderer::FlipImage(const PBYTE pImage) {
	if(m_FlipHorizontally || m_FlipVertically) {

		int sourceX;
		int sourceY;

		PSPOTLIGHTCOLOR pSource = (PSPOTLIGHTCOLOR)pImage;
		PSPOTLIGHTCOLOR pTarget = (PSPOTLIGHTCOLOR)m_ImageFlipBuffer;

		SPOTLIGHTCOLOR col;

		if(m_FlipHorizontally && m_FlipVertically) {
			// Flip horizontally and vertically

			for(sourceX = 0; sourceX < m_ImageWidth; sourceX++) {
				for(sourceY = 0; sourceY < m_ImageHeight; sourceY++) {
					col = pSource[(sourceY * m_ImageWidth) + sourceX];
					pTarget[((m_ImageHeight - 1 - sourceY) * m_ImageWidth) + (m_ImageWidth - 1 - sourceX)] = col;
				}
			}

		} else {
			if(m_FlipVertically) {
				// Flip vertically

				for(sourceX = 0; sourceX < m_ImageWidth; sourceX++) {
					for(sourceY = 0; sourceY < m_ImageHeight; sourceY++) {
						col = pSource[(sourceY * m_ImageWidth) + sourceX];
						pTarget[((m_ImageHeight - 1 - sourceY) * m_ImageWidth) + sourceX] = col;
					}
				}

			} else {
				// Flip horizontally

				for(sourceX = 0; sourceX < m_ImageWidth; sourceX++) {
					for(sourceY = 0; sourceY < m_ImageHeight; sourceY++) {
						col = pSource[(sourceY * m_ImageWidth) + sourceX];
						pTarget[(sourceY * m_ImageWidth) + (m_ImageWidth - 1 - sourceX)] = col;
					}
				}
			}
		}

		return m_ImageFlipBuffer;
	}

	return pImage;

}

void DSCameraRenderer::AllocateFlipBuffer() {

	ReleaseFlipBuffer();
	if(m_ImageWidth > 0 && m_ImageHeight > 0) {
		m_ImageFlipBuffer = new BYTE[(m_ImageWidth * m_ImageHeight) * sizeof(SPOTLIGHTCOLOR)];
	}

}

void DSCameraRenderer::ReleaseFlipBuffer() {

	if(m_ImageFlipBuffer != NULL) {
		delete []m_ImageFlipBuffer;
		m_ImageFlipBuffer = NULL;
	}

}

BYTE DSCameraRenderer::InPunchProgressMode() {

	return (m_DisplayImageBuffer != NULL && g_pPunchPosManager->GetInPunchProgressMode());

}

void DSCameraRenderer::SetPunchBorder(BYTE border) {
	m_pPunchDetector->SetPunchBorder(border);
}

BYTE DSCameraRenderer::GetPunchBorder() {
	return m_pPunchDetector->GetPunchBorder();
}

void DSCameraRenderer::SetBloodDetectionOn(bool value) {
	m_bloodDetectionIsOn = value;
}

bool DSCameraRenderer::GetBloodDetectionOn() {
	return m_bloodDetectionIsOn;
}
