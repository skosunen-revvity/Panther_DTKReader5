// --------------------------------------------------------------------------
// SearchMap.h
// --------------------------------------------------------------------------
//
// Helper class that is used to optimize which pixels in the specific area 
// should be included in the search. Currently only circle area is supported.
// When search info is created, internal helper object is created (m_pMap)
// which will hold info about the pixels that are inside the specified circle.
// When the circle center is changed, the helper object is updated. When the
// radius changes, the helper object created again
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 11.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once

// Helper to hold info about pixels that should be included (are inside circle) into search
typedef struct _SEARCHMAPENTRY {

  BYTE used;      // Data is valid

  SHORT xMin;     // Left offset (pixels) fromt he center
  SHORT xMax;     // Right offset (pixels) from the center

  SHORT lx;       // Left X co-ordinate
  SHORT rx;       // Right X co-ordinate

} SEARCHMAPENTRY, *PSEARCHMAPENTRY;


class SearchMap {

  public:
    SearchMap();
    ~SearchMap();

            // Build circle map. Init and create the internal helper object (m_pMap) and store
            // their info about pixels that are inside the circle
    void    BuildCircleMap(USHORT centerX, USHORT centerY, USHORT radius, const PBOUNDS limits);

            // Returns TRUE is specified pixel is inside the circle
    BYTE    IsSearchable(USHORT x, USHORT y);



  protected:

                    // Circles center
    POINT           m_Center;

                    // Circle radius
    USHORT          m_Radius;

    POINT           m_TopLeft;
    POINT           m_BottomRight;

                    // Info about the pixels that are inside the circle
    PSEARCHMAPENTRY m_pMap;



            // Release search map helper object
    void    ReleaseMap();

            // Allocata space for the helper object (m_pMap) that will hold info about
            // the pixels that are inside the circle
    void    CreateMap();

};