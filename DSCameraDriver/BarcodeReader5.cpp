// --------------------------------------------------------------------------
// BarcodeReader.cpp
// --------------------------------------------------------------------------
//
// SpotLight barcode reader implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 03.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#include "stdafx.h"

#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>

#include <sstream>

#include "Windows.h"
#include "BarcodeReader.h"
#include "ExceptionInfo.h"
#include "SimpleLogger.h"
// DTK barcode reader interface
#include "DTKBarReader5.h" 

// DTK5 function pointer types
typedef BARREADER(DTKBARAPI* PFN_BarcodeReader_Create)(PageProcessingCompletedCallback);
typedef void (DTKBARAPI* PFN_BarcodeReader_Destroy)(BARREADER);
typedef BARREADERRESULT(DTKBARAPI* PFN_BarcodeReader_ReadFromImageBuffer)(BARREADER, void*, int, int, int, PixelFormatEnum);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_BarcodeTypes)(BARREADER, BarcodeTypeEnum);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_Orientation)(BARREADER, BarcodeOrientationEnum);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_ScanRectangleX)(BARREADER, int);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_ScanRectangleY)(BARREADER, int);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_ScanRectangleWidth)(BARREADER, int);
typedef void (DTKBARAPI* PFN_BarcodeReader_set_ScanRectangleHeight)(BARREADER, int);
typedef int (DTKBARAPI* PFN_BarReaderResult_get_BarcodesCount)(BARREADERRESULT);
typedef BARCODE(DTKBARAPI* PFN_BarReaderResult_get_Barcode)(BARREADERRESULT, int);
typedef void (DTKBARAPI* PFN_BarReaderResult_Destroy)(BARREADERRESULT);
typedef int (DTKBARAPI* PFN_Barcode_get_Text)(BARCODE, char*, int);
typedef BarcodeTypeEnum(DTKBARAPI* PFN_Barcode_get_Type)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_Threshold)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_Checksum)(BARCODE);
typedef float (DTKBARAPI* PFN_Barcode_get_Angle)(BARCODE);
typedef BarcodeOrientationEnum(DTKBARAPI* PFN_Barcode_get_Orientation)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_TypeString)(BARCODE, char*, int);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderStartY1)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderStartY2)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderEndY1)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderEndY2)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderStartX1)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderStartX2)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderEndX1)(BARCODE);
typedef int (DTKBARAPI* PFN_Barcode_get_BorderEndX2)(BARCODE);
typedef void (DTKBARAPI* PFN_Barcode_Destroy)(BARCODE);
typedef int (DTKBARAPI* PFN_BarcodeReader_ActivateLicenseOnline)(const char*, const char*);

// --------------------------------------------------------------------------

// Class name, used as a param to exception methods
_CLASSNAME(_T("BarcodeReader"));

// --------------------------------------------------------------------------


BarcodeReader::BarcodeReader() {

    m_BcOrientation = 0;
    m_BarcodesChanged = FALSE;;

    m_hDTKReader = NULL;
    m_pDTKDCReader = NULL;
    m_pszReaderLocation = NULL;

    m_readBarcode = nullptr;
    m_dwLastReadTime = 0;

    m_usBarcodeReadingInterval1 = 0;
    m_usBarcodeReadingInterval2 = 0;

    m_enableScanRect[0] = false;
    m_scanRectLeft[0] = 0;
    m_scanRectTop[0] = 0;
    m_scanRectRight[0] = 0;
    m_scanRectBottom[0] = 0;

    m_enableScanRect[1] = false;
    m_scanRectLeft[1] = 0;
    m_scanRectTop[1] = 0;
    m_scanRectRight[1] = 0;
    m_scanRectBottom[1] = 0;

    m_enableScanRect[2] = false;
    m_scanRectLeft[2] = 0;
    m_scanRectTop[2] = 0;
    m_scanRectRight[2] = 0;
    m_scanRectBottom[2] = 0;

    m_barcodeBufferLength = 0;
    m_minimumBarcodeHeight = 0;
    m_nextBufferItem = 0;  // Initialize this too

    // INITIALIZE THE BARCODE BUFFER ARRAY TO nullptr
    for (int i = 0; i < BARCODE_BUFFER_MAX_LENGTH; i++) {
        m_barcodeBuffer[i] = nullptr;
    }

    m_inStandby = false;

    // Initialize DTK5 function pointers
    m_pfnBarcodeReader_Create = NULL;
    m_pfnBarcodeReader_Destroy = NULL;
    m_pfnBarcodeReader_ReadFromImageBuffer = NULL;
    m_pfnBarcodeReader_set_BarcodeTypes = NULL;
    m_pfnBarcodeReader_set_Orientation = NULL;
    m_pfnBarcodeReader_set_ScanRectangleX = NULL;
    m_pfnBarcodeReader_set_ScanRectangleY = NULL;
    m_pfnBarcodeReader_set_ScanRectangleWidth = NULL;
    m_pfnBarcodeReader_set_ScanRectangleHeight = NULL;
    m_pfnBarReaderResult_get_BarcodesCount = NULL;
    m_pfnBarReaderResult_get_Barcode = NULL;
    m_pfnBarReaderResult_Destroy = NULL;
    m_pfnBarcode_get_Text = NULL;
    m_pfnBarcode_get_Type = NULL;
    m_pfnBarcode_get_Threshold = NULL;
    m_pfnBarcode_get_Checksum = NULL;
    m_pfnBarcode_get_Angle = NULL;
    m_pfnBarcode_get_Orientation = NULL;
    m_pfnBarcode_get_TypeString = NULL;
    m_pfnBarcode_get_BorderStartY1 = NULL;
    m_pfnBarcode_get_BorderStartY2 = NULL;
    m_pfnBarcode_get_BorderEndY1 = NULL;
    m_pfnBarcode_get_BorderEndY2 = NULL;
    m_pfnBarcode_get_BorderStartX1 = NULL;
    m_pfnBarcode_get_BorderStartX2 = NULL;
    m_pfnBarcode_get_BorderEndX1 = NULL;
    m_pfnBarcode_get_BorderEndX2 = NULL;
    m_pfnBarcode_Destroy = NULL;
    m_pfnBarcodeReader_ActivateLicenseOnline = NULL;

}

BarcodeReader::~BarcodeReader() {

    Release();

}

void BarcodeReader::Initialize(PBARCODEREADERSETTINGS pSettings) {

    TCHAR msgBuf[128];

    _METHODNAME(_T("Initialize"));
    if (pSettings == NULL) {
        // NULL object
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_SETTINGS_IS_NULL);
    }

    if (pSettings->pszReaderBinaryLocation == NULL || _tcslen(pSettings->pszReaderBinaryLocation) == 0) {
        // We need the reader binary location
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_READER_LOCATION_IS_MISSING);
    }

    // Cache settings
    CopyString(pSettings->pszReaderBinaryLocation, m_pszReaderLocation);
    m_usBarcodeReadingInterval1 = pSettings->usBarcodeReadingInterval1;
    m_usBarcodeReadingInterval2 = pSettings->usBarcodeReadingInterval2;
    m_BcOrientation = pSettings->BcOrientation;

    m_enableScanRect[0] = pSettings->enableScanRect1 != 0;
    m_scanRectLeft[0] = pSettings->scanRect1Left;
    m_scanRectTop[0] = pSettings->scanRect1Top;
    m_scanRectRight[0] = pSettings->scanRect1Right;
    m_scanRectBottom[0] = pSettings->scanRect1Bottom;

    m_enableScanRect[1] = pSettings->enableScanRect2 != 0;
    m_scanRectLeft[1] = pSettings->scanRect2Left;
    m_scanRectTop[1] = pSettings->scanRect2Top;
    m_scanRectRight[1] = pSettings->scanRect2Right;
    m_scanRectBottom[1] = pSettings->scanRect2Bottom;

    m_enableScanRect[2] = pSettings->enableScanRect3 != 0;
    m_scanRectLeft[2] = pSettings->scanRect3Left;
    m_scanRectTop[2] = pSettings->scanRect3Top;
    m_scanRectRight[2] = pSettings->scanRect3Right;
    m_scanRectBottom[2] = pSettings->scanRect3Bottom;

    for (int i = 0; i < 3; i++) {
        _stprintf(msgBuf, _T("R%u:%u (%u,%u) (%u,%u)"), i, m_enableScanRect[i] ? 1 : 0, m_scanRectLeft[i], m_scanRectTop[i], m_scanRectRight[i], m_scanRectBottom[i]);
        LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
    }

    // Init barcode buffer and buffer length.
    m_barcodeBufferLength = pSettings->barcodeRedundancy;
    if (m_barcodeBufferLength > BARCODE_BUFFER_MAX_LENGTH) {
        m_barcodeBufferLength = BARCODE_BUFFER_MAX_LENGTH;
    }
    ClearBarcodeBuffer();

    m_minimumBarcodeHeight = pSettings->minimumBarcodeHeight;

    // Load and init reader
    LoadDTKReader(pSettings->pszReaderBinaryLocation);
    InitializeDTKReader(pSettings);

}

void BarcodeReader::Release() {

    ReleaseString(m_pszReaderLocation);

    // Clear barcode buffer BEFORE releasing DTK reader
    ClearBarcodeBuffer();

    if (m_pDTKDCReader != NULL && m_pfnBarcodeReader_Destroy != NULL) {
        // Release DTK reader
        m_pfnBarcodeReader_Destroy(m_pDTKDCReader);
        m_pDTKDCReader = NULL;
    }

    if (m_hDTKReader != NULL) {
        // Unload DTK reader DLL
        FreeLibrary(m_hDTKReader);
        m_hDTKReader = NULL;
    }

    // Clear function pointers
    m_pfnBarcodeReader_Create = NULL;
    m_pfnBarcodeReader_Destroy = NULL;
    m_pfnBarcodeReader_ReadFromImageBuffer = NULL;
    m_pfnBarcodeReader_set_BarcodeTypes = NULL;
    m_pfnBarcodeReader_set_Orientation = NULL;
    m_pfnBarcodeReader_set_ScanRectangleX = NULL;
    m_pfnBarcodeReader_set_ScanRectangleY = NULL;
    m_pfnBarcodeReader_set_ScanRectangleWidth = NULL;
    m_pfnBarcodeReader_set_ScanRectangleHeight = NULL;
    m_pfnBarReaderResult_get_BarcodesCount = NULL;
    m_pfnBarReaderResult_get_Barcode = NULL;
    m_pfnBarReaderResult_Destroy = NULL;
    m_pfnBarcode_get_Text = NULL;
    m_pfnBarcode_get_Type = NULL;
    m_pfnBarcode_get_Threshold = NULL;
    m_pfnBarcode_get_Checksum = NULL;
    m_pfnBarcode_get_Angle = NULL;
    m_pfnBarcode_get_Orientation = NULL;
    m_pfnBarcode_get_TypeString = NULL;
    m_pfnBarcode_get_BorderStartY1 = NULL;
    m_pfnBarcode_get_BorderStartY2 = NULL;
    m_pfnBarcode_get_BorderEndY1 = NULL;
    m_pfnBarcode_get_BorderEndY2 = NULL;
    m_pfnBarcode_get_BorderStartX1 = NULL;
    m_pfnBarcode_get_BorderStartX2 = NULL;
    m_pfnBarcode_get_BorderEndX1 = NULL;
    m_pfnBarcode_get_BorderEndX2 = NULL;
    m_pfnBarcode_Destroy = NULL;
    m_pfnBarcodeReader_ActivateLicenseOnline = NULL;

}

void BarcodeReader::GetCurrentSettings(PBARCODEREADERSETTINGS pSettings) {

    _METHODNAME(_T("GetCurrentSettings"));
    ThrowNotInitializedException(METHODNAME);

    try {

        pSettings->maxBarcodeCount = 1; // Current implementation supports reading of only one barcode
        pSettings->usBarcodeReadingInterval1 = m_usBarcodeReadingInterval1;
        pSettings->usBarcodeReadingInterval2 = m_usBarcodeReadingInterval2;
        pSettings->BcOrientation = m_BcOrientation;

        pSettings->enableScanRect1 = m_enableScanRect[0] ? 1 : 0;
        pSettings->scanRect1Left = m_scanRectLeft[0];
        pSettings->scanRect1Top = m_scanRectTop[0];
        pSettings->scanRect1Right = m_scanRectRight[0];
        pSettings->scanRect1Bottom = m_scanRectBottom[0];

        pSettings->enableScanRect2 = m_enableScanRect[1] ? 1 : 0;
        pSettings->scanRect2Left = m_scanRectLeft[1];
        pSettings->scanRect2Top = m_scanRectTop[1];
        pSettings->scanRect2Right = m_scanRectRight[1];
        pSettings->scanRect2Bottom = m_scanRectBottom[1];

        pSettings->enableScanRect3 = m_enableScanRect[2] ? 1 : 0;
        pSettings->scanRect3Left = m_scanRectLeft[2];
        pSettings->scanRect3Top = m_scanRectTop[2];
        pSettings->scanRect3Right = m_scanRectRight[2];
        pSettings->scanRect3Bottom = m_scanRectBottom[2];

        pSettings->minimumBarcodeHeight = m_minimumBarcodeHeight;
        pSettings->barcodeRedundancy = m_barcodeBufferLength;

        CopyString(m_pszReaderLocation, pSettings->pszReaderBinaryLocation);

    }
    catch (ExceptionInfo* pEX) {
        throw pEX;
    }
    catch (...) {
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_GETSETTINGS_FAILED);
    }

}

void BarcodeReader::UpdateSettings(PBARCODEREADERSETTINGS pSettings) {

    _METHODNAME(_T("UpdateSettings"));
    ThrowNotInitializedException(METHODNAME);

    m_usBarcodeReadingInterval1 = pSettings->usBarcodeReadingInterval1;
    m_usBarcodeReadingInterval2 = pSettings->usBarcodeReadingInterval2;
    m_BcOrientation = pSettings->BcOrientation;

    m_enableScanRect[0] = pSettings->enableScanRect1 != 0;
    m_scanRectLeft[0] = pSettings->scanRect1Left;
    m_scanRectTop[0] = pSettings->scanRect1Top;
    m_scanRectRight[0] = pSettings->scanRect1Right;
    m_scanRectBottom[0] = pSettings->scanRect1Bottom;

    m_enableScanRect[1] = pSettings->enableScanRect2 != 0;
    m_scanRectLeft[1] = pSettings->scanRect2Left;
    m_scanRectTop[1] = pSettings->scanRect2Top;
    m_scanRectRight[1] = pSettings->scanRect2Right;
    m_scanRectBottom[1] = pSettings->scanRect2Bottom;

    m_enableScanRect[2] = pSettings->enableScanRect3 != 0;
    m_scanRectLeft[2] = pSettings->scanRect3Left;
    m_scanRectTop[2] = pSettings->scanRect3Top;
    m_scanRectRight[2] = pSettings->scanRect3Right;
    m_scanRectBottom[2] = pSettings->scanRect3Bottom;

    m_minimumBarcodeHeight = pSettings->minimumBarcodeHeight;
    m_barcodeBufferLength = pSettings->barcodeRedundancy;
    if (m_barcodeBufferLength > BARCODE_BUFFER_MAX_LENGTH) {
        m_barcodeBufferLength = BARCODE_BUFFER_MAX_LENGTH;
    }

    SetBcOrientation();

}

void BarcodeReader::Read(HBITMAP hBitmap, DWORD dwCurrentTime) {
    _METHODNAME(_T("Read"));
    ThrowNotInitializedException(METHODNAME);

    // ADD ENTRY LOGGING TO CONFIRM METHOD IS CALLED
    LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() method called ***"));

    // Declare msgBuf at function scope for exception handlers
    TCHAR msgBuf[512];

    try {
        BARREADERRESULT hResult = NULL;
        int barcodesCount = 0;

        m_BarcodesChanged = FALSE;
        m_dwLastReadTime = dwCurrentTime;

        // Validate inputs
        if (hBitmap == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("hBitmap is NULL"));
            return;
        }

        // Log bitmap information
        BITMAP bm;
        if (GetObject(hBitmap, sizeof(BITMAP), &bm) == 0) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to get bitmap object"));
            return;
        }

        // ADD BITMAP INFO LOGGING WITH BOUNDS CHECKING
        if (bm.bmWidth > 0 && bm.bmHeight > 0 && bm.bmWidth < 10000 && bm.bmHeight < 10000) {
            _stprintf_s(msgBuf, _countof(msgBuf), _T("Processing bitmap: %dx%d, %d bpp"), bm.bmWidth, bm.bmHeight, bm.bmBitsPixel);
            LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
        } else {
            _stprintf_s(msgBuf, _countof(msgBuf), _T("Invalid bitmap dimensions: %dx%d"), bm.bmWidth, bm.bmHeight);
            LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
            return;
        }

        // Check if function pointers are loaded
        if (!m_pfnBarcodeReader_ReadFromImageBuffer) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("BarcodeReader_ReadFromImageBuffer function pointer is NULL"));
            return;
        }

        // Get DIB data from bitmap
        HDC hdc = GetDC(NULL);
        if (hdc == NULL) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to get device context"));
            return;
        }

        // ALWAYS USE 24-BIT RGB
        BITMAPINFO bi = { 0 };
        bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
        bi.bmiHeader.biWidth = bm.bmWidth;
        bi.bmiHeader.biHeight = -bm.bmHeight; // negative for top-down
        bi.bmiHeader.biPlanes = 1;
        bi.bmiHeader.biBitCount = 24;
        bi.bmiHeader.biCompression = BI_RGB;

        int stride = ((bm.bmWidth * 3 + 3) & ~3); // 4-byte alignment for 24-bit
        int imageSize = stride * bm.bmHeight;
        
        // ADD BOUNDS CHECKING FOR IMAGE SIZE
        if (imageSize <= 0 || imageSize > 50000000) { // Max ~50MB
            _stprintf_s(msgBuf, _countof(msgBuf), _T("Invalid image size calculated: %d bytes"), imageSize);
            LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
            ReleaseDC(NULL, hdc);
            return;
        }

        _stprintf_s(msgBuf, _countof(msgBuf), _T("Image buffer: %dx%d, stride=%d, size=%d bytes"), bm.bmWidth, bm.bmHeight, stride, imageSize);
        LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

        void* pImageData = malloc(imageSize);
        if (pImageData == NULL) {
            ReleaseDC(NULL, hdc);
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to allocate image buffer"));
            return;
        }

        // ZERO THE BUFFER TO PREVENT GARBAGE DATA
        memset(pImageData, 0, imageSize);

        if (GetDIBits(hdc, hBitmap, 0, bm.bmHeight, pImageData, &bi, DIB_RGB_COLORS) == 0) {
            free(pImageData);
            ReleaseDC(NULL, hdc);
            DWORD dwError = GetLastError();
            _stprintf_s(msgBuf, _countof(msgBuf), _T("GetDIBbits failed with error: %lu"), dwError);
            LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
            return;
        }

        ReleaseDC(NULL, hdc);

        // ADD SCANNING PROGRESS LOGGING
        LOG_INFO(CLASSNAME, METHODNAME, _T("Starting barcode scan with DTK5 reader"));

        // TRY SCANNING THE FULL IMAGE FIRST BEFORE USING RECTANGLES
        bool foundWithFullScan = false;
        
        LOG_INFO(CLASSNAME, METHODNAME, _T("Trying full image scan first"));
        
        // Reset scan rectangle to full image
        if (m_pfnBarcodeReader_set_ScanRectangleX && m_pDTKDCReader) {
            m_pfnBarcodeReader_set_ScanRectangleX(m_pDTKDCReader, 0);
        }
        if (m_pfnBarcodeReader_set_ScanRectangleY && m_pDTKDCReader) {
            m_pfnBarcodeReader_set_ScanRectangleY(m_pDTKDCReader, 0);
        }
        if (m_pfnBarcodeReader_set_ScanRectangleWidth && m_pDTKDCReader) {
            m_pfnBarcodeReader_set_ScanRectangleWidth(m_pDTKDCReader, bm.bmWidth);
        }
        if (m_pfnBarcodeReader_set_ScanRectangleHeight && m_pDTKDCReader) {
            m_pfnBarcodeReader_set_ScanRectangleHeight(m_pDTKDCReader, bm.bmHeight);
        }

        LOG_INFO(CLASSNAME, METHODNAME, _T("Calling DTK5 BarcodeReader_ReadFromImageBuffer for full image"));

        // Try full image scan with correct pixel format
        hResult = NULL;
        if (m_pfnBarcodeReader_ReadFromImageBuffer && m_pDTKDCReader && pImageData) {
            hResult = m_pfnBarcodeReader_ReadFromImageBuffer(m_pDTKDCReader, pImageData, bm.bmWidth, bm.bmHeight, stride, PIXFMT_BGR24); // Use BGR24 for Windows DIB
        }

        if (hResult != NULL) {
            LOG_INFO(CLASSNAME, METHODNAME, _T("DTK5 ReadFromImageBuffer (full image) returned valid result"));
            
            if (m_pfnBarReaderResult_get_BarcodesCount) {
                barcodesCount = m_pfnBarReaderResult_get_BarcodesCount(hResult);
                _stprintf_s(msgBuf, _countof(msgBuf), _T("DTK5 found %d barcodes in full image scan"), barcodesCount);
                LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                
                if (barcodesCount > 0) {
                    foundWithFullScan = true;
                }
            }
        } else {
            LOG_WARNING(CLASSNAME, METHODNAME, _T("DTK5 ReadFromImageBuffer (full image) returned NULL"));
        }

        // IF FULL IMAGE SCAN DIDN'T WORK, TRY THE CONFIGURED RECTANGLES
        if (!foundWithFullScan) {
            if (hResult && m_pfnBarReaderResult_Destroy) {
                m_pfnBarReaderResult_Destroy(hResult);
                hResult = NULL;
            }
            
            LOG_INFO(CLASSNAME, METHODNAME, _T("Full image scan found no barcodes, trying configured rectangles"));
            
            // Try scanning with different rectangles (adapted from old logic)
            for (int rect = 0; rect < 3; rect++) {
                if (m_enableScanRect[rect]) {
                    // LOG EACH SCAN RECTANGLE ATTEMPT WITH BOUNDS CHECKING
                    if (m_scanRectLeft[rect] >= 0 && m_scanRectTop[rect] >= 0 && 
                        m_scanRectRight[rect] > m_scanRectLeft[rect] && 
                        m_scanRectBottom[rect] > m_scanRectTop[rect] &&
                        m_scanRectRight[rect] <= bm.bmWidth && m_scanRectBottom[rect] <= bm.bmHeight) {
                        
                        _stprintf_s(msgBuf, _countof(msgBuf), _T("Scanning rectangle %d: (%d,%d) to (%d,%d)"),
                            rect, m_scanRectLeft[rect], m_scanRectTop[rect], m_scanRectRight[rect], m_scanRectBottom[rect]);
                        LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                    } else {
                        _stprintf_s(msgBuf, _countof(msgBuf), _T("Invalid scan rectangle %d: (%d,%d) to (%d,%d) - skipping"),
                            rect, m_scanRectLeft[rect], m_scanRectTop[rect], m_scanRectRight[rect], m_scanRectBottom[rect]);
                        LOG_WARNING(CLASSNAME, METHODNAME, msgBuf);
                        continue;
                    }

                    // Set scanning rectangle using DTK5 API with validation
                    if (m_pfnBarcodeReader_set_ScanRectangleX && m_pDTKDCReader) {
                        m_pfnBarcodeReader_set_ScanRectangleX(m_pDTKDCReader, m_scanRectLeft[rect]);
                    }
                    if (m_pfnBarcodeReader_set_ScanRectangleY && m_pDTKDCReader) {
                        m_pfnBarcodeReader_set_ScanRectangleY(m_pDTKDCReader, m_scanRectTop[rect]);
                    }
                    int width = m_scanRectRight[rect] - m_scanRectLeft[rect];
                    int height = m_scanRectBottom[rect] - m_scanRectTop[rect];
                    
                    if (width > 0 && height > 0) {
                        if (m_pfnBarcodeReader_set_ScanRectangleWidth && m_pDTKDCReader) {
                            m_pfnBarcodeReader_set_ScanRectangleWidth(m_pDTKDCReader, width);
                        }
                        if (m_pfnBarcodeReader_set_ScanRectangleHeight && m_pDTKDCReader) {
                            m_pfnBarcodeReader_set_ScanRectangleHeight(m_pDTKDCReader, height);
                        }
                    }

                    // LOG BEFORE DTK5 CALL
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Calling DTK5 BarcodeReader_ReadFromImageBuffer"));

                    // Try to read from image buffer using DTK5 API
                    hResult = NULL;
                    if (m_pfnBarcodeReader_ReadFromImageBuffer && m_pDTKDCReader && pImageData) {
                        hResult = m_pfnBarcodeReader_ReadFromImageBuffer(m_pDTKDCReader, pImageData, bm.bmWidth, bm.bmHeight, stride, PIXFMT_BGR24);
                    }

                    // LOG RESULT OF DTK5 CALL
                    if (hResult != NULL) {
                        LOG_INFO(CLASSNAME, METHODNAME, _T("DTK5 ReadFromImageBuffer returned valid result"));
                    }
                    else {
                        LOG_WARNING(CLASSNAME, METHODNAME, _T("DTK5 ReadFromImageBuffer returned NULL"));
                    }

                    if (hResult != NULL && m_pfnBarReaderResult_get_BarcodesCount) {
                        // Get the number of read barcodes and quit if we got at least one.
                        barcodesCount = m_pfnBarReaderResult_get_BarcodesCount(hResult);

                        // LOG BARCODE COUNT
                        _stprintf_s(msgBuf, _countof(msgBuf), _T("DTK5 found %d barcodes in rectangle %d"), barcodesCount, rect);
                        LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

                        if (barcodesCount > 0) {
                            LOG_INFO(CLASSNAME, METHODNAME, _T("Barcodes found - stopping scan"));
                            break;
                        }
                        // Clean up result if no barcodes found
                        if (m_pfnBarReaderResult_Destroy && hResult) {
                            m_pfnBarReaderResult_Destroy(hResult);
                            hResult = NULL;
                        }
                    }
                    else {
                        LOG_WARNING(CLASSNAME, METHODNAME, _T("Could not get barcode count from DTK5 result"));
                    }
                }
                else {
                    // LOG SKIPPED RECTANGLES
                    _stprintf_s(msgBuf, _countof(msgBuf), _T("Scan rectangle %d is disabled - skipping"), rect);
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                }
            }
        }

        // ADD IMAGE DEBUGGING - SAVE FIRST FEW BYTES TO LOG
        if (pImageData) {
            unsigned char* imageBytes = (unsigned char*)pImageData;
            _stprintf_s(msgBuf, _countof(msgBuf), _T("Image data sample (first 12 bytes): %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X"), 
                imageBytes[0], imageBytes[1], imageBytes[2], imageBytes[3], 
                imageBytes[4], imageBytes[5], imageBytes[6], imageBytes[7],
                imageBytes[8], imageBytes[9], imageBytes[10], imageBytes[11]);
            LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
            
            // Check if image data is all zeros (would indicate a problem)
            bool allZeros = true;
            for (int i = 0; i < min(1000, imageSize); i++) {
                if (imageBytes[i] != 0) {
                    allZeros = false;
                    break;
                }
            }
            if (allZeros) {
                LOG_WARNING(CLASSNAME, METHODNAME, _T("WARNING: Image data appears to be all zeros - this may indicate a bitmap conversion problem"));
            }
        }

        // LOG FINAL SCAN RESULTS
        if (barcodesCount > 0 && hResult != NULL) {
            _stprintf_s(msgBuf, _countof(msgBuf), _T("Barcode scan completed successfully - processing %d barcodes"), barcodesCount);
            LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

            // Get the first barcode
            BARCODE pBar = NULL;
            if (m_pfnBarReaderResult_get_Barcode) {
                pBar = m_pfnBarReaderResult_get_Barcode(hResult, 0);
            }

            if (pBar != NULL) {
                LOG_INFO(CLASSNAME, METHODNAME, _T("Successfully retrieved barcode object from DTK5"));

                // Get barcode text
                char barcodeText[256] = { 0 };
                if (m_pfnBarcode_get_Text && m_pfnBarcode_get_Text(pBar, barcodeText, sizeof(barcodeText)) > 0) {

                    // LOG BARCODE TEXT IMMEDIATELY
                    _stprintf_s(msgBuf, _countof(msgBuf), _T("Raw barcode text retrieved: '%S'"), barcodeText);
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

                    // Get barcode properties for debug output
                    int threshold = 0, checksum = 0;
                    float angle = 0.0f;
                    char typeString[64] = { 0 };
                    BarcodeOrientationEnum orientation = BO_Unknown;
                    BarcodeTypeEnum barcodeType = BT_Unknown;

                    if (m_pfnBarcode_get_Threshold) threshold = m_pfnBarcode_get_Threshold(pBar);
                    if (m_pfnBarcode_get_Checksum) checksum = m_pfnBarcode_get_Checksum(pBar);
                    if (m_pfnBarcode_get_Angle) angle = m_pfnBarcode_get_Angle(pBar);
                    if (m_pfnBarcode_get_TypeString) m_pfnBarcode_get_TypeString(pBar, typeString, sizeof(typeString));
                    if (m_pfnBarcode_get_Orientation) orientation = m_pfnBarcode_get_Orientation(pBar);
                    if (m_pfnBarcode_get_Type) barcodeType = m_pfnBarcode_get_Type(pBar);

                    // Calculate barcode height (adapted from old logic)
                    int start1 = 0, start2 = 0, end1 = 0, end2 = 0, heightStart = 0, heightEnd = 0, barcodeHeight = 0;

                    if ((orientation == BO_LeftToRight) || (orientation == BO_RightToLeft)) {
                        // Horizontal barcodes
                        if (m_pfnBarcode_get_BorderStartY1) start1 = m_pfnBarcode_get_BorderStartY1(pBar);
                        if (m_pfnBarcode_get_BorderStartY2) start2 = m_pfnBarcode_get_BorderStartY2(pBar);
                        if (m_pfnBarcode_get_BorderEndY1) end1 = m_pfnBarcode_get_BorderEndY1(pBar);
                        if (m_pfnBarcode_get_BorderEndY2) end2 = m_pfnBarcode_get_BorderEndY2(pBar);
                    }
                    else {
                        // Vertical barcodes
                        if (m_pfnBarcode_get_BorderStartX1) start1 = m_pfnBarcode_get_BorderStartX1(pBar);
                        if (m_pfnBarcode_get_BorderStartX2) start2 = m_pfnBarcode_get_BorderStartX2(pBar);
                        if (m_pfnBarcode_get_BorderEndX1) end1 = m_pfnBarcode_get_BorderEndX1(pBar);
                        if (m_pfnBarcode_get_BorderEndX2) end2 = m_pfnBarcode_get_BorderEndX2(pBar);
                    }

                    heightStart = abs(start2 - start1);
                    heightEnd = abs(end2 - end1);
                    // Select the smallest value
                    if (heightStart < heightEnd) {
                        barcodeHeight = heightStart;
                    }
                    else {
                        barcodeHeight = heightEnd;
                    }

                    // Debug output with comprehensive barcode information
                    _stprintf_s(msgBuf, _countof(msgBuf), _T("Barcode details: '%S' type:'%S' threshold:%d angle:%.2f checksum:%d height:%d"),
                        barcodeText, typeString, threshold, angle, checksum, barcodeHeight);
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

                    // Check minimum barcode height requirement
                    if (barcodeHeight >= m_minimumBarcodeHeight) {
                        LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode meets minimum height requirement"));

                        // Convert char* to PTCHAR (using MultiByteToWideChar for proper conversion)
                        int len = MultiByteToWideChar(CP_ACP, 0, barcodeText, -1, NULL, 0);
                        if (len > 0 && len < 1000) { // Sanity check
                            PTCHAR barcodeTextW = new TCHAR[len];
                            if (MultiByteToWideChar(CP_ACP, 0, barcodeText, -1, barcodeTextW, len) > 0) {

                                LOG_INFO(CLASSNAME, METHODNAME, _T("Adding barcode to buffer for redundancy check"));
                                SaveToBuffer(barcodeTextW);

                                if (CheckRedundancyOk()) {
                                    LOG_INFO(CLASSNAME, METHODNAME, _T("*** Redundancy check passed - accepting barcode ***"));

                                    if (m_readBarcode == nullptr) {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("No previous barcode - accepting as new"));
                                        // Previously read barcode does not exist, accept the first one
                                        CopyString(barcodeTextW, m_readBarcode);
                                        m_readType = barcodeType;
                                        m_BarcodesChanged = TRUE;

                                        // Clean up
                                        delete[] barcodeTextW;
                                        if (m_pfnBarcode_Destroy) m_pfnBarcode_Destroy(pBar);
                                        if (m_pfnBarReaderResult_Destroy) m_pfnBarReaderResult_Destroy(hResult);
                                        free(pImageData);

                                        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed successfully - new barcode accepted ***"));
                                        return;
                                    }

                                    // Previously read barcode exists, check if current barcode matches
                                    if (_tcscmp(barcodeTextW, m_readBarcode) == 0) {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode matches previous - no change"));
                                        // Clean up
                                        delete[] barcodeTextW;
                                        if (m_pfnBarcode_Destroy) m_pfnBarcode_Destroy(pBar);
                                        if (m_pfnBarReaderResult_Destroy) m_pfnBarReaderResult_Destroy(hResult);
                                        free(pImageData);

                                        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed - barcode unchanged ***"));
                                        return;
                                    }

                                    LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode differs from previous - applying Inter2of5 logic"));

                                    // Handle Interleaved 2 of 5 special logic (adapted from old code)
                                    if (barcodeType != BT_Inter2of5) {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("New barcode is NOT Inter2of5 - safe to accept"));
                                        // New code is NOT interleaved 2 of 5, it is safe to use
                                        ReleaseString(m_readBarcode);
                                        CopyString(barcodeTextW, m_readBarcode);
                                        m_readType = barcodeType;
                                        m_BarcodesChanged = TRUE;

                                        // Clean up
                                        delete[] barcodeTextW;
                                        if (m_pfnBarcode_Destroy) m_pfnBarcode_Destroy(pBar);
                                        if (m_pfnBarReaderResult_Destroy) m_pfnBarReaderResult_Destroy(hResult);
                                        free(pImageData);

                                        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed - new non-Inter2of5 barcode accepted ***"));
                                        return;
                                    }

                                    if (m_readType != BT_Inter2of5) {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("Previous barcode is NOT Inter2of5, new is Inter2of5 - keeping old"));
                                        // The cached type is NOT inter 2 of 5, but the new code is. Keep the old code
                                        delete[] barcodeTextW;
                                        if (m_pfnBarcode_Destroy) m_pfnBarcode_Destroy(pBar);
                                        if (m_pfnBarReaderResult_Destroy) m_pfnBarReaderResult_Destroy(hResult);
                                        free(pImageData);

                                        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed - kept previous non-Inter2of5 barcode ***"));
                                        return;
                                    }

                                    // Both are inter 2 of 5, take the new code if it's longer or equal
                                    if (_tcslen(m_readBarcode) < _tcslen(barcodeTextW)) {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("Both are Inter2of5, new is longer - accepting new"));
                                        ReleaseString(m_readBarcode);
                                        CopyString(barcodeTextW, m_readBarcode);
                                        m_readType = barcodeType;
                                        m_BarcodesChanged = TRUE;

                                        // Clean up
                                        delete[] barcodeTextW;
                                        if (m_pfnBarcode_Destroy) m_pfnBarcode_Destroy(pBar);
                                        if (m_pfnBarReaderResult_Destroy) m_pfnBarReaderResult_Destroy(hResult);
                                        free(pImageData);

                                        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed - longer Inter2of5 barcode accepted ***"));
                                        return;
                                    }
                                    else {
                                        LOG_INFO(CLASSNAME, METHODNAME, _T("Both are Inter2of5, old is longer or equal - keeping old"));
                                    }
                                }
                                else {
                                    LOG_WARNING(CLASSNAME, METHODNAME, _T("Redundancy check failed - barcode not consistent"));
                                }
                            }
                            delete[] barcodeTextW;
                        }
                    }
                    else {
                        _stprintf_s(msgBuf, _countof(msgBuf), _T("Barcode height %d below minimum %d - rejected"), barcodeHeight, m_minimumBarcodeHeight);
                        LOG_WARNING(CLASSNAME, METHODNAME, msgBuf);
                    }
                }
                else {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to get barcode text from DTK5"));
                }

                // Clean up barcode
                if (m_pfnBarcode_Destroy) {
                    m_pfnBarcode_Destroy(pBar);
                }
            }
            else {
                LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to get barcode object from DTK5 result"));
            }

            // Clean up result
            if (m_pfnBarReaderResult_Destroy) {
                m_pfnBarReaderResult_Destroy(hResult);
            }
        }
        else {
            LOG_INFO(CLASSNAME, METHODNAME, _T("No barcodes found in any scan rectangle"));
        }

        // Clean up image data
        free(pImageData);

        LOG_INFO(CLASSNAME, METHODNAME, _T("*** BarcodeReader::Read() completed - no barcode changes ***"));

    }
    catch (ExceptionInfo* pExc) {
        _stprintf_s(msgBuf, _countof(msgBuf), _T("ExceptionInfo caught in Read() - Error code: %d"), pExc->GetErrorCode());
        LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
        throw pExc;
    }
    catch (const std::exception& e) {
        // ADD STD::EXCEPTION HANDLER
        LOG_ERROR(CLASSNAME, METHODNAME, _T("std::exception caught in Read()"));
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_READ_FAILED);
    }
    catch (...) {
        LOG_ERROR(CLASSNAME, METHODNAME, _T("Unknown exception caught in Read()"));
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_READ_FAILED);
    }
}

PTCHAR BarcodeReader::GetBarcodeType() {
    _METHODNAME(_T("GetBarcodeType"));
    ThrowNotInitializedException(METHODNAME);
    // What is weak in this code is that the returned string values below need to be in written exactly same as the enumerators
    // in barcode.cs
    switch (m_readType) {
    case BT_Code39:
        return (PTCHAR)_T("Code39");
    case BT_Inter2of5:
        return (PTCHAR)_T("Interleaved2of5");
    case BT_UPCA:
        return (PTCHAR)_T("UPCA");
    case BT_UPCE:
        return (PTCHAR)_T("UPCE");
    case BT_EAN8:
    case BT_EAN13:
        return (PTCHAR)_T("EAN");
    case BT_Codabar:
        return (PTCHAR)_T("Codabar");
    case BT_Code128:
        return (PTCHAR)_T("Code128");
    case BT_Code93:
        return (PTCHAR)_T("Code93");
    case BT_Code11:
        return (PTCHAR)_T("Code11");
    default:
        return (PTCHAR)_T("Unknown");
    }
}


void BarcodeReader::ClearBarcodes() {

    _METHODNAME(_T("ClearBarcodes"));
    ThrowNotInitializedException(METHODNAME);

    try {
        ReleaseString(m_readBarcode);
        m_readBarcode = nullptr;
        ClearBarcodeBuffer();

    }
    catch (ExceptionInfo* pExc) {
        throw pExc;
    }
    catch (...) {
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_CLEARBARCODES_FAILED);
    }

}

static USHORT m_savedInterval = 10000;

BYTE BarcodeReader::IsReadRequired(DWORD dwCurrentTime, int searchingPunches) {
    if (m_inStandby) {
        // What is problem with DTK reader is that its attempt to prevent unlicenced server use is implemented
        // in a fashion that it interprets our software being server use when the PC running our software is idle 
        // for a long time. When idle for a long time, DTK reader shows a MessageBox in UI, if the barcode reading has
        // been attempted more often than once a second. This is avoided by not reading barcode at all when instrument
        // is in standby.
        return FALSE;
    }

    // System timer will wrap in 49.7 days :)
    if (dwCurrentTime > m_dwLastReadTime) {
        USHORT interval;
        LPCTSTR msg;

        if (searchingPunches != 0) {
            interval = m_usBarcodeReadingInterval2;
            msg = _T("Barcode reading interval 2");
        }
        else {
            interval = m_usBarcodeReadingInterval1;
            msg = _T("Barcode reading interval 1");
        }

        if (interval != m_savedInterval) {
            m_savedInterval = interval;
            LOG_INFO(CLASSNAME, _T("IsReadRequired"), (PTCHAR)msg); 
        }

        return (dwCurrentTime - m_dwLastReadTime) > interval ? TRUE : FALSE;
    }

    return TRUE;
}

void BarcodeReader::LoadDTKReader(PTCHAR pszDTKReaderLocation) {

    if (m_hDTKReader == NULL) {

        _METHODNAME(_T("LoadDTKReader"));
        LOG_INFO(CLASSNAME, METHODNAME, _T("LoadDTKReader method started"));
        BYTE errorOccured = TRUE;

        try {

            LOG_INFO(CLASSNAME, METHODNAME, _T("Validating DTK reader location"));
            if (pszDTKReaderLocation == NULL) {
                LOG_ERROR(CLASSNAME, METHODNAME, _T("DTK reader location is NULL"));
                ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_FAILED_TO_LOAD_READER);
            }

            // LOG THE FULL PATH WE'RE TRYING TO LOAD
            TCHAR msgBuf[512];
            _stprintf(msgBuf, _T("Attempting to load DTK reader from: %s"), pszDTKReaderLocation);
            LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

            // CHECK CURRENT USER PRIVILEGES
            BOOL isAdmin = FALSE;
            HANDLE hToken = NULL;
            if (OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
                TOKEN_ELEVATION elevation;
                DWORD dwSize;
                if (GetTokenInformation(hToken, TokenElevation, &elevation, sizeof(elevation), &dwSize)) {
                    isAdmin = elevation.TokenIsElevated;
                }
                CloseHandle(hToken);
            }
            _stprintf(msgBuf, _T("Running with admin privileges: %s"), isAdmin ? _T("YES") : _T("NO"));
            LOG_INFO(CLASSNAME, METHODNAME, msgBuf);

            // CHECK IF FILE EXISTS AND ACCESS PERMISSIONS
            DWORD fileAttributes = GetFileAttributes(pszDTKReaderLocation);
            if (fileAttributes == INVALID_FILE_ATTRIBUTES) {
                DWORD dwError = GetLastError();
                _stprintf(msgBuf, _T("DTK reader file not found or access denied. Error: %lu"), dwError);
                LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
                
                // Check specific error codes
                if (dwError == ERROR_ACCESS_DENIED) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("ACCESS DENIED - File exists but insufficient permissions"));
                } else if (dwError == ERROR_FILE_NOT_FOUND) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("FILE NOT FOUND"));
                } else if (dwError == ERROR_PATH_NOT_FOUND) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("PATH NOT FOUND"));
                }
                
                ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_FAILED_TO_LOAD_READER);
                return;
            }

            // TEST READ ACCESS TO THE FILE
            HANDLE hFile = CreateFile(pszDTKReaderLocation, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
            if (hFile == INVALID_HANDLE_VALUE) {
                DWORD dwError = GetLastError();
                _stprintf(msgBuf, _T("Cannot open DTK file for reading. Error: %lu"), dwError);
                LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
                
                if (dwError == ERROR_ACCESS_DENIED) {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("READ ACCESS DENIED - Insufficient file permissions"));
                }
                
                ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_FAILED_TO_LOAD_READER);
                return;
            } else {
                LOG_INFO(CLASSNAME, METHODNAME, _T("DTK file opened successfully for reading"));
                CloseHandle(hFile);
            }

            // CHECK DIRECTORY PERMISSIONS OF THE DLL LOCATION
            TCHAR dllDirectory[MAX_PATH];
            _tcscpy_s(dllDirectory, pszDTKReaderLocation);
            PTCHAR lastSlash = _tcsrchr(dllDirectory, _T('\\'));
            if (lastSlash) {
                *lastSlash = _T('\0');
                _stprintf(msgBuf, _T("DTK DLL directory: %s"), dllDirectory);
                LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                
                // Check if we can list files in the directory
                WIN32_FIND_DATA findData;
                TCHAR searchPattern[MAX_PATH];
                _stprintf(searchPattern, _T("%s\\*"), dllDirectory);
                HANDLE hFind = FindFirstFile(searchPattern, &findData);
                if (hFind != INVALID_HANDLE_VALUE) {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Directory listing successful"));
                    FindClose(hFind);
                } else {
                    DWORD dwError = GetLastError();
                    _stprintf(msgBuf, _T("Cannot list directory contents. Error: %lu"), dwError);
                    LOG_WARNING(CLASSNAME, METHODNAME, msgBuf);
                }
            }

            // CHECK FOR DEPENDENCIES IN THE SAME DIRECTORY
            LOG_INFO(CLASSNAME, METHODNAME, _T("Checking for common DTK dependencies"));
            TCHAR depPath[MAX_PATH];
            const PTCHAR commonDeps[] = { 
                _T("msvcr120.dll"), _T("msvcp120.dll"), _T("msvcr140.dll"), _T("msvcp140.dll"),
                _T("vcruntime140.dll"), _T("mfc120u.dll"), _T("mfc140u.dll"), NULL 
            };
            
            for (int i = 0; commonDeps[i] != NULL; i++) {
                _stprintf(depPath, _T("%s\\%s"), dllDirectory, commonDeps[i]);
                if (GetFileAttributes(depPath) != INVALID_FILE_ATTRIBUTES) {
                    _stprintf(msgBuf, _T("Found dependency: %s"), commonDeps[i]);
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                }
            }

            // ENHANCED LOAD ATTEMPT WITH DETAILED ERROR REPORTING
            LOG_INFO(CLASSNAME, METHODNAME, _T("Loading DTK reader DLL"));
            SetLastError(0);

            // Try loading with different flags to diagnose dependency issues
            LOG_INFO(CLASSNAME, METHODNAME, _T("Attempting LoadLibraryEx with LOAD_WITH_ALTERED_SEARCH_PATH"));
            m_hDTKReader = LoadLibraryEx(pszDTKReaderLocation, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
            DWORD loadError = GetLastError();

            if (m_hDTKReader == NULL) {
                _stprintf(msgBuf, _T("LoadLibraryEx with LOAD_WITH_ALTERED_SEARCH_PATH failed. Error: %lu"), loadError);
                LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);

                // Try without flags
                LOG_INFO(CLASSNAME, METHODNAME, _T("Attempting LoadLibrary without flags"));
                SetLastError(0);
                m_hDTKReader = LoadLibrary(pszDTKReaderLocation);
                loadError = GetLastError();
            }

            if (m_hDTKReader != NULL) {
                LOG_INFO(CLASSNAME, METHODNAME, _T("DTK reader DLL loaded successfully"));
                
                // LOG THE MODULE INFORMATION
                TCHAR moduleFileName[MAX_PATH];
                if (GetModuleFileName(m_hDTKReader, moduleFileName, MAX_PATH)) {
                    _stprintf(msgBuf, _T("Loaded module path: %s"), moduleFileName);
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                }

                // Continue with function pointer loading...
                LOG_INFO(CLASSNAME, METHODNAME, _T("Getting function pointers"));

                // Load DTK5 function pointers - COMPLETE THE LOADING
                m_pfnBarcodeReader_Create = (PFN_BarcodeReader_Create)GetProcAddress(m_hDTKReader, "BarcodeReader_Create");
                m_pfnBarcodeReader_Destroy = (PFN_BarcodeReader_Destroy)GetProcAddress(m_hDTKReader, "BarcodeReader_Destroy");
                m_pfnBarcodeReader_ReadFromImageBuffer = (PFN_BarcodeReader_ReadFromImageBuffer)GetProcAddress(m_hDTKReader, "BarcodeReader_ReadFromImageBuffer");
                m_pfnBarcodeReader_set_BarcodeTypes = (PFN_BarcodeReader_set_BarcodeTypes)GetProcAddress(m_hDTKReader, "BarcodeReader_set_BarcodeTypes");
                m_pfnBarcodeReader_set_Orientation = (PFN_BarcodeReader_set_Orientation)GetProcAddress(m_hDTKReader, "BarcodeReader_set_Orientation");
                m_pfnBarcodeReader_set_ScanRectangleX = (PFN_BarcodeReader_set_ScanRectangleX)GetProcAddress(m_hDTKReader, "BarcodeReader_set_ScanRectangleX");
                m_pfnBarcodeReader_set_ScanRectangleY = (PFN_BarcodeReader_set_ScanRectangleY)GetProcAddress(m_hDTKReader, "BarcodeReader_set_ScanRectangleY");
                m_pfnBarcodeReader_set_ScanRectangleWidth = (PFN_BarcodeReader_set_ScanRectangleWidth)GetProcAddress(m_hDTKReader, "BarcodeReader_set_ScanRectangleWidth");
                m_pfnBarcodeReader_set_ScanRectangleHeight = (PFN_BarcodeReader_set_ScanRectangleHeight)GetProcAddress(m_hDTKReader, "BarcodeReader_set_ScanRectangleHeight");
                m_pfnBarReaderResult_get_BarcodesCount = (PFN_BarReaderResult_get_BarcodesCount)GetProcAddress(m_hDTKReader, "BarReaderResult_get_BarcodesCount");
                m_pfnBarReaderResult_get_Barcode = (PFN_BarReaderResult_get_Barcode)GetProcAddress(m_hDTKReader, "BarReaderResult_get_Barcode");
                m_pfnBarReaderResult_Destroy = (PFN_BarReaderResult_Destroy)GetProcAddress(m_hDTKReader, "BarReaderResult_Destroy");
                m_pfnBarcode_get_Text = (PFN_Barcode_get_Text)GetProcAddress(m_hDTKReader, "Barcode_get_Text");
                m_pfnBarcode_get_Type = (PFN_Barcode_get_Type)GetProcAddress(m_hDTKReader, "Barcode_get_Type");
                m_pfnBarcode_get_Threshold = (PFN_Barcode_get_Threshold)GetProcAddress(m_hDTKReader, "Barcode_get_Threshold");
                m_pfnBarcode_get_Checksum = (PFN_Barcode_get_Checksum)GetProcAddress(m_hDTKReader, "Barcode_get_Checksum");
                m_pfnBarcode_get_Angle = (PFN_Barcode_get_Angle)GetProcAddress(m_hDTKReader, "Barcode_get_Angle");
                m_pfnBarcode_get_Orientation = (PFN_Barcode_get_Orientation)GetProcAddress(m_hDTKReader, "Barcode_get_Orientation");
                m_pfnBarcode_get_TypeString = (PFN_Barcode_get_TypeString)GetProcAddress(m_hDTKReader, "Barcode_get_TypeString");
                m_pfnBarcode_get_BorderStartY1 = (PFN_Barcode_get_BorderStartY1)GetProcAddress(m_hDTKReader, "Barcode_get_BorderStartY1");
                m_pfnBarcode_get_BorderStartY2 = (PFN_Barcode_get_BorderStartY2)GetProcAddress(m_hDTKReader, "Barcode_get_BorderStartY2");
                m_pfnBarcode_get_BorderEndY1 = (PFN_Barcode_get_BorderEndY1)GetProcAddress(m_hDTKReader, "Barcode_get_BorderEndY1");
                m_pfnBarcode_get_BorderEndY2 = (PFN_Barcode_get_BorderEndY2)GetProcAddress(m_hDTKReader, "Barcode_get_BorderEndY2");
                m_pfnBarcode_get_BorderStartX1 = (PFN_Barcode_get_BorderStartX1)GetProcAddress(m_hDTKReader, "Barcode_get_BorderStartX1");
                m_pfnBarcode_get_BorderStartX2 = (PFN_Barcode_get_BorderStartX2)GetProcAddress(m_hDTKReader, "Barcode_get_BorderStartX2");
                m_pfnBarcode_get_BorderEndX1 = (PFN_Barcode_get_BorderEndX1)GetProcAddress(m_hDTKReader, "Barcode_get_BorderEndX1");
                m_pfnBarcode_get_BorderEndX2 = (PFN_Barcode_get_BorderEndX2)GetProcAddress(m_hDTKReader, "Barcode_get_BorderEndX2");
                m_pfnBarcode_Destroy = (PFN_Barcode_Destroy)GetProcAddress(m_hDTKReader, "Barcode_Destroy");
                m_pfnBarcodeReader_ActivateLicenseOnline = (PFN_BarcodeReader_ActivateLicenseOnline)GetProcAddress(m_hDTKReader, "BarcodeReader_ActivateLicenseOnline");

                // ADD LOGGING FOR EACH FUNCTION POINTER
                LOG_INFO(CLASSNAME, METHODNAME, _T("Function pointer loading results:"));
                if (!m_pfnBarcodeReader_Create) LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to load BarcodeReader_Create"));
                if (!m_pfnBarcodeReader_Destroy) LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to load BarcodeReader_Destroy"));
                if (!m_pfnBarcodeReader_ReadFromImageBuffer) LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to load BarcodeReader_ReadFromImageBuffer"));
                if (!m_pfnBarcodeReader_set_BarcodeTypes) LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to load BarcodeReader_set_BarcodeTypes"));
                if (!m_pfnBarcodeReader_set_Orientation) LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to load BarcodeReader_set_Orientation"));

                // Check if critical functions were loaded successfully
                if (m_pfnBarcodeReader_Create && m_pfnBarcodeReader_Destroy && m_pfnBarcodeReader_ReadFromImageBuffer) {
                    errorOccured = FALSE;
                    LOG_INFO(CLASSNAME, METHODNAME, _T("DTK5 function pointers loaded successfully"));
                }
                else {
                    LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to get critical DTK5 functions"));
                    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_FAILED_TO_GET_METHODS);
                }

            } else {
                // Failed to load DTK reader - provide detailed error information
                _stprintf(msgBuf, _T("LoadLibrary failed with error: %lu"), loadError);
                LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);

                // Decode common LoadLibrary errors
                switch (loadError) {
                    case ERROR_MOD_NOT_FOUND:
                        LOG_ERROR(CLASSNAME, METHODNAME, _T("ERROR_MOD_NOT_FOUND - The specified module could not be found"));
                        break;
                    case ERROR_BAD_EXE_FORMAT:
                        LOG_ERROR(CLASSNAME, METHODNAME, _T("ERROR_BAD_EXE_FORMAT - Architecture mismatch (32-bit vs 64-bit)"));
                        break;
                    case ERROR_ACCESS_DENIED:
                        LOG_ERROR(CLASSNAME, METHODNAME, _T("ERROR_ACCESS_DENIED - Insufficient permissions to load library"));
                        break;
                    case ERROR_DLL_INIT_FAILED:
                        LOG_ERROR(CLASSNAME, METHODNAME, _T("ERROR_DLL_INIT_FAILED - DLL initialization failed"));
                        break;
                    case ERROR_INVALID_ACCESS:
                        LOG_ERROR(CLASSNAME, METHODNAME, _T("ERROR_INVALID_ACCESS - Invalid memory access during load"));
                        break;
                    default:
                        _stprintf(msgBuf, _T("Unknown LoadLibrary error: %lu"), loadError);
                        LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
                        break;
                }

                // Format the system error message
                LPVOID lpMsgBuf;
                FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                    NULL, loadError, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&lpMsgBuf, 0, NULL);

                _stprintf(msgBuf, _T("LoadLibrary error description: %s"), (LPCTSTR)lpMsgBuf);
                LOG_ERROR(CLASSNAME, METHODNAME, msgBuf);
                LocalFree(lpMsgBuf);

                // CHECK SYSTEM PATHS FOR DEPENDENCIES
                LOG_INFO(CLASSNAME, METHODNAME, _T("Checking system PATH for dependencies"));
                TCHAR* pathEnv = _tgetenv(_T("PATH"));
                if (pathEnv) {
                    _stprintf(msgBuf, _T("PATH environment variable length: %d"), (int)_tcslen(pathEnv));
                    LOG_INFO(CLASSNAME, METHODNAME, msgBuf);
                } else {
                    LOG_WARNING(CLASSNAME, METHODNAME, _T("Could not retrieve PATH environment variable"));
                }

                ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_FAILED_TO_LOAD_READER);
            }

        }
        catch (ExceptionInfo* pExc) {
            LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("ExceptionInfo caught in LoadDTKReader"), pExc->GetErrorCode());
            throw pExc;
        }
        catch (...) {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Unknown exception caught in LoadDTKReader"));
            ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BARCODEREADER_READER_LOADING_FAILED);
        }
    }
    else {
        LOG_INFO(CLASSNAME, _T("LoadDTKReader"), _T("DTK reader already loaded, skipping initialization"));
    }
}

void BarcodeReader::InitializeDTKReader(PBARCODEREADERSETTINGS pSettings) {

    _METHODNAME(_T("InitializeDTKReader"));
    LOG_INFO(CLASSNAME, METHODNAME, _T("InitializeDTKReader method started"));

    if (m_hDTKReader != NULL && m_pfnBarcodeReader_Create != NULL) {

        LOG_INFO(CLASSNAME, METHODNAME, _T("Creating DTK5 barcode reader instance"));

        // Create BarcodeReader instance using new DTK5 API
        m_pDTKDCReader = m_pfnBarcodeReader_Create(NULL); // No callback needed

        if (m_pDTKDCReader != NULL) {

            LOG_INFO(CLASSNAME, METHODNAME, _T("DTK5 barcode reader instance created successfully"));

            // Try to activate licenses if function is available
            if (m_pfnBarcodeReader_ActivateLicenseOnline) {
                LOG_INFO(CLASSNAME, METHODNAME, _T("Attempting to activate licenses"));

                // Try to activate developer license
                int result = m_pfnBarcodeReader_ActivateLicenseOnline("5XZ1D-GEVDS-2JX17-5K2X5-8H6LV", "DTK5 Developer License");
                if (result == 0) {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Developer license activated successfully"));
                }
                else {
                    LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Developer license activation failed"), result);
                }

                // Try to activate server license
                result = m_pfnBarcodeReader_ActivateLicenseOnline("OU221WN3C4KD8MHN5U1E", "DTK5 Server License");
                if (result == 0) {
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Server license activated successfully"));
                }
                else {
                    LOG_ERROR_CODE(CLASSNAME, METHODNAME, _T("Server license activation failed"), result);
                }
            }

            LOG_INFO(CLASSNAME, METHODNAME, _T("Configuring barcode type selection"));
            BarcodeTypeEnum barcodeTypeSelection = BT_Unknown;
            if (pSettings->enableCode128) {
                barcodeTypeSelection = (BarcodeTypeEnum)(barcodeTypeSelection | BT_Code128);
                LOG_INFO(CLASSNAME, METHODNAME, _T("Code128 barcode type enabled"));
            }
            if (pSettings->enableInterleaved2of5) {
                barcodeTypeSelection = (BarcodeTypeEnum)(barcodeTypeSelection | BT_Inter2of5);
                LOG_INFO(CLASSNAME, METHODNAME, _T("Interleaved2of5 barcode type enabled"));
            }
            if (pSettings->enableCode39) {
                barcodeTypeSelection = (BarcodeTypeEnum)(barcodeTypeSelection | BT_Code39);
                LOG_INFO(CLASSNAME, METHODNAME, _T("Code39 barcode type enabled"));
            }
            if (pSettings->enableDataMatrix) {
                barcodeTypeSelection = (BarcodeTypeEnum)(barcodeTypeSelection | BT_DataMatrix);
                LOG_INFO(CLASSNAME, METHODNAME, _T("DataMatrix barcode type enabled"));
            }

            if (barcodeTypeSelection == BT_Unknown) {
                // Default to some common types if none specified
                barcodeTypeSelection = (BarcodeTypeEnum)(BT_Code39 | BT_Code128 | BT_Inter2of5);
                LOG_INFO(CLASSNAME, METHODNAME, _T("No barcode types specified, using defaults"));
            }

            LOG_INFO(CLASSNAME, METHODNAME, _T("Setting barcode types in DTK5 reader"));
            try {
                if (m_pfnBarcodeReader_set_BarcodeTypes) {
                    m_pfnBarcodeReader_set_BarcodeTypes(m_pDTKDCReader, barcodeTypeSelection);
                    LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode types set successfully"));
                }
            }
            catch (...) {
                LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception caught while setting barcode types"));
            }

            LOG_INFO(CLASSNAME, METHODNAME, _T("Setting barcode orientation"));
            try {
                SetBcOrientation();
                LOG_INFO(CLASSNAME, METHODNAME, _T("Barcode orientation set successfully"));
            }
            catch (...) {
                LOG_ERROR(CLASSNAME, METHODNAME, _T("Exception caught while setting barcode orientation"));
            }

            LOG_INFO(CLASSNAME, METHODNAME, _T("DTK5 reader initialization completed successfully"));

        }
        else {
            LOG_ERROR(CLASSNAME, METHODNAME, _T("Failed to create DTK5 barcode reader instance"));
        }
    }
    else {
        LOG_ERROR(CLASSNAME, METHODNAME, _T("DTK reader DLL not loaded or function pointer not available"));
    }

}

void BarcodeReader::SetBcOrientation() {

    if (m_pDTKDCReader != NULL && m_pfnBarcodeReader_set_Orientation != NULL) {

        BarcodeOrientationEnum newVal = BO_Unknown;

        if (m_BcOrientation & BCR_RD_LEFT_TO_RIGHT) { newVal = (BarcodeOrientationEnum)(newVal | BO_LeftToRight); }
        if (m_BcOrientation & BCR_RD_RIGHT_TO_LEFT) { newVal = (BarcodeOrientationEnum)(newVal | BO_RightToLeft); }
        if (m_BcOrientation & BCR_RD_TOP_TO_BOTTOM) { newVal = (BarcodeOrientationEnum)(newVal | BO_TopToBottom); }
        if (m_BcOrientation & BCR_RD_BOTTOM_TO_TOP) { newVal = (BarcodeOrientationEnum)(newVal | BO_BottomToTop); }

        m_pfnBarcodeReader_set_Orientation(m_pDTKDCReader, newVal);
    }

}

void BarcodeReader::ThrowNotInitializedException(PTCHAR pszMethodName) {

    if (m_pDTKDCReader == NULL) {
        ExceptionInfo::Throw(CLASSNAME, pszMethodName, ERR_BARCODEREADER_NOT_INITIALIZED);
    }

}

void BarcodeReader::EnterStandby() {
    m_inStandby = true;
}

void BarcodeReader::LeaveStandby() {
    m_inStandby = false;
}

// Check if all buffered barcodes are equal.
bool BarcodeReader::CheckRedundancyOk() {
    bool redundancyOk = true;
    USHORT i;
    for (i = 0; i < m_barcodeBufferLength; i++) {
        if ((m_barcodeBuffer[i] == nullptr) || (_tcscmp(m_barcodeBuffer[i], m_barcodeBuffer[0]) != 0)) {
            redundancyOk = false;
            break;
        }
    }
    return redundancyOk;
}

// Save a barcode to the barcode buffer.
void BarcodeReader::SaveToBuffer(PTCHAR bcString) {
    // Create a copy of the string since DTK5 may manage its own memory
    PTCHAR stringCopy;
    CopyString(bcString, stringCopy);

    // Release old string if it exists
    if (m_barcodeBuffer[m_nextBufferItem] != nullptr) {
        ReleaseString(m_barcodeBuffer[m_nextBufferItem]);
    }

    m_barcodeBuffer[m_nextBufferItem] = stringCopy;
    m_nextBufferItem++;
    if (m_nextBufferItem >= m_barcodeBufferLength) {
        m_nextBufferItem = 0;
    }
}

// Clear the barcode buffer.
void BarcodeReader::ClearBarcodeBuffer() {
    USHORT i;
    m_nextBufferItem = 0;

    for (i = 0; i < BARCODE_BUFFER_MAX_LENGTH; i++) {
        if (m_barcodeBuffer[i] != nullptr) {
            ReleaseString(m_barcodeBuffer[i]);
            m_barcodeBuffer[i] = nullptr;
        }
    }
}