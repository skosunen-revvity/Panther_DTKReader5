// --------------------------------------------------------------------------
// PunchPositionManager.h
// --------------------------------------------------------------------------
//
// Punch position container
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 05.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#include "stdafx.h"
#include "PunchPositionManager.h"
#include "ExceptionInfo.h"

// --------------------------------------------------------------------------

// Event names
#define EVENTNAME_PUNCHPOSITIONS_COUNT_CHANGED      _T("SpotLightDSC_PunchPosCountChanged")
#define EVENTNAME_BLOODCARD_DETECTED                _T("SpotLightDSC_BloodcardDetected")
#define EVENTNAME_BLOODCARD_REMOVED                 _T("SpotLightDSC_BloodcardRemoved")
#define EVENTNAME_BARCODES_UPDATED                  _T("SpotLightDSC_BarcodesUpdated")
#define EVENTNAME_CAMERADRIVER_EXCEPTION_OCCURED    _T("SpotLightDSC_CameradriverExceptionOccured")
#define EVENTNAME_FRAMES_SKIPPED                    _T("SpotLightDSC_FramesSkipped")

// --------------------------------------------------------------------------

_CLASSNAME(_T("PunchPositionManager"));

// --------------------------------------------------------------------------



PunchPositionManager::PunchPositionManager() {

  m_bInitialized                            = FALSE;
  m_ReadBarcodes                            = FALSE;

  m_PunchPositionFlags                      = PPM_NONE;

  m_cPunchPositionCount                     = UNKNOWN_ELEMENT_COUNT;
  m_pPunchInfo                              = NULL;

  m_hPunchPositionCountChangedNotification  = NULL;
  m_hBloodCardDetectedNotification          = NULL;
  m_hBloodCardRemovedNotification           = NULL;
  m_hBarcodesUpdatedNotification            = NULL;
  m_hCameraExceptionOccuredNotification     = NULL;
  m_hFramesSkippedNotification              = NULL;

  m_pCameraDriverException                  = NULL;

  ZeroMemory(&m_csLock, sizeof(CRITICAL_SECTION));

}

PunchPositionManager::~PunchPositionManager() {

  Release();

}

void PunchPositionManager::Initialize() {

  // Create notification events
  m_hPunchPositionCountChangedNotification  = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_PUNCHPOSITIONS_COUNT_CHANGED);
  m_hBloodCardDetectedNotification          = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_BLOODCARD_DETECTED);
  m_hBloodCardRemovedNotification           = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_BLOODCARD_REMOVED);
  m_hBarcodesUpdatedNotification            = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_BARCODES_UPDATED);
  m_hCameraExceptionOccuredNotification     = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_CAMERADRIVER_EXCEPTION_OCCURED);
  m_hFramesSkippedNotification              = CreateEvent(NULL, TRUE, FALSE, EVENTNAME_FRAMES_SKIPPED);

  InitializeCriticalSection(&m_csLock);

  m_bInitialized = TRUE;

}

void PunchPositionManager::Release() {

  if(m_bInitialized) {
    Lock();
    ReleasePunchPositionStorage();

    if(m_hPunchPositionCountChangedNotification != NULL) {
      CloseHandle(m_hPunchPositionCountChangedNotification);
      m_hPunchPositionCountChangedNotification = NULL;
    }

    if(m_hBloodCardDetectedNotification != NULL) {
      CloseHandle(m_hBloodCardDetectedNotification);
      m_hBloodCardDetectedNotification = NULL;
    }

    if(m_hBloodCardRemovedNotification != NULL) {
      CloseHandle(m_hBloodCardRemovedNotification);
      m_hBloodCardRemovedNotification = NULL;
    }

    if(m_hBarcodesUpdatedNotification != NULL) {
      CloseHandle(m_hBarcodesUpdatedNotification);
      m_hBarcodesUpdatedNotification = NULL;
    }

    if(m_hCameraExceptionOccuredNotification != NULL) {
      CloseHandle(m_hCameraExceptionOccuredNotification);
      m_hCameraExceptionOccuredNotification = NULL;
    }

    if(m_hFramesSkippedNotification != NULL) {
      CloseHandle(m_hFramesSkippedNotification);
      m_hFramesSkippedNotification = NULL;
    }

    Unlock();

    DeleteCriticalSection(&m_csLock);
    ZeroMemory(&m_csLock, sizeof(CRITICAL_SECTION));

    m_bInitialized = FALSE;
  }

}

void PunchPositionManager::GetPunchPositions(BYTE& count, PPUNCHPOSITION& pPositions) {

  _METHODNAME(_T("GetPunchPositions"));
  if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
    // Storage is empty
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
  }

  count = (BYTE)m_cPunchPositionCount;
  if(count > 0) {
    pPositions = new PUNCHPOSITION[m_cPunchPositionCount];
    for(BYTE i = 0; i < m_cPunchPositionCount; i++) {
      pPositions[i].dwTime      = m_pPunchInfo[i].dwTime;
      pPositions[i].ID          = m_pPunchInfo[i].ID;
      pPositions[i].Used        = m_pPunchInfo[i].Used;
      pPositions[i].X           = m_pPunchInfo[i].X;
      pPositions[i].Y           = m_pPunchInfo[i].Y;
      pPositions[i].Radius      = m_pPunchInfo[i].Radius;

      pPositions[i].BorderColor = m_pPunchInfo[i].BorderColor;
      pPositions[i].BorderWidth = m_pPunchInfo[i].BorderWidth;
      pPositions[i].FillColor   = m_pPunchInfo[i].FillColor;
      pPositions[i].InBlood     = m_pPunchInfo[i].InBlood;
      pPositions[i].Locked      = m_pPunchInfo[i].Locked;
      pPositions[i].TextColor   = m_pPunchInfo[i].TextColor;

    }
  } else {
    pPositions = NULL;
  }

}

void PunchPositionManager::SetPunchPositions(BYTE count, PPUNCHPOSITION pPositions) {

  _METHODNAME(_T("SetPunchPositions"));
  if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
    // Storage is empty
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
  }

  if(count > m_cPunchPositionCount) {
    count = (BYTE)m_cPunchPositionCount;
  }

  for(BYTE i = 0; i < count; i++) {
    m_pPunchInfo[i].dwTime      = pPositions[i].dwTime;
    m_pPunchInfo[i].ID          = pPositions[i].ID;
    m_pPunchInfo[i].Used        = pPositions[i].Used;
    m_pPunchInfo[i].X           = pPositions[i].X;
    m_pPunchInfo[i].Y           = pPositions[i].Y;
    m_pPunchInfo[i].Radius      = pPositions[i].Radius;

    m_pPunchInfo[i].BorderColor = pPositions[i].BorderColor;
    m_pPunchInfo[i].BorderWidth = pPositions[i].BorderWidth;
    m_pPunchInfo[i].FillColor   = pPositions[i].FillColor;
    m_pPunchInfo[i].InBlood     = pPositions[i].InBlood;
    m_pPunchInfo[i].Locked      = pPositions[i].Locked;
    m_pPunchInfo[i].TextColor   = pPositions[i].TextColor;
  }

}

BYTE PunchPositionManager::GetRequestedPunchPositionCount() {

  _METHODNAME(_T("GetRequestedPunchPositionCount"));
  if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
    // Storage is empty
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
  }

  BYTE count = 0;
  for(BYTE i = 0; i < m_cPunchPositionCount; i++) {
    if(m_pPunchInfo[i].Used) {
      count++;
    }
  }

  return count;

}

BYTE PunchPositionManager::GetCountOfPunchPositionsWithValidPosition() {

  _METHODNAME(_T("GetCountOfPunchPositionsWithValidPosition"));
  if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
    // Storage is empty
    ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
  }

  BYTE count = 0;
  for(BYTE i = 0; i < m_cPunchPositionCount; i++) {
    if(m_pPunchInfo[i].Used && m_pPunchInfo[i].InBlood) {
      count++;
    }
  }

  return count;

}

void PunchPositionManager::Lock() {

  EnterCriticalSection(&m_csLock);

}

BYTE PunchPositionManager::TryToLock() {
  
  return TryEnterCriticalSection(&m_csLock) != 0 ? TRUE : FALSE;

}

void PunchPositionManager::Unlock() {

  LeaveCriticalSection(&m_csLock);

}

void PunchPositionManager::NotifyPunchPositionCountChangedListener() {

  if(m_hPunchPositionCountChangedNotification != NULL) {
    SetEvent(m_hPunchPositionCountChangedNotification);
  }

}

void PunchPositionManager::NotifyBloodCardDetectedListener() {

  if(m_hBloodCardDetectedNotification != NULL) {
    SetEvent(m_hBloodCardDetectedNotification);
  }

}

void PunchPositionManager::NotifyBloodCardRemovedListener() {

  if(m_hBloodCardRemovedNotification != NULL) {
    SetEvent(m_hBloodCardRemovedNotification);
  }

}

void PunchPositionManager::NotifyBarcodesUpdatedListener() {

  if(m_hBarcodesUpdatedNotification != NULL) {
    SetEvent(m_hBarcodesUpdatedNotification);
  }

}

void PunchPositionManager::NotifyCameraDriverExceptionListener() {

  if(m_hCameraExceptionOccuredNotification != NULL) {
    SetEvent(m_hCameraExceptionOccuredNotification);
  }

}
 
void PunchPositionManager::NotifyFramesSkippedListener() {

  if(m_hFramesSkippedNotification != NULL) {
    SetEvent(m_hFramesSkippedNotification);
  }

}

void PunchPositionManager::SetCameraDriverException(ExceptionInfo* pException) {

  Lock();

  // Store only the first exception
  if(m_pCameraDriverException == NULL) {
    m_pCameraDriverException = pException;
    NotifyCameraDriverExceptionListener();
  } else {
    // Cant set, release exception info
    try {
      delete pException;
    } catch (...) {
    }
  }

  Unlock();

}

void PunchPositionManager::ClearCameraDriverException() {

  Lock();

  if(m_pCameraDriverException != NULL) {
    try {
      delete m_pCameraDriverException;
    } catch (...) {
    } finally {
      m_pCameraDriverException = NULL;
    }
  }

  Unlock();

}

void PunchPositionManager::GetPunchPositionInfo(BYTE element, PPUNCHPOSITION pInfo) {

    _METHODNAME(_T("GetPunchPositionInfo"));
    if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
        // Storage is empty
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
    }

    if(element >= m_cPunchPositionCount) {
        // Invalid element index
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_INVALID_PPSTORAGE_INDEX);
    }

    pInfo->dwTime       = m_pPunchInfo[element].dwTime;
    pInfo->ID           = m_pPunchInfo[element].ID;
    pInfo->Used         = m_pPunchInfo[element].Used;
    pInfo->X            = m_pPunchInfo[element].X;
    pInfo->Y            = m_pPunchInfo[element].Y;
    pInfo->Radius       = m_pPunchInfo[element].Radius;

    pInfo->exactPosValid = m_pPunchInfo[element].exactPosValid;
    pInfo->exactX       = m_pPunchInfo[element].exactX;
    pInfo->exactY       = m_pPunchInfo[element].exactY;
    pInfo->diameter     = m_pPunchInfo[element].diameter;

    pInfo->BorderColor  = m_pPunchInfo[element].BorderColor;
    pInfo->BorderWidth  = m_pPunchInfo[element].BorderWidth;
    pInfo->FillColor    = m_pPunchInfo[element].FillColor;
    pInfo->InBlood      = m_pPunchInfo[element].InBlood;
    pInfo->Locked       = m_pPunchInfo[element].Locked;
    pInfo->TextColor    = m_pPunchInfo[element].TextColor;

}

void PunchPositionManager::SetPunchPositionInfo(BYTE element, PPUNCHPOSITION pInfo) {

    _METHODNAME(_T("SetPunchPositionInfo"));
    if(m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT || m_pPunchInfo == NULL) {
        // Storage is empty
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_PPSTORAGE_IS_EMPTY);
    }

    if(element >= m_cPunchPositionCount) {
        // Invalid element index
        ExceptionInfo::Throw(CLASSNAME, METHODNAME, ERR_PPMANAGER_INVALID_PPSTORAGE_INDEX);
    }

    m_pPunchInfo[element].dwTime        = pInfo->dwTime;
    m_pPunchInfo[element].ID            = pInfo->ID;
    m_pPunchInfo[element].Used          = pInfo->Used;
    m_pPunchInfo[element].X             = pInfo->X;
    m_pPunchInfo[element].Y             = pInfo->Y;
    m_pPunchInfo[element].Radius        = pInfo->Radius;

    m_pPunchInfo[element].exactPosValid = pInfo->exactPosValid;
    m_pPunchInfo[element].exactX       = pInfo->exactX;
    m_pPunchInfo[element].exactY       = pInfo->exactY;
    m_pPunchInfo[element].diameter     = pInfo->diameter;

    m_pPunchInfo[element].BorderColor   = pInfo->BorderColor;
    m_pPunchInfo[element].BorderWidth   = pInfo->BorderWidth;
    m_pPunchInfo[element].FillColor     = pInfo->FillColor;
    m_pPunchInfo[element].InBlood       = pInfo->InBlood;
    m_pPunchInfo[element].Locked        = pInfo->Locked;
    m_pPunchInfo[element].TextColor     = pInfo->TextColor;

}

void PunchPositionManager::StartPunchPositionsUpdate(BYTE cPunchPositionCount, DWORD flags) {

  m_PunchPositionFlags = flags;
  CreatePunchPositionStorage(cPunchPositionCount);

}

void PunchPositionManager::CreatePunchPositionStorage(BYTE cPunchPositionCount)
{

  if(m_cPunchPositionCount != cPunchPositionCount) {
    ReleasePunchPositionStorage();
    if(cPunchPositionCount > 0)  {
      m_pPunchInfo          = new PUNCHPOSITION[cPunchPositionCount];
      m_cPunchPositionCount = cPunchPositionCount;
    }
  }

  ResetPunchPositionStorage();

}

void PunchPositionManager::ResetPunchPositionStorage()
{

  if(m_pPunchInfo != NULL && m_cPunchPositionCount > 0) {
    for(BYTE i = 0; i < m_cPunchPositionCount; i++) {
      
      m_pPunchInfo[i].dwTime      = UNKNOWN_TIME;
      m_pPunchInfo[i].ID          = UNKNOWN_ID;
      m_pPunchInfo[i].X           = UNKNOWN_COORD_X;
      m_pPunchInfo[i].Y           = UNKNOWN_COORD_Y;
      m_pPunchInfo[i].Radius      = UNKNOWN_RADIUS;

      m_pPunchInfo[i].exactPosValid = 0;

      m_pPunchInfo[i].BorderColor = RGB(0, 0, 0);
      m_pPunchInfo[i].BorderWidth = 0;
      m_pPunchInfo[i].FillColor   = RGB(0, 0, 0);
      m_pPunchInfo[i].InBlood     = FALSE;
      m_pPunchInfo[i].Locked      = FALSE;
      m_pPunchInfo[i].TextColor   = RGB(255, 255, 255);

    }
  }

}

void PunchPositionManager::ReleasePunchPositionStorage()
{

  if(m_pPunchInfo != NULL) {
    delete []m_pPunchInfo;
    m_pPunchInfo = NULL;
  }

  m_cPunchPositionCount = UNKNOWN_ELEMENT_COUNT;

}
