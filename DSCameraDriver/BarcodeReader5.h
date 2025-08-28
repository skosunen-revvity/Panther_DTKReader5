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

#pragma once

// Include DTK5 header for proper enum definitions
#include "DTKBarReader5.h"

// Barcode reading directions
#define BCR_RD_LEFT_TO_RIGHT  1   // Read barcodes from left to right
#define BCR_RD_RIGHT_TO_LEFT  2   // Read barcodes from right to left
#define BCR_RD_TOP_TO_BOTTOM  4   // Read barcodes from top to bottom
#define BCR_RD_BOTTOM_TO_TOP  8   // Read barcodes from bottom to top

#define BARCODE_BUFFER_MAX_LENGTH 9   // Buffer can contain up to 9 barcode strings

// Barcode implementation type. This is a future reservation
enum eBarcodeReaderType {
    DTK = 0
};


// Barcode reader settings
typedef struct _BARCODEREADERSETTINGS {

    PTCHAR  pszReaderBinaryLocation;      // Actual reader DLL location
    BYTE    maxBarcodeCount;              // Number of barcodes that are read from the image

    USHORT  usBarcodeReadingInterval1;    // Barcode rading interval in milliseconds. This is used as a intervall 
    // between read attempts when we don´t have any barcodes in cache

    USHORT  usBarcodeReadingInterval2;    // Barcode rading interval in milliseconds. This is used as a intervall 
    // between read attempts when barcode cache contains barcodes

    BYTE    BcOrientation;                // Specifies which way barcodes are detected from the image.
    // This is a bit field, valid values are.
    // 1 (BCR_RD_LEFT_TO_RIGHT) = Left to right
    // 2 (BCR_RD_RIGHT_TO_LEFT) = Right to left
    // 4 (BCR_RD_TOP_TO_BOTTOM) = Top to bottom
    // 8 (BCR_RD_BOTTOM_TO_TOP) = Bottom to top

    BYTE	enableCode128;                // Enable / disable Code 128 symbology
    BYTE	enableInterleaved2of5;        // Enable / disable Interleaved 2 of 5 symbology.
    BYTE	enableCode39;                 // Enable / disable Code 39 symbology.
    BYTE	enableDataMatrix;             // Enable / disable Data Matrix symbology (2D)

    // Scan rectangles. There are three possible rectangles for limitation of the area
    // for barcodes scanning. Decoding is attempted only inside these rectangles.
    BYTE enableScanRect1;    // Enable / disable scan rectangle 1
    USHORT scanRect1Left;    // Rectangle coordinates, left
    USHORT scanRect1Top;     // Rectangle coordinates, top
    USHORT scanRect1Right;   // Rectangle coordinates, right
    USHORT scanRect1Bottom;  // Rectangle coordinates, bottom

    BYTE enableScanRect2;    // Enable / disable scan rectangle 2
    USHORT scanRect2Left;
    USHORT scanRect2Top;
    USHORT scanRect2Right;
    USHORT scanRect2Bottom;

    BYTE enableScanRect3;    // Enable / disable scan rectangle 3
    USHORT scanRect3Left;
    USHORT scanRect3Top;
    USHORT scanRect3Right;
    USHORT scanRect3Bottom;

    USHORT minimumBarcodeHeight;  // Minimum height required for a barcode, pixels.
    USHORT barcodeRedundancy;     // Number of identical decodings needed for a barcode to be accepted.

} BARCODEREADERSETTINGS, * PBARCODEREADERSETTINGS;


class BarcodeReader {

public:
    BarcodeReader();
    ~BarcodeReader();

    // Initialize reader. Will load the reader DLL and initialize it
    void                  Initialize(PBARCODEREADERSETTINGS pSettings);

    // Release reader and unload the reader DLL. Barcode storage is 
    // also released
    void                  Release();

    // Clear all read barcodes, call this when blood card has been removed
    void                  ClearBarcodes();

    // Try to read barcodes from the bitmap. Before trying to read barcode,
    // IsReadRequired-method should be used to verify whether read is required.
    // After it has return TRUE, read can be used. Previously read barcodes are
    // removed and the only the new are stored
    void                  Read(HBITMAP hBitmap, DWORD dwCurrentTime);

    // Return TRUE if enough time has elapsed from the last read.
    BYTE                  IsReadRequired(DWORD dwCurrentTime, int searchingPunches);

    // Get current reader settings
    void                  GetCurrentSettings(PBARCODEREADERSETTINGS pSettings);

    // Change settings. You can´t change the actual reader (pszReaderBinaryLocation)
    void                  UpdateSettings(PBARCODEREADERSETTINGS pSettings);

    // Return TRUE if barcodes changed during last read
    inline BYTE           IsBarcodesChanged() { return m_BarcodesChanged; }

    // Get the time when last read was done
    inline DWORD          GetLastReadTime() { return m_dwLastReadTime; }

    // Get the read barcode
    inline PTCHAR   GetBarcode() { return m_readBarcode; }

    // Get the type of read barcode as string, such as "Code39"
    PTCHAR   GetBarcodeType();

    // Enter/ Leave standby. Currently, in standby the only difference to normal operating mode is that the barcode 
    // reading interval is high thus preventing barcode reader module license complaining problems.
    virtual void EnterStandby();
    virtual void LeaveStandby();


protected:

    // Specifies which way barcodes are detected from the image.
    // This is a bit field, valid values are.
    // 1 (BCR_RD_LEFT_TO_RIGHT) = Left to right
    // 2 (BCR_RD_RIGHT_TO_LEFT) = Right to left
    // 4 (BCR_RD_TOP_TO_BOTTOM) = Top to bottom
    // 8 (BCR_RD_BOTTOM_TO_TOP) = Bottom to top
    BYTE                  m_BcOrientation;

    // Will be set to TRUE when barcodes changed during last read operation
    BYTE                  m_BarcodesChanged;

    // Actual barcode reader location
    PTCHAR                m_pszReaderLocation;

    // Previously read barcode
    PTCHAR                m_readBarcode;
    int                   m_readType;

    // Buffering of read barcodes. Needed for redundany check.
    // Where to insert the next decoded barcode.
    USHORT                m_nextBufferItem;
    // Number of barcodes to save in buffer.
    USHORT                m_barcodeBufferLength;
    // The buffer will hold the n latest barcodes, where n is 0..9.
    PTCHAR                m_barcodeBuffer[BARCODE_BUFFER_MAX_LENGTH];

    // Time when last read was made
    DWORD                 m_dwLastReadTime;

    // DTK barcode reader library (DLL) handle
    HMODULE               m_hDTKReader;

    // Actual DTK5 barcode reader handle
    BARREADER             m_pDTKDCReader;

    // Barcode rading interval in milliseconds. This is used as a intervall 
    // between read attempts when we don´t have any barcodes in cache
    USHORT                m_usBarcodeReadingInterval1;

    // Barcode rading interval in milliseconds. This is used as a intervall 
    // between read attempts when barcode cache contains barcodes
    USHORT                m_usBarcodeReadingInterval2;

    // DTK5 Function pointer types - using proper enum types
    typedef BARREADER(*PFN_BarcodeReader_Create)(PageProcessingCompletedCallback);
    typedef void (*PFN_BarcodeReader_Destroy)(BARREADER);
    typedef BARREADERRESULT(*PFN_BarcodeReader_ReadFromImageBuffer)(BARREADER, void*, int, int, int, PixelFormatEnum);
    typedef void (*PFN_BarcodeReader_set_BarcodeTypes)(BARREADER, BarcodeTypeEnum);
    typedef void (*PFN_BarcodeReader_set_Orientation)(BARREADER, BarcodeOrientationEnum);
    typedef void (*PFN_BarcodeReader_set_ScanRectangleX)(BARREADER, int);
    typedef void (*PFN_BarcodeReader_set_ScanRectangleY)(BARREADER, int);
    typedef void (*PFN_BarcodeReader_set_ScanRectangleWidth)(BARREADER, int);
    typedef void (*PFN_BarcodeReader_set_ScanRectangleHeight)(BARREADER, int);
    typedef int (*PFN_BarReaderResult_get_BarcodesCount)(BARREADERRESULT);
    typedef BARCODE(*PFN_BarReaderResult_get_Barcode)(BARREADERRESULT, int);
    typedef void (*PFN_BarReaderResult_Destroy)(BARREADERRESULT);
    typedef int (*PFN_Barcode_get_Text)(BARCODE, char*, int);
    typedef BarcodeTypeEnum(*PFN_Barcode_get_Type)(BARCODE);
    typedef int (*PFN_Barcode_get_Threshold)(BARCODE);
    typedef int (*PFN_Barcode_get_Checksum)(BARCODE);
    typedef float (*PFN_Barcode_get_Angle)(BARCODE);
    typedef BarcodeOrientationEnum(*PFN_Barcode_get_Orientation)(BARCODE);
    typedef int (*PFN_Barcode_get_TypeString)(BARCODE, char*, int);
    typedef int (*PFN_Barcode_get_BorderStartY1)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderStartY2)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderEndY1)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderEndY2)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderStartX1)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderStartX2)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderEndX1)(BARCODE);
    typedef int (*PFN_Barcode_get_BorderEndX2)(BARCODE);
    typedef void (*PFN_Barcode_Destroy)(BARCODE);
    typedef int (*PFN_BarcodeReader_ActivateLicenseOnline)(const char*, const char*);

    // DTK5 Function pointers
    PFN_BarcodeReader_Create m_pfnBarcodeReader_Create;
    PFN_BarcodeReader_Destroy m_pfnBarcodeReader_Destroy;
    PFN_BarcodeReader_ReadFromImageBuffer m_pfnBarcodeReader_ReadFromImageBuffer;
    PFN_BarcodeReader_set_BarcodeTypes m_pfnBarcodeReader_set_BarcodeTypes;
    PFN_BarcodeReader_set_Orientation m_pfnBarcodeReader_set_Orientation;
    PFN_BarcodeReader_set_ScanRectangleX m_pfnBarcodeReader_set_ScanRectangleX;
    PFN_BarcodeReader_set_ScanRectangleY m_pfnBarcodeReader_set_ScanRectangleY;
    PFN_BarcodeReader_set_ScanRectangleWidth m_pfnBarcodeReader_set_ScanRectangleWidth;
    PFN_BarcodeReader_set_ScanRectangleHeight m_pfnBarcodeReader_set_ScanRectangleHeight;
    PFN_BarReaderResult_get_BarcodesCount m_pfnBarReaderResult_get_BarcodesCount;
    PFN_BarReaderResult_get_Barcode m_pfnBarReaderResult_get_Barcode;
    PFN_BarReaderResult_Destroy m_pfnBarReaderResult_Destroy;
    PFN_Barcode_get_Text m_pfnBarcode_get_Text;
    PFN_Barcode_get_Type m_pfnBarcode_get_Type;
    PFN_Barcode_get_Threshold m_pfnBarcode_get_Threshold;
    PFN_Barcode_get_Checksum m_pfnBarcode_get_Checksum;
    PFN_Barcode_get_Angle m_pfnBarcode_get_Angle;
    PFN_Barcode_get_Orientation m_pfnBarcode_get_Orientation;
    PFN_Barcode_get_TypeString m_pfnBarcode_get_TypeString;
    PFN_Barcode_get_BorderStartY1 m_pfnBarcode_get_BorderStartY1;
    PFN_Barcode_get_BorderStartY2 m_pfnBarcode_get_BorderStartY2;
    PFN_Barcode_get_BorderEndY1 m_pfnBarcode_get_BorderEndY1;
    PFN_Barcode_get_BorderEndY2 m_pfnBarcode_get_BorderEndY2;
    PFN_Barcode_get_BorderStartX1 m_pfnBarcode_get_BorderStartX1;
    PFN_Barcode_get_BorderStartX2 m_pfnBarcode_get_BorderStartX2;
    PFN_Barcode_get_BorderEndX1 m_pfnBarcode_get_BorderEndX1;
    PFN_Barcode_get_BorderEndX2 m_pfnBarcode_get_BorderEndX2;
    PFN_Barcode_Destroy m_pfnBarcode_Destroy;
    PFN_BarcodeReader_ActivateLicenseOnline m_pfnBarcodeReader_ActivateLicenseOnline;

    // Load DTK reader library and obtain pointers to init/release methods
    void                  LoadDTKReader(const PTCHAR pszDTKReaderLocation);

    // Init DTK reader: get the avtual reader (m_pDTKDCReader), add licence
    // and configure it
    void                  InitializeDTKReader(PBARCODEREADERSETTINGS pSettings);

    // Throw exception (ExceptionInfo*) if reader isn´t initialized
    void                  ThrowNotInitializedException(const PTCHAR pszMethodName);

    // Set the barcode reading directions
    void                  SetBcOrientation();

    // Check if all buffered barcodes are equal.
    bool                  CheckRedundancyOk();

    // Save a barcode to the barcode buffer.
    void                  SaveToBuffer(PTCHAR bcString);

    // Clear the barcode buffer.
    void                  ClearBarcodeBuffer();

    // Whether the module is in standby or not
    bool m_inStandby;

    // Barcode scan rectangles definitions
    bool m_enableScanRect[3];
    USHORT m_scanRectLeft[3];
    USHORT m_scanRectTop[3];
    USHORT m_scanRectRight[3];
    USHORT m_scanRectBottom[3];

    USHORT m_minimumBarcodeHeight;

};