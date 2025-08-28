// --------------------------------------------------------------------------
// PunchPositionManager.h
// --------------------------------------------------------------------------
//
// Global data storage. Helper class that is used to hold searched/found punch
// position information between the actual image renderer (DSCameraRenderer) and
// camera interface (DSCameraDriver/DSRenderManager).
//
// Class contains Lock/Unlock methods which can be used to lock the class. Image
// renderer is using TryToLock-method to gain access. If it fails to, image
// processing is skipped.
//
// Lock-method doesn´t actually lock the class, all the methods are still available
// and are working: it just sets the internal Critical section to state signaled
// state, which is checked by the image renderer, via TryToLock-method, when trying
// to gain access. Camera interface will use Lock-method, when data is updated.
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 05.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $


#pragma once

#include "ExceptionInfo.h"

// --------------------------------------------------------------------------

#define UNKNOWN_TIME                  0       // Unknown time
#define UNKNOWN_ID                    -1      // Unknown position ID
#define UNKNOWN_COORD_X               -1      
#define UNKNOWN_COORD_Y               -1
#define UNKNOWN_RADIUS                0
#define UNKNOWN_COLOR                 RGB(0, 0, 0)
#define UNKNOWN_WIDTH                 0

#define UNKNOWN_ELEMENT_COUNT         -1

#define PPM_NONE                      0
#define PPM_DRAWID                    1
#define PPM_DRAWID_ONLY_IN_BLOOD      2
#define PPM_FILLBKG                   4
#define PPM_DRAWBORDER                8
#define PPM_DRAWBORDER_IF_LOCKED      16
#define PPM_IN_PUNCH_PROGRESS_MODE    32


// --------------------------------------------------------------------------

// Container to hold info about 1 punch position
typedef struct _PUNCHPOSITION {

  SHORT     X;              // X co-ordinate
  SHORT     Y;              // Y co-ordinate
  BYTE      Radius;         // Punch radius
    
  BYTE      Used;           // When TRUE, position data is processed
  BYTE      ID;             // ID for display purposes

  BYTE      InBlood;        // When TRUE, position is in blood
  DWORD     dwTime;         // Time when position was found

  COLORREF  TextColor;      // ID color
  COLORREF  FillColor;      // Background color
  COLORREF  BorderColor;    // Border color
  
  BYTE      BorderWidth;    // Border width
  BYTE      Locked;         // When TRUE, position is locked and its not changed during processing

  // Data Imager-modules need to store, but not use
  double    exactX;         // double-accuracy presentation of X-coordinate.
  double    exactY;         // Same for Y
  BYTE      exactPosValid;  // Whether double accuracy presentation is valid or not
  int       diameter;       // Diameter for punch, expressed as micrometers

} PUNCHPOSITION, *PPUNCHPOSITION;


class PunchPositionManager {

  public:
    PunchPositionManager();
    ~PunchPositionManager();

                    // Initialize, events and critical section is created
    void            Initialize();

                    // Release, events and critical section is released
    void            Release();

                    // Lock. Sets the internal critical section to signaled state which blocks next,
                    // when called from different thread, Lock call, and fails the TryToLock method
    void            Lock();

                    // Try to lock the internal critical section. If attempt succeeds, TRUE is returned
    BYTE            TryToLock();

                    // Releases the internal critical section
    void            Unlock();

                    // Allocates space for punch positions (cPunchPositionCount)
    void            StartPunchPositionsUpdate(BYTE cPunchPositionCount, DWORD flags);

                    // Raises the punch position count changed event
    void            NotifyPunchPositionCountChangedListener();

                    // Raises the card detected event
    void            NotifyBloodCardDetectedListener();

                    // Raises the card removed event
    void            NotifyBloodCardRemovedListener();
      
                    // Raises the barcodes updated event
    void            NotifyBarcodesUpdatedListener();

                    // Raises the exception occured in image renderer event
    void            NotifyCameraDriverExceptionListener();

                    // Raises the frame processing skipped event
    void            NotifyFramesSkippedListener();

                    // Get info about specific punch position. element-argument is the zero based index
    void            GetPunchPositionInfo(BYTE element, PPUNCHPOSITION pInfo);

                    // Set the pucnh info. element-argument is zero based index
    void            SetPunchPositionInfo(BYTE element, PPUNCHPOSITION pInfo);

                    // Cache exception info (pException) and raise the exception occured event
                    // (NotifyCameraDriverExceptionListener). Only the first exception is stored,
                    // so if the cached exception is already set, it is left and the pException 
                    // argument is released
    void            SetCameraDriverException(ExceptionInfo* pException);

                    // Release cached camera exception
    void            ClearCameraDriverException();

                    // Get the count of used positions that are placed in blood
    BYTE            GetCountOfPunchPositionsWithValidPosition();

                    // Get the number of requested positions (punchinfos Used = TRUE)
    BYTE            GetRequestedPunchPositionCount();
    
                    // Get the punch position infos
    void            GetPunchPositions(BYTE& count, PPUNCHPOSITION& pPositions);

                    // Set the punch position infos
    void            SetPunchPositions(BYTE count, PPUNCHPOSITION pPositions);



                    // Get event handle for the punch count changed event
    inline HANDLE   GetPunchPositionCountChangedNotification()  { return m_hPunchPositionCountChangedNotification; }

                    // Get the event handle for the card detected event
    inline HANDLE   GetBloodCardDetectedNotification()          { return m_hBloodCardDetectedNotification; }

                    // Get th event handle for the card removed event
    inline HANDLE   GetBloodCardRemovedNotification()           { return m_hBloodCardRemovedNotification; }

                    // Get the event handle for the barcodes updated event
    inline HANDLE   GetBarcodesUpdatedNotification()            { return m_hBarcodesUpdatedNotification; }

                    // Get the event handle for the camera exception occured event
    inline HANDLE   GetCameraExceptionOccuredNotification()     { return m_hCameraExceptionOccuredNotification; }

                    // Get the event handle for the frame processing skipped event
    inline HANDLE   GetFramesSkippedNotification()              { return m_hFramesSkippedNotification; }



                    // Get draw punch ID flag
    inline BYTE     GetDrawId()                                 { return m_PunchPositionFlags & PPM_DRAWID; }

                    // Get draw punch ID only when position contains blood flag
    inline BYTE     GetDrawIdOnlyInBlood()                      { return m_PunchPositionFlags & PPM_DRAWID_ONLY_IN_BLOOD; }

                    // Get fill punch background flag
    inline BYTE     GetFillBkg()                                { return m_PunchPositionFlags & PPM_FILLBKG; }

                    // Get draw punch border flag
    inline BYTE     GetDrawBorder()                             { return m_PunchPositionFlags & PPM_DRAWBORDER; }

                    // Get draw punch border only if position is locked flag
    inline BYTE     GetDrawBorderIfLocked()                     { return m_PunchPositionFlags & PPM_DRAWBORDER_IF_LOCKED; }

                    // Get in punch progress mode flag. When this is TRUE, and display image is used, client (UI)
                    // can set the position´s InBlood property and use that to display the punch progress
    inline BYTE     GetInPunchProgressMode()                    { return m_PunchPositionFlags & PPM_IN_PUNCH_PROGRESS_MODE; }

                    // Get the number of punch positions
    inline BYTE     GetPunchPositionCount()                     { return m_cPunchPositionCount == UNKNOWN_ELEMENT_COUNT ? 0 : (BYTE)m_cPunchPositionCount; }

                    // Set barcode reading state. When TRUE, barcodes are read from the image
    inline void     SetReadBarcodesState(BYTE newState)         { m_ReadBarcodes = newState; }

                    // Get the barcode reading state. When TRUE, barcodes are read from the image
    inline BYTE     GetReadBarcodesState()                      { return m_ReadBarcodes; }

                    // Get cached camera exception
    inline ExceptionInfo* GetCameraDriverException()            { return m_pCameraDriverException; }



  private:

                          // When TRUE, calss has been initialized
    BYTE                  m_bInitialized;

                          // When TRUE, barcodes are read from the image
    BYTE                  m_ReadBarcodes;

                          // CS to lock class, via Lock, Unlock and TryToLock methods
    CRITICAL_SECTION      m_csLock;

                          // Number of punbh positions info objects in m_pPunchInfo
    short                 m_cPunchPositionCount;

                          // Punch position flags:
    DWORD                 m_PunchPositionFlags;

                          // Punch position infos
    PPUNCHPOSITION        m_pPunchInfo;

                          
                          // Event handles
    HANDLE                m_hPunchPositionCountChangedNotification;
    HANDLE                m_hBloodCardDetectedNotification;
    HANDLE                m_hBloodCardRemovedNotification;
    HANDLE                m_hBarcodesUpdatedNotification;
    HANDLE                m_hCameraExceptionOccuredNotification;
    HANDLE                m_hFramesSkippedNotification;


                          // Cached camera exception
    ExceptionInfo*        m_pCameraDriverException;

          // Allocate space for punch position infos and reset the data
    void  CreatePunchPositionStorage(BYTE cPunchPositionCount);

          // Release punch position infos
    void  ReleasePunchPositionStorage();

          // Set punch position infos to their default values
    void  ResetPunchPositionStorage();

};