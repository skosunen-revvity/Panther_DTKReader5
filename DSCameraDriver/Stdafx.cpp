// stdafx.cpp : source file that includes just the standard includes
// DSCameraDriver.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information

#include "stdafx.h"

// --------------------------------------------------------------------------

// Global punch position manager. Created and released in DSCamera
PunchPositionManager* g_pPunchPosManager = NULL;


// --------------------------------------------------------------------------

BYTE CreateBoundsInformation(USHORT centerX, USHORT centerY, USHORT radius, const PBOUNDS pLimits, PBOUNDS pBounds) {

  BYTE cutMade = FALSE;

  if(pBounds != NULL) {

    pBounds->top      = centerY - radius;
    pBounds->left     = centerX - radius;
    pBounds->bottom   = centerY + radius;
    pBounds->right    = centerX + radius;

    if(pLimits != NULL) {
      if(pBounds->top < pLimits->top) {
        pBounds->top = pLimits->top;
        cutMade = TRUE;
      }

      if(pBounds->left < pLimits->left) {
        pBounds->left = pLimits->left;
        cutMade = TRUE;
      }
      
      if(pBounds->bottom > pLimits->bottom) {
        pBounds->bottom = pLimits->bottom;
        cutMade = TRUE;
      }

      if(pBounds->right > pLimits->right) {
        pBounds->right = pLimits->right;
        cutMade = TRUE;
      }
    }    

  }

  return cutMade;

}

BYTE IsInsideCircle(USHORT centerX, USHORT centerY, USHORT radius, USHORT x, USHORT y) {

  int dX = x - centerX;
  int dY = y - centerY;

  int sumOfSquares = dX * dX + dY * dY;
  return (radius * radius >= sumOfSquares);

}

void CopyString(const PTCHAR pszSource, PTCHAR& pszDest) {

  size_t len = 0;
  if(pszSource != NULL) {
    len = _tcslen(pszSource);
  }

  if(len > 0) {
    pszDest = new TCHAR[len + 1];
    _tcscpy(pszDest, pszSource);
    pszDest[len] = '\0';

  } else {
    pszDest = NULL;
  }

}

void ReleaseString(PTCHAR& pszStr) {

  if(pszStr != NULL) {
    delete []pszStr;
    pszStr = NULL;
  }

}
