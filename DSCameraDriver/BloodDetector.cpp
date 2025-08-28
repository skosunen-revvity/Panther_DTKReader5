// --------------------------------------------------------------------------
// BloodDetector.cpp
// --------------------------------------------------------------------------
//
// Class for detecting blood from the specified bitmap
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
#include "BloodDetector.h"
#include "ExceptionInfo.h"


// --------------------------------------------------------------------------

// Values which are used in the blood map
#define BLOODDETECTOR_BLOODMAP_BLOOD    1   // Position contains blood
#define BLOODDETECTOR_BLOODMAP_EMPTY    0   // Position is empty

// Class name, used as a param to exception methods
_CLASSNAME(_T("BloodDetector"));

// --------------------------------------------------------------------------


BloodDetector::BloodDetector() {

  m_bInitialized            = FALSE;

  m_usSearchAreaRadius      = 0;
  m_usImageWidth            = 0;

  m_pBloodMap               = NULL;
  m_usBloodMapWidth         = 0;
  m_usBloodMapHeight        = 0;

  m_bdmDetectionMethod      = DetectBlood;
  m_InputType               = ColorImage;
  m_pSearchMap              = NULL;
  m_BloodDetectionPercent   = 0;

  ZeroMemory(&m_crRedColorRange, sizeof(COLOR_RANGE));
  ZeroMemory(&m_crGreenColorRange, sizeof(COLOR_RANGE));
  ZeroMemory(&m_crBlueColorRange, sizeof(COLOR_RANGE));

  ZeroMemory(&m_ptSearchAreaCenter, sizeof(POINT));
  ZeroMemory(&m_ptSearchAreaTopLeft, sizeof(POINT));
  ZeroMemory(&m_ptSearchAreaBottomRight, sizeof(POINT));

}

BloodDetector::~BloodDetector() {

  ReleaseBloodMap();

  if(m_pSearchMap != NULL) {
    delete m_pSearchMap;
    m_pSearchMap = NULL;
  }

}

void BloodDetector::Initialize(const PBLOODDETECTORSETTINGS pSettings) {

  _METHODNAME(_T("Initialize"));
  ThrowAlreadyInitializedException(METHODNAME);

  m_bInitialized = FALSE;

  // Cache settings
  CopySettings(pSettings);

  // Init blood map and calc its size
  InitializeBloodMap();

  // Init helper - used to tell where blood should be searched
  InitializeSearchMap();

  m_bInitialized = TRUE;

}

void BloodDetector::Release() {

  _METHODNAME(_T("Release"));
  ThrowNotInitializedException(METHODNAME);

  if(m_pSearchMap != NULL) {
    delete m_pSearchMap;
    m_pSearchMap = NULL;
  }

  m_bInitialized = FALSE;
  ReleaseBloodMap();

}

void BloodDetector::GetCurrentSettings(PBLOODDETECTORSETTINGS pSettings) {

  _METHODNAME(_T("GetCurrentSettings"));
  ThrowNotInitializedException(METHODNAME);

  pSettings->bdmDetectionMethod       = m_bdmDetectionMethod;
  pSettings->bloodDetectionPercent    = m_BloodDetectionPercent;

  pSettings->BloodColorMin            = RGB( m_crRedColorRange.usMinColor,
                                             m_crGreenColorRange.usMinColor,
                                             m_crBlueColorRange.usMinColor );

  pSettings->BloodColorMax            = RGB( m_crRedColorRange.usMaxColor,
                                             m_crGreenColorRange.usMaxColor,
                                             m_crBlueColorRange.usMaxColor );

  pSettings->ImageType                = m_InputType;
  pSettings->SearchAreaBottomRight    = m_ptSearchAreaBottomRight;
  pSettings->SearchAreaCenter         = m_ptSearchAreaCenter;
  pSettings->SearchAreaTopLeft        = m_ptSearchAreaTopLeft;
  pSettings->ImageWidth               = m_usImageWidth;
  pSettings->SearchAreaRadius         = m_usSearchAreaRadius;

}

void BloodDetector::UpdateSettings(const PBLOODDETECTORSETTINGS pSettings) {

  _METHODNAME(_T("UpdateSettings"));
  ThrowNotInitializedException(METHODNAME);

  CopySettings(pSettings);

  // Init blood map and calc its size
  InitializeBloodMap();
  InitializeSearchMap();

}

BYTE BloodDetector::Detect(const PBYTE pData) {

  _METHODNAME(_T("Detect"));
  ThrowNotInitializedException(METHODNAME);

  if(pData == NULL) {
    // Image data is missing
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BLOODDETECTOR_IMAGE_DATA_IS_NULL);
  }

  PSPOTLIGHTCOLOR pImg  = (PSPOTLIGHTCOLOR)pData;
  BYTE bloodDetected    = FALSE;
  DWORD total           = 0;
  DWORD blood           = 0;
  
  BYTE mapValue;

  try {

    int xEnd = m_ptSearchAreaBottomRight.x;
    int yEnd = m_ptSearchAreaBottomRight.y;
    int tmp;

    for(int y = m_ptSearchAreaTopLeft.y; y < yEnd; y++) {

      tmp = y * m_usImageWidth;
      for(int x = m_ptSearchAreaTopLeft.x; x < xEnd; x++) {

        mapValue = BLOODDETECTOR_BLOODMAP_EMPTY;

        // Check if inside search area
        if(m_pSearchMap->IsSearchable(x, y)) {
          if(IsBlood(&pImg[tmp + x])) {
            // Blood!
            bloodDetected = TRUE;
            blood++;
            mapValue = BLOODDETECTOR_BLOODMAP_BLOOD;
          }
        }

        total++;

        // Store detection results
        SetBloodMapValue(x, y, mapValue);
      }
    }

  } catch (ExceptionInfo* pEI) {
    InitializeBloodMap();
    throw pEI;
  } catch (...) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BLOODDETECTOR_IMAGE_DATA_ACCESS_VIOLATION);
  }


  if(m_BloodDetectionPercent > 0) {
    if(total > 0 && blood > 0) {
      // Return TRUE, if enough pixels are blood
      return (BYTE)((blood * 100) / total) >= m_BloodDetectionPercent ? TRUE : FALSE;
    }
  }

  return bloodDetected;

}

BYTE BloodDetector::IsBlood(USHORT x, USHORT y) {

  _METHODNAME(_T("IsBlood"));
  ThrowNotInitializedException(METHODNAME);

  // Check from blood map if specified pixel is blood
  return GetBloodMapValue(x, y) == BLOODDETECTOR_BLOODMAP_BLOOD ? TRUE : FALSE;

}

BYTE BloodDetector::IsBlood(const PSPOTLIGHTCOLOR color) {

  if(m_InputType == ColorImage) {

    // When input type is color, all channels are used
    if( color->r < m_crRedColorRange.usMinColor || color->r > m_crRedColorRange.usMaxColor ||
        color->g < m_crGreenColorRange.usMinColor || color->g > m_crGreenColorRange.usMaxColor ||
        color->b < m_crBlueColorRange.usMinColor || color->b > m_crBlueColorRange.usMaxColor ) {

        return FALSE;
    } else {
      return TRUE;
    }

  } else {

    // In grayscale mode, only the red channel is used
    return !(color->r < m_crRedColorRange.usMinColor || color->r > m_crRedColorRange.usMaxColor);

  }

}

BYTE BloodDetector::GetBloodMapValue(USHORT x, USHORT y) {

  // TODO: otettu pois käytöstä ku yritettään optimoida hieman
  //       täytyy fundeerata jos otettia käyttöön vaikka toisaalta
  //       noita exceptioneja ei oo koskaa tullut


  //_METHODNAME(_T("GetBloodMapValue"));

  //if(m_pBloodMap == NULL) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BLOODDETECTOR_BLOODMAP_IS_NULL);
  //}

  //if(x < m_ptSearchAreaTopLeft.x || x >= m_ptSearchAreaBottomRight.x) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_INVALID_X_COORD);
  //}

  //if(y < m_ptSearchAreaTopLeft.y || y >= m_ptSearchAreaBottomRight.y) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_INVALID_Y_COORD);
  //}

  int usedX = x - m_ptSearchAreaTopLeft.x;
  int usedY = y - m_ptSearchAreaTopLeft.y;

  return m_pBloodMap[(usedY * m_usBloodMapWidth) + usedX];

}

void BloodDetector::SetBloodMapValue(USHORT x, USHORT y, BYTE value) {

  // TODO: otettu pois käytöstä ku yritettään optimoida hieman
  //       täytyy fundeerata jos otettia käyttöön vaikka toisaalta
  //       noita exceptioneja ei oo koskaa tullut


  //_METHODNAME(_T("SetBloodMapValue"));

  //if(m_pBloodMap == NULL) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_BLOODDETECTOR_BLOODMAP_IS_NULL);
  //}

  //if(x < m_ptSearchAreaTopLeft.x || x > m_ptSearchAreaBottomRight.x) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_INVALID_X_COORD);
  //}

  //if(y < m_ptSearchAreaTopLeft.y || y > m_ptSearchAreaBottomRight.y) {
  //  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_INVALID_Y_COORD);
  //}

  int usedX = x - m_ptSearchAreaTopLeft.x;
  int usedY = y - m_ptSearchAreaTopLeft.y;

  m_pBloodMap[usedY * m_usBloodMapWidth + usedX] = value;

}

void BloodDetector::CopySettings(const PBLOODDETECTORSETTINGS pSettings) {

  m_bdmDetectionMethod            = pSettings->bdmDetectionMethod;
  m_InputType                     = pSettings->ImageType;
  
  m_usImageWidth                  = pSettings->ImageWidth;
  m_BloodDetectionPercent         = pSettings->bloodDetectionPercent;

  m_usSearchAreaRadius            = pSettings->SearchAreaRadius;

  m_ptSearchAreaCenter.x          = pSettings->SearchAreaCenter.x;
  m_ptSearchAreaCenter.y          = pSettings->SearchAreaCenter.y;

  m_ptSearchAreaBottomRight.x     = pSettings->SearchAreaBottomRight.x;
  m_ptSearchAreaBottomRight.y     = pSettings->SearchAreaBottomRight.y;

  m_ptSearchAreaTopLeft.x         = pSettings->SearchAreaTopLeft.x;
  m_ptSearchAreaTopLeft.y         = pSettings->SearchAreaTopLeft.y;

  m_crRedColorRange.usMinColor    = GetRValue(pSettings->BloodColorMin);
  m_crRedColorRange.usMaxColor    = GetRValue(pSettings->BloodColorMax);

  m_crGreenColorRange.usMinColor  = GetGValue(pSettings->BloodColorMin);;
  m_crGreenColorRange.usMaxColor  = GetGValue(pSettings->BloodColorMax);;

  m_crBlueColorRange.usMinColor   = GetBValue(pSettings->BloodColorMin);;
  m_crBlueColorRange.usMaxColor   = GetBValue(pSettings->BloodColorMax);;

}

void BloodDetector::InitializeBloodMap() {

  ReleaseBloodMap();

  m_usBloodMapWidth   = (USHORT)(m_ptSearchAreaBottomRight.x - m_ptSearchAreaTopLeft.x);
  m_usBloodMapHeight  = (USHORT)(m_ptSearchAreaBottomRight.y - m_ptSearchAreaTopLeft.y);

  int mapLenght       = m_usBloodMapWidth * m_usBloodMapHeight;
  m_pBloodMap         = new BYTE[mapLenght];

  memset(m_pBloodMap, BLOODDETECTOR_BLOODMAP_EMPTY, mapLenght);

}

void BloodDetector::ReleaseBloodMap() {

  if(m_pBloodMap != NULL) {
    delete []m_pBloodMap;
    m_pBloodMap = NULL;
  }

  m_usBloodMapWidth   = 0;
  m_usBloodMapHeight  = 0;

}

void BloodDetector::ThrowNotInitializedException(const PTCHAR pszMethod) {

  if(!m_bInitialized) {
    ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_BLOODDETECTOR_NOT_INITIALIZED);
  }

}

void BloodDetector::ThrowAlreadyInitializedException(const PTCHAR pszMethod) {

  if(m_bInitialized) {
    ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_BLOODDETECTOR_BD_ALREADY_INITIALIZED);
  }

}

void BloodDetector::InitializeSearchMap() {

  if(m_pSearchMap != NULL) {
    delete m_pSearchMap;
  }

  BOUNDS b;
  b.left    = m_ptSearchAreaTopLeft.x;
  b.top     = m_ptSearchAreaTopLeft.y;
  b.right   = m_ptSearchAreaBottomRight.x;
  b.bottom  = m_ptSearchAreaBottomRight.y;

  m_pSearchMap = new SearchMap();
  m_pSearchMap->BuildCircleMap((USHORT)m_ptSearchAreaCenter.x, (USHORT)m_ptSearchAreaCenter.y,
    m_usSearchAreaRadius, &b);

}

BYTE  BloodDetector::GetBloodDetectionColorMax() {
	int sum = m_crRedColorRange.usMaxColor + m_crGreenColorRange.usMaxColor + m_crBlueColorRange.usMaxColor;

	return sum / 3;
}

void BloodDetector::SetBloodDetectionColorMax(BYTE value) {
	m_crRedColorRange.usMaxColor = value;
	m_crGreenColorRange.usMaxColor = value;
	m_crBlueColorRange.usMaxColor = value;
}
