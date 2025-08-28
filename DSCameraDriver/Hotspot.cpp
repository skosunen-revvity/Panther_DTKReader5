// --------------------------------------------------------------------------
// Hotspot.cpp
// --------------------------------------------------------------------------
//
// Hotspot implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 11.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#include "stdafx.h"
#include "hotspot.h"


// --------------------------------------------------------------------------
// Static methods

USHORT Hotspot::GetIndex(eInputImageType imageType, const PSPOTLIGHTCOLOR color) {

  if(imageType == ColorImage) {

    // TODO: Mitäs me tehään kun o värikuva - pitäiskö meitin rakentaa tsydeemi jossa jokaisen
    //       kanavan min ja max voidaan asettaa erikseen

    return color->r + color->g + color->b;
  } else {
    // In grayscale mode, only the red channel is used
    return color->r;

  }

}

// --------------------------------------------------------------------------

Hotspot::Hotspot() {

  m_pHotspotData        = NULL;

  m_ImageType           = ColorImage;
  m_RequiredChangePercent      = 0;
  m_ImageWidth          = 0;

  m_DetectionAccuracyX  = 0;;
  m_DetectionAccuracyY  = 0;

  ZeroMemory(&m_TopLeft, sizeof(POINT));
  ZeroMemory(&m_BottomRight, sizeof(POINT));
  ZeroMemory(&m_Accepted, sizeof(INDEXVALUERANGE));

}

Hotspot::~Hotspot() {

  ReleaseHotspotData();

}

void Hotspot::Create(const PPOINT pTopLeft, const PPOINT pBottomRight, const PINDEXVALUERANGE pAccepted,
  eInputImageType imageType, BYTE invalidPercent, USHORT sourceWidth, BYTE detectionAccuracyX, BYTE detectionAccuracyY,
  const PBYTE pSource) {

  m_DetectionAccuracyX = detectionAccuracyX;
  m_DetectionAccuracyY = detectionAccuracyY;

  m_ImageWidth      = sourceWidth;
  m_ImageType       = imageType;
  m_Accepted.dwMin  = pAccepted->dwMin;
  m_Accepted.dwMax  = pAccepted->dwMax;

  m_RequiredChangePercent  = invalidPercent;

  CreateHotspotData(pTopLeft, pBottomRight);
  CopyHotspotData(sourceWidth, pSource);

}

BYTE Hotspot::IsSame(const PBYTE pSource) {

  BYTE same = FALSE;
  SHORT _x;
  SHORT _y;

  USHORT currentIntensityValue;
  USHORT orginalIntensityValue;

  INT changedPixels = 0;
  INT totalPixels   = 0;
  USHORT width      = (USHORT)(m_BottomRight.x - m_TopLeft.x);

  PSPOTLIGHTCOLOR pImg  = (PSPOTLIGHTCOLOR)pSource;
  int tmpY;
  int tmp_Y;

  for(SHORT y = (SHORT)m_TopLeft.y; y < m_BottomRight.y; y += m_DetectionAccuracyY) {
    
    _y    = (SHORT)(y - m_TopLeft.y);

    tmpY  = y * m_ImageWidth;
    tmp_Y = _y * width;

    SHORT _min;
    SHORT _max;


    for(SHORT x = (SHORT)m_TopLeft.x;  x < m_BottomRight.x; x += m_DetectionAccuracyX ) {
      _x = (SHORT)(x - m_TopLeft.x);

      currentIntensityValue = GetIndex(m_ImageType, &pImg[tmpY + x]);
      orginalIntensityValue = m_pHotspotData[tmp_Y + _x];

      totalPixels++;

      // TODO: Nää arvot vois laskea valmiiksi tonne m_pHotspotData
      _min = (SHORT)orginalIntensityValue - (SHORT)m_Accepted.dwMin;
      _max = (SHORT)orginalIntensityValue + (SHORT)m_Accepted.dwMax;

      if( currentIntensityValue < _min || currentIntensityValue > _max ) {
        changedPixels++;
      }
    }
  }

  if(m_RequiredChangePercent > 0) {
    return (BYTE)((changedPixels * 100) / totalPixels) <= m_RequiredChangePercent ? TRUE : FALSE;
  } else {
    return changedPixels == 0;
  }

}

void Hotspot::ReleaseHotspotData() {

  if(m_pHotspotData != NULL) {
    delete []m_pHotspotData;
    m_pHotspotData = NULL;
  }

  ZeroMemory(&m_TopLeft, sizeof(POINT));
  ZeroMemory(&m_BottomRight, sizeof(POINT));

}

void Hotspot::CreateHotspotData(const PPOINT pTopLeft, const PPOINT pBottomRight) {

  ReleaseHotspotData();

  m_TopLeft.x = pTopLeft->x;
  m_TopLeft.y = pTopLeft->y;

  m_BottomRight.x = pBottomRight->x;
  m_BottomRight.y = pBottomRight->y;

  USHORT width  = (USHORT)(m_BottomRight.x - m_TopLeft.x);
  USHORT height = (USHORT)(m_BottomRight.y - m_TopLeft.y);

  m_pHotspotData = new USHORT[height * width];

}

void Hotspot::CopyHotspotData(USHORT sourceWidth, const PBYTE pSource) {

  USHORT _x;
  USHORT _y;

  USHORT width = (USHORT)(m_BottomRight.x - m_TopLeft.x);
  PSPOTLIGHTCOLOR pImg  = (PSPOTLIGHTCOLOR)pSource;

  for(USHORT y = (USHORT)m_TopLeft.y; y < m_BottomRight.y; y += m_DetectionAccuracyY) {
    _y = (USHORT)(y - m_TopLeft.y);
    for(USHORT x = (USHORT)m_TopLeft.x; x < m_BottomRight.x; x += m_DetectionAccuracyX) {
      _x = (USHORT)(x - m_TopLeft.x);
      m_pHotspotData[(_y * width) + _x] = GetIndex(m_ImageType, &pImg[(y * sourceWidth) + x]);
    }
  }

}
