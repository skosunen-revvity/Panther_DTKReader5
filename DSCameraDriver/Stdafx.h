// --------------------------------------------------------------------------
// stdafx.h
// --------------------------------------------------------------------------
//
// Include file for standard system include files, or project specific include files 
// that are used frequently, but are changed infrequently
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 03.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#pragma once

// --------------------------------------------------------------------------

// Used to disable warning C4996: 'wcscpy': This function or variable may be unsafe. Consider using wcscpy_s instead.
#pragma warning ( disable: 4996 )

// --------------------------------------------------------------------------

#include <Windows.h>
#include "streams.h"
#include <TCHAR.h>

#include "ErrorCodes.h"
#include "PunchPositionManager.h"

// --------------------------------------------------------------------------

// Helper macro to define METHODNAME variable
#define _METHODNAME(a)        const PTCHAR METHODNAME = a;

// Helper macro to define CLASSNAME variable
#define _CLASSNAME(a)         const PTCHAR CLASSNAME = a;


// --------------------------------------------------------------------------


// Global punch position/global data manager. Created and released in DSCamera
extern PunchPositionManager* g_pPunchPosManager;

// Type of used image
enum eInputImageType {
  
  GrayscaleImage = 0,       // Input data is already converted to grayscale; only the red channel 
                            // is used when detecting blood or calculating index value

  ColorImage                // Input data is color image, in which case all color channels are 
                            // checked when detecting blood or calculating index value
};

// Color struct, used to access image data/pixels
typedef struct _SPOTLIGHTCOLOR {
  BYTE b;
  BYTE g;
  BYTE r;
} SPOTLIGHTCOLOR, *PSPOTLIGHTCOLOR;

// Struct to hold bounds info.
typedef struct _BOUNDS {
  int top;
  int left;
  int bottom;
  int right;
} BOUNDS, *PBOUNDS;

typedef struct _INDEXVALUERANGE {

  DWORD dwMin;
  DWORD dwMax;

} INDEXVALUERANGE, *PINDEXVALUERANGE;


// --------------------------------------------------------------------------
// Helper class to safely release COM objects
// --------------------------------------------------------------------------

template <class T> void SafeRelease(T **ppT)
{
  if(*ppT) {
    (*ppT)->Release();
    *ppT = NULL;
  }
}

// --------------------------------------------------------------------------

// Copy string form source to dest. Will allocate space
void CopyString(const PTCHAR pszSource, PTCHAR& pszDest);

// Release string and set it to NULL
void ReleaseString(PTCHAR& pszStr);

// Create bounds (rectangle) around the specified circle. Use limits to crop the created image. Returns TRUE
// if crop was performed.
BYTE CreateBoundsInformation(USHORT centerX, USHORT centerY, USHORT radius, const PBOUNDS pLimits, PBOUNDS pBounds);

// Return TRUE if specified point (x, y) is inside circle
BYTE IsInsideCircle(USHORT centerX, USHORT centerY, USHORT radius, USHORT x, USHORT y);

