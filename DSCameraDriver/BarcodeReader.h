// --------------------------------------------------------------------------
// BarcodeReader.h
// --------------------------------------------------------------------------
//
// SpotLight barcode reader definition. Class that can be used to read barcodes
// from the specified bitmap. Actual reading is done using the specified reader,
// currently only supported reader is the DTK Barcode REader. This DTK reader is
// initialized and loaded when class is initialized, in Initialize-method.
//
// Barcodes can be read from the image using the Read-method. In order to optimize
// reading a bit, IsReadRequired-method can be used to check if enough time have
// elapsed from the last read operation. Class contains two intervall values fo
// the time checking:
// - m_usBarcodeReadingInterval1: used when imager is not searching punch positions
// - m_usBarcodeReadingInterval2: used when imager is searching punch positions
//
// Image may contain a number of barcodes, and the used reader (DTK) is able to
// read a number of barcodes from the same image. Only one (the first one, or the
// one that was read on previous attempt too) is returned to higher level.
//
// In order to release the class, Release-method must be called: it will unload
// and release the actual DTK Reader.
//
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 29.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

// --------------------------------------------------------------------------

#pragma once

#include <windows.h>
#include "DTKBarReader5.h"   // v5 API types (we call functions via pointers)

// Barcode reading directions
#define BCR_RD_LEFT_TO_RIGHT  1
#define BCR_RD_RIGHT_TO_LEFT  2
#define BCR_RD_TOP_TO_BOTTOM  4
#define BCR_RD_BOTTOM_TO_TOP  8

#define BARCODE_BUFFER_MAX_LENGTH 9

enum eBarcodeReaderType {
    DTK = 0
};

// Settings structure (unchanged)
typedef struct _BARCODEREADERSETTINGS {
    PTCHAR  pszReaderBinaryLocation;
    BYTE    maxBarcodeCount;
    USHORT  usBarcodeReadingInterval1;
    USHORT  usBarcodeReadingInterval2;
    BYTE    BcOrientation;
    BYTE    enableCode128;
    BYTE    enableInterleaved2of5;
    BYTE    enableCode39;
    BYTE    enableDataMatrix;

    BYTE enableScanRect1;
    USHORT scanRect1Left;
    USHORT scanRect1Top;
    USHORT scanRect1Right;
    USHORT scanRect1Bottom;

    BYTE enableScanRect2;
    USHORT scanRect2Left;
    USHORT scanRect2Top;
    USHORT scanRect2Right;
    USHORT scanRect2Bottom;

    BYTE enableScanRect3;
    USHORT scanRect3Left;
    USHORT scanRect3Top;
    USHORT scanRect3Right;
    USHORT scanRect3Bottom;

    USHORT minimumBarcodeHeight;
    USHORT barcodeRedundancy;
} BARCODEREADERSETTINGS, *PBARCODEREADERSETTINGS;

// v5 function pointer typedefs (only what we use now)
typedef BARREADER (DTKBARAPI *PFN_BR5_CREATE)(PageProcessingCompletedCallback);
typedef void      (DTKBARAPI *PFN_BR5_DESTROY)(BARREADER);

class BarcodeReader {
public:
    BarcodeReader();
    ~BarcodeReader();

    void  Initialize(PBARCODEREADERSETTINGS pSettings);
    void  Release();

    void  ClearBarcodes();
    void  Read(HBITMAP hBitmap, DWORD dwCurrentTime); // Stub (not implemented yet)
    BYTE  IsReadRequired(DWORD dwCurrentTime, int searchingPunches);
    void  GetCurrentSettings(PBARCODEREADERSETTINGS pSettings);
    void  UpdateSettings(PBARCODEREADERSETTINGS pSettings);

    inline BYTE   IsBarcodesChanged() { return m_BarcodesChanged; }
    inline DWORD  GetLastReadTime()   { return m_dwLastReadTime; }
    inline PTCHAR GetBarcode()        { return m_readBarcode; }
    PTCHAR GetBarcodeType();

    virtual void EnterStandby();
    virtual void LeaveStandby();

protected:
    BYTE    m_BcOrientation;
    BYTE    m_BarcodesChanged;
    PTCHAR  m_pszReaderLocation;
    PTCHAR  m_readBarcode;
    int     m_readType;

    USHORT  m_nextBufferItem;
    USHORT  m_barcodeBufferLength;
    PTCHAR  m_barcodeBuffer[BARCODE_BUFFER_MAX_LENGTH];

    DWORD   m_dwLastReadTime;

    // v5 dynamic load
    HMODULE m_hDTK5Lib;
    PFN_BR5_CREATE  m_pfnCreate5;
    PFN_BR5_DESTROY m_pfnDestroy5;
    BARREADER       m_hBarReader5;

    USHORT  m_usBarcodeReadingInterval1;
    USHORT  m_usBarcodeReadingInterval2;

    bool    m_inStandby;

    bool    m_enableScanRect[3];
    USHORT  m_scanRectLeft[3];
    USHORT  m_scanRectTop[3];
    USHORT  m_scanRectRight[3];
    USHORT  m_scanRectBottom[3];

    USHORT  m_minimumBarcodeHeight;

    void ThrowNotInitializedException(const PTCHAR pszMethodName);

    // Reserved for later migration steps (currently unused)
    void SetBcOrientation();

    bool CheckRedundancyOk();
    void SaveToBuffer(PTCHAR bcString);
    void ClearBarcodeBuffer();

    bool LoadDTK5(const PTCHAR dllPath);
    void UnloadDTK5();
};
