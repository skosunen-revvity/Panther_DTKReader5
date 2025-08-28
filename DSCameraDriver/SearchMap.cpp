// --------------------------------------------------------------------------
// SearchMap.cpp
// --------------------------------------------------------------------------
//
// SearchMap implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 11.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#include "stdafx.h"
#include "SearchMap.h"


// --------------------------------------------------------------------------

// TODO: Enable if needed
//_CLASSNAME(_T("SearchMap"));

// --------------------------------------------------------------------------

 

SearchMap::SearchMap() {

  m_Radius      = 0;
  m_pMap        = NULL;

  ZeroMemory(&m_Center, sizeof(POINT));
  ZeroMemory(&m_TopLeft, sizeof(POINT));
  ZeroMemory(&m_BottomRight, sizeof(POINT));

}

SearchMap::~SearchMap() {

  ReleaseMap();

}

void SearchMap::CreateMap() {

  ReleaseMap();

  USHORT dimesnion = m_Radius * 2 + 1;
  m_pMap = new SEARCHMAPENTRY[dimesnion];

}

void SearchMap::BuildCircleMap(USHORT centerX, USHORT centerY, USHORT radius, const PBOUNDS limits) {

  USHORT prevX    = (USHORT)m_Center.x;

  m_Center.x      = centerX;
  m_Center.y      = centerY;

  m_TopLeft.x     = limits->left;
  m_TopLeft.y     = limits->top;

  m_BottomRight.x = limits->right;
  m_BottomRight.y = limits->bottom;

  if(m_Radius == radius) {

    if(prevX != centerX) {

      // Center x have changed - update left/right values
      USHORT rows = m_Radius * 2 + 1;
      for(BYTE i = 0; i < rows; i++) {
        SEARCHMAPENTRY sme = m_pMap[i];
        if(sme.used) {
          sme.lx = (SHORT)(m_Center.x - sme.xMin);
          sme.rx = (SHORT)(m_Center.x + sme.xMax);

          m_pMap[i] = sme;
        }
      }
    }

    return;
  }

  m_Radius = radius;

  CreateMap();

  SHORT xMin;
  SHORT xMax;

  USHORT dimesnion = m_Radius * 2 + 1;
  for(USHORT y = 0; y < dimesnion; y++) {

    xMin = -1;
    xMax = -1;

    // Search left
    for(USHORT x = 0; x < m_Radius; x++) {
      if(IsInsideCircle(m_Radius, m_Radius, m_Radius, x, y)) {
        xMin = x;
        break;
      }
    }

    if(xMin > -1) {
      // Search right
      for(USHORT x = 2 * m_Radius; x >= m_Radius; x--) {
        if(IsInsideCircle(m_Radius, m_Radius, m_Radius, x, y)) {
          xMax = x;
          break;
        }
      }
    }


    if(xMax > -1) {
      m_pMap[y].used = TRUE;

      // Store how many pixels from the center (left/right)
      m_pMap[y].xMin = m_Radius - xMin;
      m_pMap[y].xMax = xMax - m_Radius;

      // Left and right X
      m_pMap[y].lx = (SHORT)(m_Center.x - m_pMap[y].xMin);
      m_pMap[y].rx = (SHORT)(m_Center.x + m_pMap[y].xMax);
    } else {
      // Cant found
      m_pMap[y].used = FALSE;
    }

  }

}

BYTE SearchMap::IsSearchable(USHORT x, USHORT y) {

  // TODO: Muista t‰‰ - otettu pois k‰ytˆst‰ koska n‰ytt‰‰ ett‰ t‰nne ei tulla koskaan
  //       ja t‰ll‰ saa hieman optimoitua
  //
  //       Pit‰‰ ehk‰ ottaa k‰yttˆˆn tai sitten pist‰‰ try/catch

  //if(x < m_TopLeft.x || x > m_BottomRight.x || y < m_TopLeft.y || y > m_BottomRight.y) {
  //  // Outside
  //  return FALSE;
  //}

  //USHORT r = (USHORT)(y - m_TopLeft.y);

  SEARCHMAPENTRY sme = m_pMap[ y - m_TopLeft.y ];
  if(sme.used) {
    if(x < sme.lx || x > sme.rx) {
      // Outside
      return FALSE;
    }


    // TODO: t‰‰ on orginaali tapa, jos uusi toimii niin poista t‰‰
    //if(x < m_Center.x - sme.xMin || x > m_Center.x + sme.xMax) {
    //  // Outside
    //  return FALSE;
    //}

    // Inside
    return TRUE;
  }

  // Outside
  return FALSE;
}

void SearchMap::ReleaseMap() {

  if(m_pMap != NULL) {
    delete []m_pMap;
    m_pMap = NULL;
  }

}