// --------------------------------------------------------------------------
// PunchDetector.h
// --------------------------------------------------------------------------
//
// SpotLight punch placement detector. Using the Detect-method, punch detector can be
// used to verify that the specifed circle contains blood and that the extra space 
// (PunchBorder) around it is unpunched. IsPunched-method can be used to check whether
// specific pixel is punched or not.
//
// Allocated punches are put into a list of punches, so new position searches can skip
// the area already allocated for a punch.
//
// After all requested punch positions are detected, Clear-method needs to be called
// before next detection cycle is started.
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 10.05.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once

#include "BloodDetector.h"



// Detector settings
typedef struct _PUNCHDETECTORSETTINGS {

  BYTE    PunchBorder;                // Border padding around punch in pixels - extra space that
                                      // needs to be unpunched
  POINT   ptSearchAreaCenter;         // Center position of circular punch search area
  USHORT  searchAreaRadius;           // Radius of circular punch search area

} PUNCHDETECTORSETTINGS, *PPUNCHDETECTORSETTINGS;



class PunchDetector {

  public:
    PunchDetector();
    ~PunchDetector();


                  // Initialize detector
    void          Initialize(const PPUNCHDETECTORSETTINGS pSettings);

                  // Release detector
    void          Release();

                  // Detects whether punch can be placed in the specified position. Punch can be placed if
                  // the entire area contains blood and is unpunched + the additional border around it is
                  // unpunched. Returns TRUE, if punch can be add. The succesful detection result is stored
                  // to internal list of already allocated punches. Therefore calling this method two times
                  // with the same parameters makes the second call fail.    
    BYTE          Detect(SHORT x, SHORT y, BYTE radius, BYTE checkProximityToOthers, BloodDetector* pBloodDetector);

                  // Return TRUE if specified pixel is part of a punch
    BYTE          IsPunched(USHORT x, USHORT y);

                  // Clear detection info
    void          Clear(USHORT punchesToSearchFor);

                  // Get currently used detector settings
    void          GetCurrentSettings(PPUNCHDETECTORSETTINGS pSettings);

                  // Update detector settings
    void          UpdateSettings(const PPUNCHDETECTORSETTINGS pSettings);

                  // Punch Border, the area outside the actual punch area, that is reserved for punch position inaccuracies, the
                  // border area needs to be in blood too (assuming that the punch area needs to be in blood), and no other punches
                  // may be even partially positioned on this border area.
    inline BYTE   GetPunchBorder()      { return m_PunchBorder; }
    void          SetPunchBorder(BYTE border);

                  // Blood Border, the area in the outskirts of detected blood, that is reserved for punchhead-to-camera position inaccuracies.
    inline BYTE   GetBloodBorder()      { return m_BloodBorder; }
    void          SetBloodBorder(BYTE border);

  private:

                  // TRUE when detector is initialized
    BYTE          m_Initialized;

                  // Additional border, used when detecting punch
    BYTE          m_PunchBorder;

                  // Blood Border, the area in the outskirts of detected blood, that is reserved for punchhead-to-camera position inaccuracies.
    BYTE          m_BloodBorder;

                  // Top left corner for punch detection
    POINT         m_ptSearchAreaTopLeft;

                  // Bottom right corner for punch detection
    POINT         m_ptSearchAreaBottomRight;

                  // Blood search area center
    POINT         m_ptSearchAreaCenter;

                  // Blood search area radius
    USHORT        m_searchAreaRadius;

                  // Array of punches that have been located in current image
    PPUNCHPOSITION m_PAllocatedPunches;

                  // Maximum number of punches in above array
    USHORT        m_allocatedPunchesLength;

            // Cache settings to local variables
    void    CopySettings(const PPUNCHDETECTORSETTINGS pSettings);

            // Return TRUE if all in the specified circle are blood
    BYTE    IsAllBlood(USHORT x, USHORT y, USHORT radius, BloodDetector* pBloodDetector);

            // Return TRUE if all the pixels in the specified circle are unallocated for punch.
    BYTE    IsAllNotAllocated(USHORT x, USHORT y, USHORT radius);

            // Add punch info. Mark the pixels inside the specified circle to be part of
            // punch. The info is stored into internal punch map helper object (m_pPunchMap)
    void    AddPunchInfo(USHORT x, USHORT y, BYTE radius);

            // Clear the array of allocated punches
    void    ClearAllocatedPunches();

            // Throw exception if detector isn´t initialized
    void    ThrowNotInitializedException(const PTCHAR pszMethod);

            // Throw exception if detector is already initialized
    void    ThrowAlreadyInitializedException(const PTCHAR pszMethod);

  

};