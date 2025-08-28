// --------------------------------------------------------------------------
// PunchDetector.cpp
// --------------------------------------------------------------------------
//
// SpotLight punch placement detector implementation
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 10.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#include "stdafx.h"
#include "PunchDetector.h"
#include "ExceptionInfo.h"


// --------------------------------------------------------------------------

// Punch map value for unpunched pixel
#define PUNCHMAP_EMPTY        0

// Punch map value for pixel that is part of the punch
#define PUNCHMAP_PUNCHED      1

// Class name, used as a param to exception methods
_CLASSNAME(_T("PunchDetector"));

// The default value below is chosen to be low, so that the code is forced to
// allocate more room for punches while running. This way the re-allocation code
// gets executed (and tested) in each run
#define INITIAL_ALLOCATED_PUNCHES_LENGTH 1

// --------------------------------------------------------------------------


PunchDetector::PunchDetector() {
  _METHODNAME(_T("Construct"));

  int i;
  m_Initialized         = FALSE;

  m_PunchBorder         = 0;
  m_BloodBorder         = 0;
  
  // Create and initialize allocated punches array
  m_allocatedPunchesLength = INITIAL_ALLOCATED_PUNCHES_LENGTH;
  m_PAllocatedPunches = new PUNCHPOSITION[m_allocatedPunchesLength];
  if (m_PAllocatedPunches == NULL) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PUNCHDETECTOR_CANNOT_ALLOCATE_MEMORY);
  }
  for (i = 0; i < m_allocatedPunchesLength; i++) {
    m_PAllocatedPunches[i].Used = 0;
  }

}

PunchDetector::~PunchDetector() {
  delete[] m_PAllocatedPunches;
}

void PunchDetector::Initialize(const PPUNCHDETECTORSETTINGS pSettings) {

  _METHODNAME(_T("Initialize"));
  ThrowAlreadyInitializedException(METHODNAME);

  if(pSettings == NULL) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PUNCHDETECTOR_SETTINGS_IS_NULL);
  }

  m_Initialized = FALSE;

  // Cache settings and allocate space for the punch map
  CopySettings(pSettings);

  for (int i = 0; i < m_allocatedPunchesLength; i++) {
    m_PAllocatedPunches[i].Used = 0;
  }

  m_Initialized = TRUE;

}

void PunchDetector::Release() {

  _METHODNAME(_T("Release"));
  ThrowNotInitializedException(METHODNAME);
  
  m_Initialized = FALSE;

}

BYTE PunchDetector::Detect(SHORT x, SHORT y,  BYTE radius, BYTE checkProximityToOthers, BloodDetector* pBloodDetector) {

  _METHODNAME(_T("Detect"));
  ThrowNotInitializedException(METHODNAME);

  if(pBloodDetector == NULL) {
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PUNCHDETECTOR_BD_IS_NULL);
  }
  
  LONG xDiff = x - m_ptSearchAreaCenter.x;
  LONG yDiff = y - m_ptSearchAreaCenter.y;
  LONG effectiveRadius = m_searchAreaRadius - m_BloodBorder - radius;
  if (effectiveRadius < 0) {
    // Searchd punch radius, including the border, is larger than search area radius
    return FALSE;
  }


  // 1st condition: the punch and border needs to be all inside the circular search area
  if ( ((xDiff * xDiff) + (yDiff * yDiff)) < ( effectiveRadius * effectiveRadius)) {
      // 2nd condition: No other punches too close to this one
      if(IsAllNotAllocated(x, y, radius + (checkProximityToOthers ? m_PunchBorder : 0))) {
        // 3rd condition: All the pixels contain blood
        if(IsAllBlood(x, y, radius + m_BloodBorder, pBloodDetector)) {
          // Success, store info about the punch, so that we cant use it again, before the Clear is called
          AddPunchInfo(x, y, radius);
          return TRUE;
        }
      }
    }

  return FALSE;
}

void PunchDetector::GetCurrentSettings(PPUNCHDETECTORSETTINGS pSettings) {
  _METHODNAME(_T("GetCurrentSettings"));
  ThrowNotInitializedException(METHODNAME);

  pSettings->PunchBorder              = m_PunchBorder;

  pSettings->ptSearchAreaCenter       = m_ptSearchAreaCenter;
  pSettings->searchAreaRadius         = m_searchAreaRadius;
}

void PunchDetector::UpdateSettings(const PPUNCHDETECTORSETTINGS pSettings) {

  _METHODNAME(_T("UpdateSettings"));
  ThrowNotInitializedException(METHODNAME);

  // Cache settins and reinit punch map
  CopySettings(pSettings);
}

BYTE PunchDetector::IsAllBlood(USHORT centerX, USHORT centerY, USHORT radius, BloodDetector* pBloodDetector) {

  BYTE oneFound = FALSE;
  ULONG radiusSquared = radius * radius;
  
  for(int y = centerY - radius; y < centerY + radius; y++) {
    for(int x = centerX - radius; x < centerX + radius; x++) {
      // Go through all the pixels inside the smallest square that fits all the circular punch area...
      LONG xDiff = centerX - x;
      LONG yDiff = centerY - y;

      if ((ULONG)((xDiff*xDiff) + (yDiff*yDiff)) < radiusSquared) {
        // ...but check only pixels inside the circular punch
        oneFound = pBloodDetector->IsBlood(x, y);
        if(!oneFound) {
          return FALSE;
        }
      }
    }
  }

  return oneFound;

}

BYTE PunchDetector::IsAllNotAllocated(USHORT centerX, USHORT centerY, USHORT radius) {
  _METHODNAME(_T("IsAllNotAllocated"));
  for (int i = 0; i < m_allocatedPunchesLength; i++) {
    PPUNCHPOSITION  current = &(m_PAllocatedPunches[i]);
    if (current->Used != 0) {
      LONG xDiff = centerX - current->X;
      LONG yDiff = centerY - current->Y;
      LONG radiusSum = radius + current->Radius;
      if ( ((xDiff * xDiff) + (yDiff * yDiff)) < (radiusSum * radiusSum)) {
        // The i:th punch is closer to suggested position than their radiuses allow, the suggested position is not
        // possible
        return FALSE;
      }
    } else {
      // All the exisiting positions checked, the suggested position is possible
      return TRUE;
    }
  }
  // All the existing punches have been checked, and the suggested position does not overlap any of them.
  // This also means, that there is no room in m_allocatedPositions for the new position, but it is not up
  // to this function to throw an exception for that
  return TRUE;
}

void PunchDetector::AddPunchInfo(USHORT centerX, USHORT centerY, BYTE radius) {
  _METHODNAME(_T("AddPunchInfo"));
  
  for (int i = 0; i < m_allocatedPunchesLength; i++) {
    PPUNCHPOSITION  current = &(m_PAllocatedPunches[i]);
    if (current->Used == 0) {
      // Unused position found
      current->X = centerX;
      current->Y = centerY;
      current->Radius = radius;
      current->Used = TRUE;
      return;
    }
  }
  ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PUNCHDETECTOR_NO_ROOM_IN_ALLOCATED_PUNCHES);
}


void PunchDetector::Clear(USHORT punchesToSearchFor) {
  _METHODNAME(_T("Clear"));
  ThrowNotInitializedException(METHODNAME);

  if (punchesToSearchFor > m_allocatedPunchesLength) {
    // Exisiting list of allocated punches is less than the requested one, make the array larger
    delete[] m_PAllocatedPunches;
    m_allocatedPunchesLength = punchesToSearchFor * 2;
    m_PAllocatedPunches = new PUNCHPOSITION[m_allocatedPunchesLength];
    if (m_PAllocatedPunches == NULL) {
      ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PUNCHDETECTOR_CANNOT_ALLOCATE_MEMORY);
    }
  }

  ClearAllocatedPunches();
}

void PunchDetector::CopySettings(const PPUNCHDETECTORSETTINGS pSettings) {

  m_PunchBorder                 = pSettings->PunchBorder;

  m_ptSearchAreaCenter.x        = pSettings->ptSearchAreaCenter.x;
  m_ptSearchAreaCenter.y        = pSettings->ptSearchAreaCenter.y;
  m_searchAreaRadius            = pSettings->searchAreaRadius;

}


void PunchDetector::ThrowNotInitializedException(const PTCHAR pszMethod) {

  if(!m_Initialized) {
    ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_PUNCHDETECTOR_NOT_INITIALIZED);
  }

}

void PunchDetector::ThrowAlreadyInitializedException(const PTCHAR pszMethod) {

  if(m_Initialized) {
    ExceptionInfo::Throw(CLASSNAME, pszMethod, ERR_PUNCHDETECTOR_ALREADY_INITIALIZED);
  }

}

void PunchDetector::ClearAllocatedPunches() {
  for (int i = 0; i < m_allocatedPunchesLength; i++) {
    PPUNCHPOSITION  current = &(m_PAllocatedPunches[i]);
    current->Used = FALSE;
  }
}

void PunchDetector::SetPunchBorder(BYTE border) {
  m_PunchBorder = border;
}

void PunchDetector::SetBloodBorder(BYTE border) {
  m_BloodBorder = border;
}


