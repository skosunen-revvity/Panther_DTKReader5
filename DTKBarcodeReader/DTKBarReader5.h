/*
*	DTKBarReader5.h
*
*   DTK Barcode Reader SDK v5
*
*	Copyright (c) DTK Software, 2025
*
*	http://www.dtksoft.com
*	email: support@dtksoft.com
*/

#ifndef __DTKBarReader5_H
#define __DTKBarReader5_H

#ifndef DTKBARAPI
#if defined WIN32 || defined _WIN32
#define DTKBARAPI __stdcall
#else
#define DTKBARAPI 
#endif
#endif

#ifdef _WIN32
#include "windows.h"
typedef __int64 int64;
#else
#include <stdint.h>
#include <time.h>
typedef int64_t int64;
#endif

#ifdef __cplusplus
extern "C" {
#endif

// =============================
// Group: Types
// =============================

// Type: BARREADER
// Handle of BarcodeReader object
//
// Used to handle pointer (void *) to BarcodeReader object
typedef void* BARREADER;

// Type: BARREADERRESULT
// Handle of BarReaderResult object
//
// Used to handle pointer (void *) to BarReaderResult object
typedef void* BARREADERRESULT;

// Type: BARCODE
// Handle of Barcode object
//
// Used to handle pointer (void *) to Barcode object
typedef void* BARCODE;

// =============================
// Group: Enumerations
// =============================

/* Enum: BarcodeTypeEnum
* Barcode types
*
*	BT_Unknown - barcode type is undefined
* 	BT_Code11 - Code 11
* 	BT_Code39 - Code 39
*  	BT_Code93 - Code 93
*  	BT_Code128 - Code 128
*  	BT_Codabar - Codabar
*  	BT_Inter2of5 - Interleaved 2 of 5
*  	BT_PatchCode - Patch Code
*  	BT_EAN8 - EAN-8
*  	BT_UPCE - UPC-E
*  	BT_EAN13 - EAN-13
*  	BT_UPCA - UPC-A
*  	BT_Plus2 - +2 Supplemental for UPC/EAN
*  	BT_Plus5 - +5 Supplemental for UPC/EAN
*  	BT_PDF417 - PDF417
*  	BT_DataMatrix - DataMatrix
*  	BT_QRCode - QR Code
*  	BT_Postnet - POSTNET
*  	BT_Planet - PLANET
*  	BT_RM4SCC - Royal Mail 4-State Customer Code (RM4SCC)
*  	BT_AustraliaPost - Australia Post
*  	BT_IntelligentMail - Intelligent Mail
*  	BT_Code39Extended - Code 39 Extended
*  	BT_MicroQRCode - Micro QR Code
*  	BT_PharmaCode - Pharmacode
*  	BT_UCC128 - UCC-128
*  	BT_RSS14 - RSS-14 / GS1 Databar
*  	BT_RSSLimited - RSS-14 / GS1 Databar Limited
*  	BT_RSSExpanded - RSS-14 / GS1 Databar Expanded / Stacked
*  	BT_All - All barcode types
*  	BT_All_1D - All 1D barcodes
*  	BT_All_2D - All 2D barcodes
*/
typedef enum BarcodeTypeEnum
{
	BT_Unknown = 0,
	BT_Code11 = 0x1,
	BT_Code39 = 0x2,
	BT_Code93 = 0x4,
	BT_Code128 = 0x8,
	BT_Codabar = 0x10,
	BT_Inter2of5 = 0x20,
	BT_PatchCode = 0x40,
	BT_EAN8 = 0x80,
	BT_UPCE = 0x100,
	BT_EAN13 = 0x200,
	BT_UPCA = 0x400,
	BT_Plus2 = 0x800,
	BT_Plus5 = 0x1000,
	BT_PDF417 = 0x2000,
	BT_DataMatrix = 0x4000,
	BT_QRCode = 0x8000,
	BT_Postnet = 0x10000,
	BT_Planet = 0x20000,
	BT_RM4SCC = 0x40000,
	BT_AustraliaPost = 0x80000,
	BT_IntelligentMail = 0x100000,
	BT_Code39Extended = 0x200000,
	BT_MicroQRCode = 0x400000,
	BT_PharmaCode = 0x800000,
	BT_UCC128 = 0x1000000,
	BT_RSS14 = 0x2000000,
	BT_RSSLimited = 0x4000000,
	BT_RSSExpanded = 0x8000000,
	BT_All = 0xffffffff,
	BT_All_1D = 0xfbf1fff,
	BT_All_2D = 0x40e000
} 	BarcodeTypeEnum;

/* Enum: BarcodeOrientationEnum
* Barcode orientations
* 
*	BO_Unknown - Orientation is not defined
*	BO_LeftToRight - Left to right orientation
*	BO_RightToLeft - Right to left orientation
*	BO_TopToBottom - Top to bottom orientation
*	BO_BottomToTop - Bottom to top orientation
*	BO_All - All orientations
*/
typedef enum BarcodeOrientationEnum
{
	BO_Unknown = 0,
	BO_LeftToRight = 1,
	BO_RightToLeft = 2,
	BO_TopToBottom = 4,
	BO_BottomToTop = 8,
	BO_All = 255
} BarcodeOrientationEnum;

/* Enum: QuietZoneSizeEnum
* Barcode quiet zone size
*
*	QZ_ExtraSmall - Extra small quiet zone
*	QZ_Small - Small quiet zone
*	QZ_Normal - Normal quiet zone
*	QZ_Large - Large quiet zone
* 
* See Also:
*	<BarcodeReader_set_QuietZoneSize>
*/
typedef enum QuietZoneSizeEnum
{
	QZ_ExtraSmall = 0x1,
	QZ_Small = 0x2,
	QZ_Normal = 0x3,
	QZ_Large = 0x4
} QuietZoneSizeEnum;

/* Enum: ThresholdModeEnum
* Thresholding mode
*
*	TM_Automatic - Automatic threshold detection 
*	TM_Fixed - Fixed threshold
*	TM_Multiple - Multiple thresholds
*	TM_Adaptive - Adaptive thresholding
*
* See Also:
*	<BarcodeReader_set_Threshold>
*/
typedef enum ThresholdModeEnum
{
	TM_Automatic = 0x01,
	TM_Fixed = 0x02,
	TM_Multiple = 0x03,
	TM_Adaptive = 0x04,
} ThresholdModeEnum;

/* Enum: PreprocessingEnum
* Preprocessing operations
*
*	PR_Erosion - perform morphological Erosion
*	PR_Dilation - perform morphological Dilation
*	PR_Despeckle - remove speckles from image
*	PR_Sharp - make an image sharper
*	PR_Invert - invert image
*	PR_FlipHorizontal - flip the image by horizontally
*	PR_FlipVertical - flip the image by vertically
* 
* See Also:
*	<Preprocessing Operations>
*/
typedef enum PreprocessingEnum
{
	PR_Erosion = 1,
	PR_Dilation = 2,
	PR_Despeckle = 3,
	PR_Sharp = 4,
	PR_Invert = 5,
	PR_FlipHorizontal = 6,
	PR_FlipVertical = 7
} PreprocessingEnum;

/* Enum: QRCodeECCLevelEnum
* QR Code Error correction levels
*
*	QRECCLevel_Undefined - ECC level not defined
*	QRECCLevel_L - Error correction level L (up to 7% damage)
*	QRECCLevel_M - Error correction level M (up to 15% damage)
*	QRECCLevel_Q - Error correction level Q (up to 25% damage)
*	QRECCLevel_H - Error correction level H (up to 30% damage)
* 
* See Also:
*	<BarcodeReader_set_QRCodeRequiredECCLevel>
*/
typedef enum QRCodeECCLevelEnum
{
	QRECCLevel_Undefined = 0x00,
	QRECCLevel_L = 0x01,
	QRECCLevel_M = 0x02,
	QRECCLevel_Q = 0x03,
	QRECCLevel_H = 0x04,
} QRCodeECCLevelEnum;

/* Enum: PixelFormatEnum
* Image buffer pixel formats
*
* 	PIXFMT_NONE - pixel format is undefined
* 	PIXFMT_GRAYSCALE - Grayscale 8 bpp format
* 	PIXFMT_RGB24 - RGB 24 bpp format
* 	PIXFMT_BGR24 - BGR 24 bpp format
*	PIXFMT_RGBA - RGB 32 bpp format (with alpha channel)
*	PIXFMT_BGRA - BGR 32 bpp format (with alpha channel)
*	PIXFMT_YUV420 - YUV420 format
*/
typedef enum PixelFormatEnum
{
	PIXFMT_NONE = 0,
	PIXFMT_GRAYSCALE = 1,
	PIXFMT_RGB24 = 2,
	PIXFMT_BGR24 = 3,
	PIXFMT_RGBA = 4,
	PIXFMT_BGRA = 5,
	PIXFMT_YUV420 = 6,
	PIXFMT_1BPP = 7,
} PixelFormatEnum;

/* Enum: PDFReadingTypeEnum
* PDF file reading type
*
*	PDF_Render - Render whole page to single image and process it.
*	PDF_Images - Extract all images from page and process each image separately.
* 
* See Also:
*	<BarcodeReader_set_PDFReadingType>
*/
typedef enum PDFReadingTypeEnum
{
	PDF_Render = 1,
	PDF_Images = 2,
} PDFReadingTypeEnum;

// =============================
// Group: Callbacks
// =============================

/* Type: PageProcessingCompletedCallback
* Invokes by BarcodeReader object after the page has finished processing
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	pageNum - page number
*	errorCode - error code
*/
typedef void(DTKBARAPI* PageProcessingCompletedCallback)(BARREADER hBarReader, int pageNum, int errorCode);

// =============================================================
// Group: BarcodeReader Functions
// =============================================================

/* Function: BarcodeReader_Create
*  Create new BarcodeReader instance
*
* Parameters:
*	pageProcessingCompletedCallback - PageProcessingCompletedCallback function
* 
* Return:
*	 Handle of BarcodeReader object
*
* Notes:
*	The parameter pageProcessingCompletedCallback can be empty (zero). In this case PageProcessingCompletedCallback will not be invoked. 
*	
*	The PageProcessingCompletedCallback function do not impact to image processing algorithms and final results. 
*	It will simply be called after the page/image has finished processing, to be able to control the progress of the multi-page files processing.
*/
BARREADER DTKBARAPI BarcodeReader_Create(PageProcessingCompletedCallback pageProcessingCompletedCallback);

/* Function: BarcodeReader_Destroy
* Destroy BarcodeReader instance
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*/
void DTKBARAPI BarcodeReader_Destroy(BARREADER hBarReader);

/* Function: BarcodeReader_ReadFromFile
* Read barcodes from image file (JPEG, BMP, PNG)
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	fileName - Path to image file
*
* Return:
*	Handle of BarReaderResult object
*
* Notes:
*	BarReaderResult object must be deleted using function <BarReaderResult_Destroy>
*/
BARREADERRESULT DTKBARAPI BarcodeReader_ReadFromFile(BARREADER hBarReader, const char* fileName);
	
/* Function: BarcodeReader_ReadFromFileW
* Read barcodes from image file (JPEG, BMP, PNG)
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	fileName - Path to image file (Unicode)
*
* Return:
*	Handle of BarReaderResult object
*
* Notes:
*	BarReaderResult object must be deleted using function <BarReaderResult_Destroy>
*/
BARREADERRESULT DTKBARAPI BarcodeReader_ReadFromFileW(BARREADER hBarReader, const wchar_t* fileName);
	
/* Function: BarcodeReader_ReadFromMemFile
* Read barcodes from image file (JPEG, BMP, PNG) contained in the memory
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	pBuffer - Pointer to a memory buffer containing the image file data
*	bufferSize - size of memory buffer in bytes
*
* Return:
*	Handle of BarReaderResult object
*
* Notes:
*	BarReaderResult object must be deleted using function <BarReaderResult_Destroy>
*/
BARREADERRESULT DTKBARAPI BarcodeReader_ReadFromMemFile(BARREADER hBarReader, void* pBuffer, int bufferSize);

/* Function: BarcodeReader_ReadFromURL
* Read barcodes from image file (JPEG, BMP, PNG) from HTTP URL
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	url - URL string, example: http://domain.com/image.jpg
*
* Return:
*	Handle of BarReaderResult object
*
* Notes:
*	BarReaderResult object must be deleted using function <BarReaderResult_Destroy>.
*
*	This function can be used for still image mode only.
*/
BARREADERRESULT DTKBARAPI BarcodeReader_ReadFromURL(BARREADER hBarReader, const char* url);

/* Function: BarcodeReader_ReadFromImageBuffer
* Read barcodes from image buffer
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	pBuffer - Pointer to buffer of image pixels data
*	width - Width of image in pixels
*	height - Height of image in pixels
*	stride - Size of one image row in bytes
*	pixelFormat - Pixel format, PIXFMT enumeration
*
* Return:
*	Handle of BarReaderResult object
*
* Notes:
*	BarReaderResult object must be deleted using function <BarReaderResult_Destroy>
*/
BARREADERRESULT DTKBARAPI BarcodeReader_ReadFromImageBuffer(BARREADER hBarReader, void* pBuffer, int width, int height, int stride, PixelFormatEnum pixelFormat);

/* Function: BarcodeReader_set_BarcodeTypes
* Sets the types of barcode for recognition
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	types - one or a combination of <BarcodeTypeEnum> enumeration values
*
* Notes:
*	Default value is 0 (BT_Unknown).
*/
void DTKBARAPI BarcodeReader_set_BarcodeTypes(BARREADER hBarReader, BarcodeTypeEnum types);

/* Function: BarcodeReader_get_BarcodeTypes
* Gets the types of barcode for recognition
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	One or a combination of <BarcodeTypeEnum> enumeration values
*
* Notes:
*	Default value is 0 (BT_Unknown).
*/
BarcodeTypeEnum DTKBARAPI BarcodeReader_get_BarcodeTypes(BARREADER hBarReader);

/* Function: BarcodeReader_set_Threshold
* Sets the color threshold to distinguish between background and foreground pixels.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	threshold - threshold value from 0 to 255
*
* Notes:
*  	Default value is 128.
*
*	Threshold related parameters are ThresholdMode, Threshold, ThresholdStep and ThresholdCount.
*	The threshold is used to distinguish between foreground and background pixels (black and white information) in color or grayscale images.
*	Possible threshold values are 0..255. Each pixel in a color or grayscale image has a brightness from 0 (black) to 255 (white).
*	When the threshold is set to 128, pixels with intensities less than 128 will be treated as black (foreground) pixels,and pixels with intensities greater than 128 as white (background) pixels.
*	The threshold parameters are ignored for binary (black and white) images.
*
*	There are the following thresholding modes
*
*	1) ThresholdMode = TM_Automatic
*
*	The threshold is determined automatically using the adaptive global threshold algorithm. The threshold will be calculated for each image (page).
*	The Threshold, ThresholdStep and ThresholdCount parameters not used in this mode.
*
*
*	2) ThresholdMode = TM_Fixed
*
*	Only the value of the Threshold parameter will be used.
*	The ThresholdStep and ThresholdCount parameters not used in this mode.
*
*
*	3) ThresholdMode = TM_Multiple
*
*	This mode will use multiple thresholds and process image for each threshold separately.
*	In this mode ThresholdCount and ThresholdStep (S) will be used.
*	The value specified in the Threshold (T) parameter will be used as the initial value, followed by T+S, T-S, T+2*S, T-2*S, and so on, until the number of calculated thresholds exceeds ThresholdCount.
*
*	Example:
*
*	Threshold = 100
*
*	ThresholdCount = 6
*
*	ThresholdStep = 10
*
*	The following thresholds will be used: 100, 110, 90, 120, 80, 130, 70
*/
void DTKBARAPI BarcodeReader_set_Threshold(BARREADER hBarReader, int threshold);

/* Function: BarcodeReader_get_Threshold
* Gets the color threshold to distinguish between background and foreground pixels.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Threshold value
*
* Notes:
*  	Default value is 128.
*
*	Threshold related parameters are ThresholdMode, Threshold, ThresholdStep and ThresholdCount.
*	The threshold is used to distinguish between foreground and background pixels (black and white information) in color or grayscale images.
*	Possible threshold values are 0..255. Each pixel in a color or grayscale image has a brightness from 0 (black) to 255 (white).
*	When the threshold is set to 128, pixels with intensities less than 128 will be treated as black (foreground) pixels,and pixels with intensities greater than 128 as white (background) pixels.
*	The threshold parameters are ignored for binary (black and white) images.
*
*	There are the following thresholding modes
*
*	1) ThresholdMode = TM_Automatic
*
*	The threshold is determined automatically using the adaptive global threshold algorithm. The threshold will be calculated for each image (page).
*	The Threshold, ThresholdStep and ThresholdCount parameters not used in this mode.
*
*
*	2) ThresholdMode = TM_Fixed
*
*	Only the value of the Threshold parameter will be used.
*	The ThresholdStep and ThresholdCount parameters not used in this mode.
*
*
*	3) ThresholdMode = TM_Multiple
*
*	This mode will use multiple thresholds and process image for each threshold separately.
*	In this mode ThresholdCount and ThresholdStep (S) will be used.
*	The value specified in the Threshold (T) parameter will be used as the initial value, followed by T+S, T-S, T+2*S, T-2*S, and so on, until the number of calculated thresholds exceeds ThresholdCount.
*
*	Example:
*
*	Threshold = 100
*
*	ThresholdCount = 6
*
*	ThresholdStep = 10
*
*	The following thresholds will be used: 100, 110, 90, 120, 80, 130, 70
*/
int DTKBARAPI BarcodeReader_get_Threshold(BARREADER hBarReader);

/* Function: BarcodeReader_set_ThresholdMode
* Sets the mode of image thresholding
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	thresholdMode - <ThresholdModeEnum> enumeration value
*
* Notes:
*  	Default value is TM_Automatic.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
void DTKBARAPI BarcodeReader_set_ThresholdMode(BARREADER barReader, ThresholdModeEnum thresholdMode);

/* Function: BarcodeReader_get_ThresholdMode
* Gets the mode of image thresholding
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	<ThresholdModeEnum> enumeration value
*
* Notes:
*  	Default value is TM_Automatic.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
ThresholdModeEnum DTKBARAPI BarcodeReader_get_ThresholdMode(BARREADER barReader);

/* Function: BarcodeReader_set_ThresholdCount
* Sets the number of thresholds to be used for recognition
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	thresholdCount - expected number of barcodes
*
* Notes:
*  	Default value is 8.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
void DTKBARAPI BarcodeReader_set_ThresholdCount(BARREADER barReader, int thresholdCount);

/* Function: BarcodeReader_get_ThresholdCount
* Gets the number of thresholds to be used for recognition
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Number of thresholds
*
* Notes:
*  	Default value is 8.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
int DTKBARAPI BarcodeReader_get_ThresholdCount(BARREADER barReader);

/* Function: BarcodeReader_set_ThresholdStep
* Sets the step for multiple thresholding
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	thresholdStep - step for multiple thresholding
*
* Notes:
*  	Default value is 16.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
void DTKBARAPI BarcodeReader_set_ThresholdStep(BARREADER barReader, int thresholdStep);

/* Function: BarcodeReader_get_ThresholdStep
* Gets the step for multiple thresholding
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Step for multiple thresholding
*
* Notes:
*  	Default value is 16.
*
*	See function <BarcodeReader_set_Threshold> for details.
*/
int DTKBARAPI BarcodeReader_get_ThresholdStep(BARREADER barReader);

/* Function: BarcodeReader_set_RecognitionTimeout
* Sets the recognition timeout
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	timeOut - number of seconds 
*
* Notes:
*  	Default value is 0.
*
*	Specifies the maximum number of seconds for the recognition process. If the recognition time exceeds this number of seconds, the process will be aborted.
* 
*	A value of 0 means that there is no time limit for the recognition process.
*/
void DTKBARAPI BarcodeReader_set_RecognitionTimeout(BARREADER barReader, int timeOut);

/* Function: BarcodeReader_get_RecognitionTimeout
* Gets the recognition timeout
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Notes:
*  	Default value is 0.
*
*	Specifies the maximum number of seconds for the recognition process. If the recognition time exceeds this number of seconds, the process will be aborted.
*
*	A value of 0 means that there is no time limit for the recognition process.
*/
int	DTKBARAPI BarcodeReader_get_RecognitionTimeout(BARREADER barReader);

/* Function: BarcodeReader_set_BarcodesToRead
* Sets the expected number of barcodes on single image
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	barcodesToRead - expected number of barcodes
*
* Notes:
*  	Default value is 1.
*
*	A value of 0 means that the maximum possible number of barcodes will be detected.
*
*	This parameter defines the expected number of barcodes on single image (page).
*	The number of barcodes detected may be greater than this value. This value is used as a condition for stopping the barcode reading process,
*	for example, if the expected number of barcodes is found during multiple thresholding, then recognition will be stopped without processing for all thresholds.
*/
void DTKBARAPI BarcodeReader_set_BarcodesToRead(BARREADER hBarReader, int barcodesToRead);

/* Function: BarcodeReader_get_BarcodesToRead
* Gets the expected number of barcodes on single image
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Expected number of barcodes
*
* Notes:
*  	Default value is 0.
*
*	A value of 0 means that the maximum possible number of barcodes will be detected.
*
*	This parameter defines the expected number of barcodes on single image (page).
*	The number of barcodes detected may be greater than this value. This value is used as a condition for stopping the barcode reading process,
*	for example, if the expected number of barcodes is found during multiple thresholding, then recognition will be stopped without processing for all thresholds.
*/
int DTKBARAPI BarcodeReader_get_BarcodesToRead(BARREADER hBarReader);

/* Function: BarcodeReader_set_ScanPage
* Sets the page number for processing
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	scanPage - page number
*
* Notes:
* 	Default value is 0.
*
*	A value 0 indicates to process all pages. Page numbers starts with 1.
*
*	This parameter is only meaningful for multipage documents.
*/
void DTKBARAPI BarcodeReader_set_ScanPage(BARREADER hBarReader, int scanPage);

/* Function: BarcodeReader_get_ScanPage
* Gets the page number for processing
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	 Page number
*
* Notes:
* 	Default value is 0.
*
*	A value 0 indicates to process all pages. Page numbers starts with 1.
*
*	This parameter is only meaningful for multipage documents.
*/
int DTKBARAPI BarcodeReader_get_ScanPage(BARREADER hBarReader);

/* Function: BarcodeReader_set_Code11HaveChecksum
* Sets the value indicating whether optional checksum validation is required for Code 11 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 11 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
void DTKBARAPI BarcodeReader_set_Code11HaveChecksum(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_Code11HaveChecksum
* Gets the value indicating whether optional checksum validation is required for Code 11 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 11 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
bool DTKBARAPI BarcodeReader_get_Code11HaveChecksum(BARREADER hBarReader);

/* Function: BarcodeReader_set_Code39HaveChecksum
* Sets the value indicating whether optional checksum validation is required for Code 39 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 39 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
void DTKBARAPI BarcodeReader_set_Code39HaveChecksum(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_Code39HaveChecksum
* Gets the value indicating whether optional checksum validation is required for Code 39 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 39 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
bool DTKBARAPI BarcodeReader_get_Code39HaveChecksum(BARREADER hBarReader);

/* Function: BarcodeReader_set_Code93HaveChecksum
* Sets the value indicating whether optional checksum validation is required for Code 93 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 93 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
void DTKBARAPI BarcodeReader_set_Code93HaveChecksum(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_Code93HaveChecksum
* Gets the value indicating whether optional checksum validation is required for Code 93 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Code 93 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
bool DTKBARAPI BarcodeReader_get_Code93HaveChecksum(BARREADER hBarReader);

/* Function: BarcodeReader_set_Inter2of5HaveChecksum
* Sets the value indicating whether optional checksum validation is required for Interleaved 2of5 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
* 	Default value is False.
*
*	This parameter is only meaningful if you have Interleaved 2of5 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
void DTKBARAPI BarcodeReader_set_Inter2of5HaveChecksum(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_Inter2of5HaveChecksum
* Gets the value indicating whether optional checksum validation is required for Interleaved 2of5 barcodes.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
*	Default value is False.
*
*	This parameter is only meaningful if you have Interleaved 2of5 barcodes generated with an option checksum. If this property is set to True, barcodes without a checksum will not be detected.
*/
bool DTKBARAPI BarcodeReader_get_Inter2of5HaveChecksum(BARREADER hBarReader);

/* Function: BarcodeReader_set_Code39NoStartStop
* Sets the value indicating whether Code 39 barcodes have start/stop symbols
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
*	Default value is False.
*
*	This parameter is only meaningful if you have Code 39 barcodes INCORRECTLY generated without start/stop symbols.
*
*	DO NOT use this property to process normal Code 39 barcodes.
*/
void DTKBARAPI BarcodeReader_set_Code39NoStartStop(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_Code39NoStartStop
* Gets the value indicating whether Code 39 barcodes have start/stop symbols
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
*	Default value is False.
*
*	This parameter is only meaningful if you have Code 39 barcodes INCORRECTLY generated without start/stop symbols.
*
*	DO NOT use this property to process normal Code 39 barcodes.
*/
bool DTKBARAPI BarcodeReader_get_Code39NoStartStop(BARREADER hBarReader);

/* Function: BarcodeReader_set_ConvertUPCEtoUPCA
* Sets the value indicating whether the UPC-E barcode must be converted to UPC-A format.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	bSet - Boolean value
*
* Notes:
*	Default value is False.
*/
void DTKBARAPI BarcodeReader_set_ConvertUPCEtoUPCA(BARREADER hBarReader, bool bSet);

/* Function: BarcodeReader_get_ConvertUPCEtoUPCA
* Gets the value indicating whether the UPC-E barcode must be converted to UPC-A format.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Boolean value
*
* Notes:
*	Default value is False.
*/
bool DTKBARAPI BarcodeReader_get_ConvertUPCEtoUPCA(BARREADER hBarReader);

/* Function: BarcodeReader_set_Inter2of5MinDigitsCount
* Sets the minimum digits count in Interleaved 2of5 barcode 
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	val - Minimum digits count
*
* Notes:
*	Default value is 6.
*/
void DTKBARAPI BarcodeReader_set_Inter2of5MinDigitsCount(BARREADER hBarReader, int val);

/* Function: BarcodeReader_get_Inter2of5MinDigitsCount
* Gets the minimum digits count in Interleaved 2of5 barcode 
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Minimum digits count in Interleaved 2of5 barcode
*
* Notes:
*	Default value is 6.
*/
int DTKBARAPI BarcodeReader_get_Inter2of5MinDigitsCount(BARREADER hBarReader);


/* Function: BarcodeReader_set_Orientation
* Sets the expected orientation of the barcode.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	orientation - one or a combination of <BarcodeOrientationEnum> enumeration values
*
* Notes:
*	Default value is BO_LetfToRight.
*
*	The barcode reader can detect barcodes in any orientation, although if you know that all barcodes will be oriented in a particular way, this can be used to speed up the recognition process.
*   For example, if you define the orientation of LetfToRight, this means that barcodes with a horizontal orientation (left-right direction) at an angle of +/- 45 degrees can be detected.
*/
void DTKBARAPI BarcodeReader_set_Orientation(BARREADER barReader, BarcodeOrientationEnum orientation);

/* Function: BarcodeReader_get_Orientation
* Gets the expected orientation of the barcode.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	One or a combination of <BarcodeOrientationEnum> enumeration values
*
* Notes:
*	Default value is BO_LetfToRight.
*
*	The barcode reader can detect barcodes in any orientation, although if you know that all barcodes will be oriented in a particular way, this can be used to speed up the recognition process.
*   For example, if you define the orientation of LetfToRight, this means that barcodes with a horizontal orientation (left-right direction) at an angle of +/- 45 degrees can be detected.
*/
BarcodeOrientationEnum DTKBARAPI BarcodeReader_get_Orientation(BARREADER barReader);

/* Function: BarcodeReader_set_ScanInterval
* Sets the image scanning interval.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	scanInterval - the image scanning interval in pixels.
*
* Notes:
*	Default value is 1.
*
*	Possible values are from 1 to 10.
*
*	A value of 1 means scanning each row or column of pixels in the image; a value of 2 means scanning every second row or column, and so on.
*	Increasing the value of this parameter can result in faster image processing, but increases the likelihood of not detecting narrow or poorly formed barcodes.
*	Decreasing the value of this parameter can have the opposite effect: slower image processing, but better barcode detection.
*/
void DTKBARAPI BarcodeReader_set_ScanInterval(BARREADER hBarReader, int scanInterval);

/* Function: BarcodeReader_get_ScanInterval
* Gets the image scanning interval.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	The image scanning interval in pixels.
*
* Notes:
*	Default value is 1.
*
*	Possible values are from 1 to 10.
*
*	A value of 1 means scanning each row or column of pixels in the image; a value of 2 means scanning every second row or column, and so on.
*	Increasing the value of this parameter can result in faster image processing, but increases the likelihood of not detecting narrow or poorly formed barcodes.
*	Decreasing the value of this parameter can have the opposite effect: slower image processing, but better barcode detection.
*/
int DTKBARAPI BarcodeReader_get_ScanInterval(BARREADER hBarReader);

/* Function: BarcodeReader_set_QRCodeRequiredECCLevel
* Sets the required ECC level for QR Codes detection
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	eccLevel - one of <QRCodeECCLevelEnum> enumeration values
*
* Notes:
*	Default value is QRECCLevel_Undefined.
*
*	This property can be used in some special cases where the error correction capability (ECC) of the QR code is known.
*/
void DTKBARAPI BarcodeReader_set_QRCodeRequiredECCLevel(BARREADER barReader, QRCodeECCLevelEnum eccLevel);

/* Function: BarcodeReader_get_QRCodeRequiredECCLevel
* Gets the required ECC level for QR Codes detection
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	One of <QRCodeECCLevelEnum> enumeration values
*
* Notes:
*	Default value is QRECCLevel_Undefined.
*
*	This property can be used in some special cases where the error correction capability (ECC) of the QR code is known.
*/
QRCodeECCLevelEnum DTKBARAPI BarcodeReader_get_QRCodeRequiredECCLevel(BARREADER barReader);

/* Function: BarcodeReader_set_QuietZoneSize
* Sets the expected size of the quiet zone of the barcode.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	quietZoneSize - one of <QuietZoneSizeEnum> enumeration values
*
* Notes:
*	Default value is QZ_Normal.
*
*	Quiet zone means the blank margin on either side of a barcode that is used to tell the barcode reader where a barcode starts and stops.
*	The purpose of a quiet zone is to prevent the reader from picking up information that does not pertain to the bar code that is being scanned.
*	As a rule, the quiet zone should be ten times the dimension of the narrowest element in the bar code. An undersized quiet zone may render a bar code unreadable.
*
*	The possible values of the QuietZoneSize parameter are: QZ_ExtraSmall, QZ_Small, QZ_Normal, QZ_Large.
*
*	The QZ_Normal is a standard quiet zone, as it defined by specification (approximately 10 times greater than the narrowest element in the barcode).
*	The other parameters QZ_Large, QZ_Small and QZ_ExtraSmall can be used if the quiet zone is abnormal, less or greater than defined in specification.
*
*	For example, the barcode can be printed in table or near some graphic object, in this case the barcode will not be detected by using QZ_Normal, and you should use QZ_Small or QZ_ExtraSmall to detect such barcode.
*
*	QZ_Small or QZ_ExtraSmall can slow down the recognition speed and can lead to more errors in recognition.
*
*	QZ_Large increases the speed and stability, but may not detect some barcodes with normal quiet zone. QZ_Large can be used if you know for sure that the quiet zones are large enough.
*/
void DTKBARAPI BarcodeReader_set_QuietZoneSize(BARREADER barReader, QuietZoneSizeEnum quietZoneSize);

/* Function: BarcodeReader_get_QuietZoneSize
* Gets the expected size of the quiet zone of the barcode.
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	One of <QuietZoneSizeEnum> enumeration values
*
* Notes:
*	Default value is QZ_Normal.
*
*	Quiet zone means the blank margin on either side of a barcode that is used to tell the barcode reader where a barcode starts and stops.
*	The purpose of a quiet zone is to prevent the reader from picking up information that does not pertain to the bar code that is being scanned.
*	As a rule, the quiet zone should be ten times the dimension of the narrowest element in the bar code. An undersized quiet zone may render a bar code unreadable.
*
*	The possible values of the QuietZoneSize parameter are: QZ_ExtraSmall, QZ_Small, QZ_Normal, QZ_Large.
*
*	The QZ_Normal is a standard quiet zone, as it defined by specification (approximately 10 times greater than the narrowest element in the barcode).
*	The other parameters QZ_Large, QZ_Small and QZ_ExtraSmall can be used if the quiet zone is abnormal, less or greater than defined in specification.
*
*	For example, the barcode can be printed in table or near some graphic object, in this case the barcode will not be detected by using QZ_Normal, and you should use QZ_Small or QZ_ExtraSmall to detect such barcode.
*
*	QZ_Small or QZ_ExtraSmall can slow down the recognition speed and can lead to more errors in recognition.
*
*	QZ_Large increases the speed and stability, but may not detect some barcodes with normal quiet zone. QZ_Large can be used if you know for sure that the quiet zones are large enough.
*/
QuietZoneSizeEnum DTKBARAPI BarcodeReader_get_QuietZoneSize(BARREADER barReader);

/* Function: BarcodeReader_set_ScanRectangleX
* Sets the X coordinate of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	x - X coordinate in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
void DTKBARAPI BarcodeReader_set_ScanRectangleX(BARREADER barReader, int x);

/* Function: BarcodeReader_get_ScanRectangleX
* Gets the X coordinate of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	X coordinate in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
int DTKBARAPI BarcodeReader_get_ScanRectangleX(BARREADER barReader);

/* Function: BarcodeReader_set_ScanRectangleY
* Sets the Y coordinate of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	y - Y coordinate in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
void DTKBARAPI BarcodeReader_set_ScanRectangleY(BARREADER barReader, int y);

/* Function: BarcodeReader_get_ScanRectangleY
* Gets the Y coordinate of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Y coordinate in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
int DTKBARAPI BarcodeReader_get_ScanRectangleY(BARREADER barReader);

/* Function: BarcodeReader_set_ScanRectangleWidth
* Sets the Width of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	width - width in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
void DTKBARAPI BarcodeReader_set_ScanRectangleWidth(BARREADER barReader, int width);

/* Function: BarcodeReader_get_ScanRectangleWidth
* Gets the Width of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Width in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
int DTKBARAPI BarcodeReader_get_ScanRectangleWidth(BARREADER barReader);

/* Function: BarcodeReader_set_ScanRectangleHeight
* Sets the Height of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	height - height in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
void DTKBARAPI BarcodeReader_set_ScanRectangleHeight(BARREADER barReader, int height);

/* Function: BarcodeReader_get_ScanRectangleHeight
* Gets the Height of the scan rectangle
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Height in pixels
*
* Notes:
*	Default value is 0.
*
*	The scan rectangle defines a rectangular area in the image for barcode detection. It should be used if you know the location of the barcode in the image.
*/
int DTKBARAPI BarcodeReader_get_ScanRectangleHeight(BARREADER barReader);

/* Function: BarcodeReader_set_PDFReadingType
* Sets the reading type of PDF documents
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	pdfReadingType - <PDFReadingTypeEnum> enumeration value
*
* Notes:
*	Default value is PDF_Images.
*
*	1) PDF_Render. The page of the PDF document will be rendered to single image with defined DPI (property PDFRenderDPI).
*	If the PDF page contains various objects such as text, graphics, images, etc., then all objects will be rendered to a single bitmap.
*
*	2) PDF_Images. All images located on the PDF page will be extracted and barcode recognition will be performed for each image separately.
*	Other non-image objects, such as text, will be ignored. A barcode can be created as a text object using a barcode font, and when using the PDF_Images option,
*	this barcode will not be recognized as only image objects will be processed.
*/
void DTKBARAPI BarcodeReader_set_PDFReadingType(BARREADER barReader, PDFReadingTypeEnum pdfReadingType);

/* Function: BarcodeReader_get_PDFReadingType
* Gets the reading type of PDF documents
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	<PDFReadingTypeEnum> enumeration value
*
* Notes:
*	Default value is PDF_Images.
*
*	1) PDF_Render. The page of the PDF document will be rendered to single image with defined DPI (property PDFRenderDPI).
*	If the PDF page contains various objects such as text, graphics, images, etc., then all objects will be rendered to a single bitmap.
*
*	2) PDF_Images. All images located on the PDF page will be extracted and barcode recognition will be performed for each image separately.
*	Other non-image objects, such as text, will be ignored. A barcode can be created as a text object using a barcode font, and when using the PDF_Images option,
*	this barcode will not be recognized as only image objects will be processed.
*/
PDFReadingTypeEnum DTKBARAPI BarcodeReader_get_PDFReadingType(BARREADER barReader);

/* Function: BarcodeReader_set_PDFRenderDPI
* Sets the DPI for PDF document pages rendering
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	dpi - DPI value
*
* Notes:
*	Default value is 300.
*
*	Used for PDFReadingType = PDF_Render
*/
void DTKBARAPI BarcodeReader_set_PDFRenderDPI(BARREADER barReader, int dpi);

/* Function: BarcodeReader_get_PDFRenderDPI
* Gets the DPI for PDF document pages rendering
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	DPI value
*
* Notes:
*	Default value is 300.
*
*	Used for PDFReadingType = PDF_Render
*/
int DTKBARAPI BarcodeReader_get_PDFRenderDPI(BARREADER barReader);

/* Function: BarcodeReader_Preprocessing_Count
* Gets the number of preprocessing operations
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Return:
*	Number of preprocessing operations
*
* Notes:
*	Default value is 0.
*
*	For more information, read <Preprocessing Operations>
*/
int DTKBARAPI BarcodeReader_Preprocessing_Count(BARREADER barReader);

/* Function: BarcodeReader_Preprocessing_Add
* Adds new preprocessing operations
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	op - <PreprocessingEnum> enumeration value
*	param1 - first parameter
*	param2 - second parameter
*
* Notes:
* 	For more information, read <Preprocessing Operations>
*/
void DTKBARAPI BarcodeReader_Preprocessing_Add(BARREADER barReader, PreprocessingEnum op, int param1, int param2);

/* Function: BarcodeReader_Preprocessing_Get
* Gets preprocessing operation and its parameters by index
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	index - index of preprocessing operation
*	op - pointer to <PreprocessingEnum> enumeration variable that receives operation value
*	param1 - pointer to an integer that receives the first parameter
*	param2 - pointer to an integer that receives the second parameter
*
* Notes:
* 	For more information, read <Preprocessing Operations>
*/
void DTKBARAPI BarcodeReader_Preprocessing_Get(BARREADER barReader, int index, PreprocessingEnum* op, int* param1, int* param2);

/* Function: BarcodeReader_Preprocessing_Set
* Sets preprocessing operation and its parameters by index
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	index - index of preprocessing operation
*	op - <PreprocessingEnum> enumeration value
*	param1 - first parameter
*	param2 - second parameter
*
* Notes:
* 	For more information, read <Preprocessing Operations>
*/
void DTKBARAPI BarcodeReader_Preprocessing_Set(BARREADER barReader, int index, PreprocessingEnum op, int param1, int param2);

/* Function: BarcodeReader_Preprocessing_Remove
* Removes preprocessing operation by index
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	index - index of preprocessing operation
*
* Notes:
* 	For more information, read <Preprocessing Operations>
*/
void DTKBARAPI BarcodeReader_Preprocessing_Remove(BARREADER barReader, int index);

/* Function: BarcodeReader_Preprocessing_Clear
* Removes all preprocessing operations
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*
* Notes:
* 	For more information, read <Preprocessing Operations>
*/
void DTKBARAPI BarcodeReader_Preprocessing_Clear(BARREADER barReader);

/* Function: BarcodeReader_GetSettingsXml
* Gets an XML string of all settings for the BarcodeReader object
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	xmlBuffer - Pointer to string buffer that receives XML string
*	xmlBufferSize - Size of string buffer
*/
int DTKBARAPI BarcodeReader_GetSettingsXml(BARREADER hBarReader, char* xmlBuffer, int xmlBufferSize);

/* Function: BarcodeReader_SetSettingsXml
* Sets the BarcodeReader settings from an XML string
*
* Parameters:
*	hBarReader - Handle of BarcodeReader object
*	xml - XML string
*
* Returns:
*	Value 0 on success, or -1 if the xml string have a wrong structure
*/
int DTKBARAPI BarcodeReader_SetSettingsXml(BARREADER hBarReader, const char* xml);

// ================================================================
// Group: Barcode Functions
// ================================================================

/* Function: Barcode_Destroy
* Destroys Barcode object instance
*
* Parameters:
*	hBarcode - Handle of Barcode object
*/
void DTKBARAPI Barcode_Destroy(BARCODE hBarcode);

/* Function: Barcode_get_String
* Gets the text of the detected barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*	buffer - Pointer to a buffer that receives the barcode text
*	bufferLen - Size of the string buffer
* 
* Returns:
*	Number of characters copied to buffer 
*/
int DTKBARAPI Barcode_get_Text(BARCODE hBarcode, char* buffer, int bufferLen);

/* Function: Barcode_get_BinaryData
* Gets the binary data of the detected barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*	buffer - Pointer to a buffer that receives the barcode binary data
*	bufferLen - Size of the string buffer
* 
* Returns:
*	Number of bytes copied to buffer 
*/
int	DTKBARAPI Barcode_get_BinaryData(BARCODE hBarcode, unsigned char* buffer, int bufferLen);

/* Function: Barcode_get_Type
* Gets the type of barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	<BarcodeTypeEnum> enumeration value
*/
BarcodeTypeEnum	DTKBARAPI Barcode_get_Type(BARCODE hBarcode);

/* Function: Barcode_get_TypeString
* Gets the type of barcode as string
*
* Parameters:
*	hBarcode - Handle of Barcode object
*	buffer - Pointer to a string buffer that receives the barcode type
*	bufferLen - Size of the string buffer
*
* Returns:
*	Number of characters copied to buffer
*/
int DTKBARAPI Barcode_get_TypeString(BARCODE hBarcode, char* buffer, int bufferLen);

/* Function: Barcode_get_Page
* Gets the page number of the detected barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Page number of the detected barcode
*
* Notes:
*	This property is only meaningful if you read barcodes from multi-page TIFF image or PDF document.
*/
int	DTKBARAPI Barcode_get_Page(BARCODE hBarcode);

/* Function: Barcode_get_Orientation
* Gets the orientation of recognized barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	 <BarcodeOrientationEnum> enumeration value
*/
BarcodeOrientationEnum DTKBARAPI Barcode_get_Orientation(BARCODE hBarcode);

/* Function: Barcode_get_Threshold
* Gets the threshold level that was used to detect this barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	 Threshold level
*/
int	DTKBARAPI Barcode_get_Threshold(BARCODE hBarcode);

/* Function: Barcode_get_BorderStartX1
* Gets the X coordinate of the first point of start boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	X coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderStartX1(BARCODE hBarcode);

/* Function: Barcode_get_BorderStartY1
* Gets the Y coordinate of the first point of start boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Y coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderStartY1(BARCODE hBarcode);

/* Function: Barcode_get_BorderStartX2
* Gets the X coordinate of the second point of start boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	X coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderStartX2(BARCODE hBarcode);

/* Function: Barcode_get_BorderStartY2
* Gets the Y coordinate of the second point of start boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Y coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderStartY2(BARCODE hBarcode);

/* Function: Barcode_get_BorderEndX1
* Gets the X coordinate of the first point of end boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	X coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderEndX1(BARCODE hBarcode);

/* Function: Barcode_get_BorderEndY1
* Gets the Y coordinate of the first point of end boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Y coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderEndY1(BARCODE hBarcode);

/* Function: Barcode_get_BorderEndX2
* Gets the X coordinate of the second point of end boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	X coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderEndX2(BARCODE hBarcode);

/* Function: Barcode_get_BorderEndY2
* Gets the Y coordinate of the second point of end boundary of the barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Y coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BorderEndY2(BARCODE hBarcode);

/* Function: Barcode_get_BoundingRectX
* Gets the X coordinate of the bounding rectangle of the barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	X coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BoundingRectX(BARCODE hBarcode);

/* Function: Barcode_get_BoundingRectY
* Gets the Y coordinate of the bounding rectangle of the barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Y coordinate in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BoundingRectY(BARCODE hBarcode);

/* Function: Barcode_get_BoundingRectWidth
* Gets the width of the bounding rectangle of the barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Width in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BoundingRectWidth(BARCODE hBarcode);

/* Function: Barcode_get_BoundingRectHeight
* Gets the height of the bounding rectangle of the barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Height in pixels
*
* Notes:
*	See <Barcode Location> section for details.
*/
int	DTKBARAPI Barcode_get_BoundingRectHeight(BARCODE hBarcode);

/* Function: Barcode_get_Angle
* Gets the skew angle of the detected barcode, in degrees.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Angle in degrees
*
* Notes:
*	See <Barcode Location> section for details.
*/
float DTKBARAPI Barcode_get_Angle(BARCODE hBarcode);

/* Function: Barcode_get_Checksum
* Gets the checksum of barcode
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Checksum value
*
* Notes:
*	This property applies to barcode with checksum.
*/
int	DTKBARAPI Barcode_get_Checksum(BARCODE hBarcode);

/* Function: Barcode_get_Dim1
* Gets the first dimension of the recognized 2D barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	First dimension of 2D barcode
*
* Notes:
*	This property only applies to Data Matrix and QR Code.
*/
int	DTKBARAPI Barcode_get_Dim1(BARCODE hBarcode);

/* Function: Barcode_get_Dim2
* Gets the second dimension of the recognized 2D barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Second dimension of 2D barcode
*
* Notes:
*	This property only applies to Data Matrix and QR Code.
*/
int	DTKBARAPI Barcode_get_Dim2(BARCODE hBarcode);

/* Function: Barcode_get_NumErrorsCorrected
* Gets the number of errors corrected when decoding a 2D barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Number of errors corrected
*/
int	DTKBARAPI Barcode_get_NumErrorsCorrected(BARCODE hBarcode);

/* Function: Barcode_get_ErrorCorrectionLevel
* Gets the error correction level of the recognized 2D barcode.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Error correction level
*/
int	DTKBARAPI Barcode_get_ErrorCorrectionLevel(BARCODE hBarcode);

/* Function: Barcode_get_QRVersion
* Gets the version of recognized QR Code
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Version number
*/
int	DTKBARAPI Barcode_get_QRVersion(BARCODE hBarcode);

/* Function: Barcode_get_RSSExpandedIsStacked
* Gets a value that indicates whether the RSS Expanded barcode is Stacked.
*
* Parameters:
*	hBarcode - Handle of Barcode object
*
* Returns:
*	Boolean value
*/
bool DTKBARAPI Barcode_get_RSSExpandedIsStacked(BARCODE hBarcode);


// ================================================================
// Group: BarReaderResult Functions
// ================================================================

/* Function: BarReaderResult_Destroy
* Destroy BarReaderResult object
*
* Parameters:
*	hResult - Handle of BarReaderResult object
*
* Notes:
*	There is not possibility to create BarReaderResult object, the BarcodeReader only can create this object, you need to destroy it when you do not need it anymore.
*/
void DTKBARAPI BarReaderResult_Destroy(BARREADERRESULT hResult);

/* Function: BarReaderResult_get_BarcodesCount
* Get number of Barcode objects containing in BarReaderResult object
*
* Parameters:
*	hResult - Handle of BarReaderResult object
*
* Returns:
*	Number of Barcode objects
*/
int DTKBARAPI BarReaderResult_get_BarcodesCount(BARREADERRESULT hResult);

/* Function: BarReaderResult_get_Barcode
* Get Barcode object from BarReaderResult by it's index
*
* Parameters:
*	hResult - Handle of BarReaderResult object
*	index - Index of Barcode object
*
* Returns:
*	Handle of Barcode object
*
* Notes:
*	Each Barcode object from BarReaderResult must be destroyed using \ref Barcode_Destroy function
*/
BARCODE DTKBARAPI BarReaderResult_get_Barcode(BARREADERRESULT hResult, int index);


/* Function: BarReaderResult_get_ErrorCode
* Get error code
*
* Parameters:
*	hResult - Handle of BarReaderResult object
*
* Returns:
*	Value 0 on success, otherwise error code.
* 
*	Below is a list of possible error codes:
* 
*	DTK_E_BUFFER_IS_NULL  			7101
* 
*	DTK_E_INVALID_SCAN_RECT  		7102
* 
*	DTK_E_FILENAME  				7103
* 
*	DTK_E_TIMEOUT  					7104
* 
*	DTK_E_DECODE_IMAGE				7105
* 
*	DTK_E_PIXEL_FORMAT				7106
* 
*	DTK_E_TIFF_DECODE				7107
* 
*	DTK_E_NO_LICENSE				7201
* 
*	DTK_E_VALIDATE_TRIAL			7202
* 
*	DTK_E_PDF_CANNOT_OPEN			7301
* 
*	DTK_E_PDF_WRONG_PASSWORD		7302
* 
*	DTK_E_PDF_WRONG_PAGENUM			7303
*
*/
int DTKBARAPI BarReaderResult_get_ErrorCode(BARREADERRESULT hResult);


// =============================
// Group: License Functions
// =============================

/* Function: BarcodeReader_ActivateLicenseOnline
* Activates license on-line
*
* Parameters:
*	licenseKey - String containing the license key
*	comments - String containing comments to assign to activation (optional)
*
* Returns:
*	0 on success, error code otherwise
*
* Notes:
*	The Internet connection (host: dtksoft.com port: 80) is required to perform license activation.
*
*	The optional comments parameter can be used to identify certain license activation, for example machine name or customer name can be used.
*/
int DTKBARAPI BarcodeReader_ActivateLicenseOnline(const char* licenseKey, const char* comments);

/* Function: BarcodeReader_GetActivationLink
* Returns activation link to activate the defined license key
*
* Parameters:
*	licenseKey - String which contain license key
*	comments - String which contain comments assigned to activation (optional)
*	activationLink - Pointer to string buffer which receives activation link
*	size - Size of string buffer
*
* Returns:
*	Number of bytes copied to buffer. If activationLink = NULL or size = 0, then function return number of bytes required for store result string
*
* Notes:
*	The Internet connection (host: dtksoft.com port: 80) is required to perform license activation.
*
*	The optional comments parameter can be used to identify certain license activation, for example machine name or customer name can be used.
*/
int DTKBARAPI BarcodeReader_GetActivationLink(const char* licenseKey, const char* comments, char* activationLink, int size);

/* Function: BarcodeReader_ActivateLicenseOffline
* Activates license on machine using activation code
*
* Parameters:
*	activationCode - String which contain activation code
*
* Returns:
*	0 on success, error code otherwise
*
* Notes:
*	The optional comments parameter can be used to identify certain license activation, for example machine name or customer name can be used.
*/
int DTKBARAPI BarcodeReader_ActivateLicenseOffline(const char* activationCode);

/* Function: BarcodeReader_GetSystemID
* Gets the current machine/system ID
*
* Parameters:
*	system_id - Pointer to string buffer which receives current machine ID (fingerprint)
*	system_id_size - Size of string buffer
*
* Returns:
*	Number of bytes copied to buffer. If system_id = NULL or system_id_size = 0, then function return number of bytes required for store result string
*/
int DTKBARAPI BarcodeReader_GetSystemID(char* system_id, int system_id_size);

/* Function: BarcodeReader_GetActivatedLicenseInfo [deprecated]
* Gets the information of the currently activated license: license key, comments, license type, expiration date (if applicable)
*
* Parameters:
*	license_key - Pointer to string buffer which receives license key activated on current machine
*	license_key_max_len - Size of string buffer
*	comments - Pointer to string buffer which receives activation comments
*	comments_max_len - Size of string buffer
*	license_type - Pointer to integer variable which receives type of activated license
*	expirationDate - Pointer to 64bit integer value (time_t C++ type) which receives expiration date of activated license
*
* Notes:
*	If expirationDate value is 0, then license does not expire.
*
*	The license type possible values:
*
*	1 - Workstation 1D
*
*	2 - Workstation 1D2D
*
*	3 - Unlimited Workstation 1D
*
*	4 - Unlimited Workstation 1D1D2D
*
*	5 - Server 1D
*
*	6 - Server 1D2D
*
*	7 - Unlimited Server 1D
*
*	8 - Unlimited Server 1D2D
* 
* 
*	WARNING: THIS FUNCTION IS DEPRECATED, PLEASE USE <GetLicenseInfo> INSTEAD.
*/
void DTKBARAPI BarcodeReader_GetActivatedLicenseInfo(char* license_key, int license_key_max_len, char* comments, int comments_max_len, int* license_type, time_t* expirationDate);

/* Function: BarcodeReader_GetLicenseInfo
* Gets information about a license, either an activated software license or an plugged in USB dongle.
*
* Parameters:
*	license_key - Pointer to string buffer which receives license key activated on current machine
*	license_key_max_len - Size of string buffer
*	comments - Pointer to string buffer which receives activation comments
*	comments_max_len - Size of string buffer
*	license_type - Pointer to integer variable which receives type of activated license
*	expirationDate - Pointer to 64bit integer value (time_t C++ type) which receives expiration date of activated license
*	usb_dongle_id - Pointer to string buffer which receives USB dongle id number
*	usb_dongle_id_len - Size of string buffer
*	valid - Pointer to integer variable which receives boolean value 1 (true) or 0 (false), indicating whether the license is valid or not.
* 
* Notes:
*	If expirationDate value is 0, then license does not expire.
*
*	The license type possible values:
*
*	1 - Workstation 1D
*
*	2 - Workstation 1D2D
*
*	3 - Unlimited Workstation 1D
*
*	4 - Unlimited Workstation 1D1D2D
*
*	5 - Server 1D
*
*	6 - Server 1D2D
*
*	7 - Unlimited Server 1D
*
*	8 - Unlimited Server 1D2D
*
*	If the 'valid' parameter is false, it means that this license is not suitable for the version of the library you are using. You need to renew your license or use an older version of the library.
*
*	If the license is not on a USB dongle, then usb_dongle_id will be an empty string.
*/
void DTKBARAPI BarcodeReader_GetLicenseInfo(
	char* license_key, int license_key_max_len,
	char* comments, int comments_max_len,
	int* license_type,
	time_t* expirationDate,
	char* usb_dongle_id, int usb_dongle_id_len,
	int* valid);


/* Function: BarcodeReader_GetLibraryVersion
* Return library version
*
* Parameters:
*	buffer - Pointer to string buffer which receives library version
*	buff_size - Size of string buffer
*
* Returns:
*	Number of bytes copied to buffer. If buffer = NULL or buff_size = 0, then function return number of bytes required to store the result string
*
*/
int DTKBARAPI BarcodeReader_GetLibraryVersion(char* buffer, int buff_size);

// ================================================================
// Group: Other Functions
// ================================================================

/* Function: PDFOpenFromFile
* Opens PDF document
*
* Parameters:
*	fileName - File name
*	password - Password (optional)
*	errorCode - Pointer to integer what receives error code
*
* Returns:
*	Handle of PDF document
*/
void* DTKBARAPI PDFOpenFromFile(const char* fileName, const char* password, int* errorCode);

/* Function: PDFOpenFromMemFile
* Opens PDF document from memory file
*
* Parameters:
*	pBuffer - Pointer to a memory buffer containing the PDF file data
*	bufferSize - size of memory buffer in bytes
*	password - Password (optional)
*	errorCode - Pointer to integer what receives error code
*
* Returns:
*	Handle of PDF document
*/
void* DTKBARAPI PDFOpenFromMemFile(void* pBuffer, int bufferSize, const char* password, int* errorCode);

/* Function: PDFClose
* Closes PDF document
*
* Parameters:
*	hPDF - Handle of PDF document
*/
void DTKBARAPI PDFClose(void* hPDF);

/* Function: PDFGetPagesCount
* Gets number of pages in PDF document
*
* Parameters:
*	hPDF - Handle of PDF document
*
* Returns:
*	Number of pages
*/
int DTKBARAPI PDFGetPagesCount(void* hPDF);

/* Function: PDFGetPageImageBuffer
* Gets the image buffer of a rendered page of PDF document
*
* Parameters:
*	hPDF - Handle of PDF document
*	page - Page number (zero-based)
*	dpi - Rendering DPI
*	pixelFormat - <PixelFormatEnum> enumeration value
*	pImageBuffer - Pointer to a pointer variable that receives the image buffer containing pixel data of the frame image.
*	width - Pointer to an integer variable that receives the width of the frame image.
*	height - Pointer to an integer variable that receives the height of the frame image.
*	stride - Pointer to an integer variable that receives the size of one image row in bytes.
* 
* Returns:
*	Number of pages
* 
* Notes:
*	You need to call <PDFFreeImageBuffer> with received pointer to free memory.
*
*/
int DTKBARAPI PDFGetPageImageBuffer(void* hPDF, int page, int dpi, PixelFormatEnum pixelFormat, void** pImageBuffer, int* width, int* height, int* stride);

/* Function: PDFFreeImageBuffer
* Free image buffer
*
* Parameters:
*	pImageBuffer - Pointer to image buffer
*
*/
void DTKBARAPI PDFFreeImageBuffer(void* pImageBuffer);

#ifdef __cplusplus
}
#endif

#endif /* __DTKBarReader5_H */

/*
* Title: Preprocessing Operations
*
* The BarcodeReader have a set of preprocessing operations what can be executed before actual barcode recognition process.
*
* There are the following set of functions to work with preprocessing operations.
*
* <BarcodeReader_Preprocessing_Count>
*
* <BarcodeReader_Preprocessing_Add>
*
* <BarcodeReader_Preprocessing_Get>
*
* <BarcodeReader_Preprocessing_Set>
*
* <BarcodeReader_Preprocessing_Remove>
*
* <BarcodeReader_Preprocessing_Clear>
*
*
* All available preprocessing operations defined in <PreprocessingEnum> enumeration.
*
* PR_Erosion - perform morphological Erosion, 1st parameter is kernel size. Value 1 - 3x3 kernel, 2 - 5x5, 3 - 7x7, etc. 2nd parameter not used.
*
* PR_Dilation - perform morphological Dilation, 1st parameter is kernel size. Value 1 - 3x3 kernel, 2 - 5x5, 3 - 7x7, etc. 2nd parameter not used.
*
* PR_Despeckle - remove speckles from image, 1st parameter is kernel size. Value 1 - 3x3 kernel, 2 - 5x5, 3 - 7x7, etc. 2nd parameter not used.
*
* PR_Sharp - make an image sharper, no parameters (1st and 2nd parameters are not used)
*
* PR_Invert - invert image, no parameters (1st and 2nd parameters are not used)
* 
* PR_FlipHorizontal - flip the image by horizontally, no parameters (1st and 2nd parameters are not used)
*
* PR_FlipVertical - flip the image by vertically, no parameters (1st and 2nd parameters are not used)
*
* Example:
* --- Code
* // Do despeckle and then inversion
* BarcodeReader_Preprocessing_Clear(hBarReader);
* BarcodeReader_Preprocessing_Add(hBarReader, PR_Despeckle, 1, 0);
* BarcodeReader_Preprocessing_Add(hBarReader, PR_Invert, 0, 0);
* ---
*/

/*
* Title: Barcode Location
*
* (see barcode_bbox.jpg)
*
* (see barcode_angle.jpg)
*
*/
