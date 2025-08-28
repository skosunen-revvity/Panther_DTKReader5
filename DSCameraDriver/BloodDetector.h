// --------------------------------------------------------------------------
// BloodDetector.h
// --------------------------------------------------------------------------
//
// Class for detecting blood from the specified bitmap. Detection is based on the color 
// range values (BloodColorMin and BloodColorMax), which specifies the colors that are 
// recognised as blood when the Detect-method is called. Detection results are stored in
// the internal blood map helper object, a byte array. Afte Detec is called, the IsBlood-
// method can be used to check whether the specified pixel is blood or not.
//
// If image is color (ImageType), all channels are checked. In grayscale image, only
// the red channel is tested.
//
// Blood is searched only from the search area, which is specified with the SearchAreaCenter
// and SearchAreaRadius, and SearchAreaTopLeft and SearchAreaBottomRight. The later two are
// used to crop the search area if the center point is too close the border. When detector
// is initialized (Initialize-method) is called, internal helper object (m_pSearchMap) is
// created to hold the info about the pixels that are used in the detection
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

#include "SearchMap.h"


// --------------------------------------------------------------------------

// Blood detection method
enum eBloodDetectionMethod {
  DetectBlood = 0           // Detect blood: blood can be in any form, it doesn´t have to be a spot
};


// Color (min/max) range
typedef struct _COLOR_RANGE {

  USHORT usMinColor;        // Min color value
  USHORT usMaxColor;        // Max color value

} COLOR_RANGE, *PCOLOR_RANGE;


// Blood detector settings
typedef struct _BLOODDETECTORSETTINGS {

  USHORT                ImageWidth;                 // Input image width in pixels

  COLORREF              BloodColorMin;              // Min accepted color value. This and BloodColorMax, are used to
                                                    // specify the color which are recognised as blood.

  COLORREF              BloodColorMax;              // Max accepted color value. This and BloodColorMin, are used to
                                                    // specify the color which are recognised as blood.

  POINT                 SearchAreaCenter;           // Search area center. Blood is searched only from the search area,
                                                    // which is specified by SearchAreaRadius, SearchAreaTopLeft,
                                                    // SearchAreaBottomRight and this

  USHORT                SearchAreaRadius;           // Search area radius. Blood is searched only from the search are,
                                                    // which is specified by SearchAreaCenter, SearchAreaTopLeft,
                                                    // SearchAreaBottomRight and this

  POINT                 SearchAreaTopLeft;          // Search area top left corner, place where blood detection is started.
                                                    // Search area center should be greater than this. Helper to crop the 
                                                    // search area if the center is too close the border

  POINT                 SearchAreaBottomRight;      // Search area bottom right corner, place where blood detection is ended,
                                                    // excluding this. Search area center should be smaller than this. Helper
                                                    // to crop the search area if the center is too close the border

  eBloodDetectionMethod bdmDetectionMethod;         // Detection method
  eInputImageType       ImageType;                  // Input image type
                                                    // - When grayscale, only red channel is checked when detecting blood
                                                    // - When color, all channels are used when detecting blood

  char                  bloodDetectionPercent;      // Blood detection percent. Used if greater than zero when detecting
                                                    // blood: this tells how many percent of the detection area must contain
                                                    // blood that the Detect-method will return TRUE.

} BLOODDETECTORSETTINGS, *PBLOODDETECTORSETTINGS;



class BloodDetector {

  public:
    BloodDetector();
    ~BloodDetector();


                  // Initialize detector
    void          Initialize(const PBLOODDETECTORSETTINGS pSettings);

                  // Release detector
    void          Release();

                  // Detect blood. If blood detection percent is greater than zero, method will return TRUE
                  // if enough pixels contain blood. If detection percen is zero, method will return TRUE
                  // if one pixel containing blood was detected. Detection results is stored into blood map
                  // helper object. IsBlood-method can be used to check whether specified pixel contains
                  // blood or not
    BYTE          Detect(const PBYTE pData);

                  // Return TRUE if specified pixel contains blood. In order to use this, Detect-method needs 
                  // to be called first. Detect method caches the information into blood map helper object 
                  // (m_pBloodMap) and this retrieves the info from there.
    BYTE          IsBlood(USHORT x, USHORT y);


                  // Get current detector settings
    void          GetCurrentSettings(PBLOODDETECTORSETTINGS pSettings);

                  // Update settings
    void          UpdateSettings(const PBLOODDETECTORSETTINGS pSettings);


                  // Get the point where blood detection is started
    inline POINT  GetSearchAreaTopLeft() { return m_ptSearchAreaTopLeft; }

                  // Get the point where blood detection is ended, excluding this
    inline POINT  GetSearchAreaBottomRight() { return m_ptSearchAreaBottomRight; }

                  // Get the search area center. Blood is searched only from the search area.
    inline POINT  GetSearchAreaCenter() { return m_ptSearchAreaCenter; }

                  // Get the search area radius
    inline USHORT GetSearchAreaRadius() { return m_usSearchAreaRadius; }    

    // Maximum intensity for blood
	BYTE GetBloodDetectionColorMax();
	void SetBloodDetectionColorMax(BYTE value);


  private:

                          // When TRUE, detector has been initialized
    BYTE                  m_bInitialized;

                          // Input image width
    USHORT                m_usImageWidth;

                          // Search area center position. Blood is searched only from the search area
    POINT                 m_ptSearchAreaCenter;

                          // Search area radius. Blood is searched only from the search area
    USHORT                m_usSearchAreaRadius;

                          // Helper to hold the top left corner where blood detection is started
    POINT                 m_ptSearchAreaTopLeft;

                          // Helper to hold the bottom right corner where blood detection is ended, excluding this
    POINT                 m_ptSearchAreaBottomRight;

                          // Red channel blood color. Red channel values which fall into range that
                          // are recognised as blood
    COLOR_RANGE           m_crRedColorRange;

                          // Green channel blood color. Green channel values which fall into range that
                          // are recognised as blood
    COLOR_RANGE           m_crGreenColorRange;

                          // Blue channel blood color. Blue channel values which fall into range that
                          // are recognised as blood
    COLOR_RANGE           m_crBlueColorRange;


                          // Blood detection method
    eBloodDetectionMethod m_bdmDetectionMethod;

                          // Input image type
    eInputImageType       m_InputType;

                          // Helper to hold info about the pixels where the blood should be searched
    SearchMap*            m_pSearchMap;

                          // Required blood detection percent. If this is greater than 0, search area
                          // must contain at least sthe specified amount (percent) blood. If this is 
                          // zero, Detect-method returns TRUE, if one pixel contains blood
    char                  m_BloodDetectionPercent;


                          // Blood map width
    USHORT                m_usBloodMapWidth;

                          // Blood map height
    USHORT                m_usBloodMapHeight;

                          // Blood map. Helper object which will hold the results for the last detection 
                          // (Detect-method). When the maps entry is 1 (BLOODDETECTOR_BLOODMAP_BLOOD), the
                          // specified pixel contains blood.
    PBYTE                 m_pBloodMap;



            // Returns TRUE if the specified color is blood - it falls in the range of accepted colors.
            // When image type (m_InputType) is color, all channels are verified against the accepted 
            // colors (m_crRedColorRange, m_crGreenColorRange and m_crBlueColorRange). If the type is
            // grayscale, only the red channel is checked, because in grayscale image, all channels 
            // contains the same value.
    BYTE    IsBlood(const PSPOTLIGHTCOLOR color);

            // Copy settings to local variables
    void    CopySettings(const PBLOODDETECTORSETTINGS pSettings);

            // Initialize blood map, allocate space for it and set all positions to empty - 0
            // (BLOODDETECTOR_BLOODMAP_EMPTY)
    void    InitializeBloodMap();

            // Release blood map and free the memory allocated for it
    void    ReleaseBloodMap();

            // Initialize helper object which pixels should be included in the blood search
    void    InitializeSearchMap();

            // Get value from blood map
    BYTE    GetBloodMapValue(USHORT x, USHORT y);

            // Set blood map value. When value is 1 (BLOODDETECTOR_BLOODMAP_BLOOD), the specified
            // pixel (x, y) contains blood, when its 0 (BLOODDETECTOR_BLOODMAP_EMPTY), the pixel
            // is empty
    void    SetBloodMapValue(USHORT x, USHORT y, BYTE value);

            // Throw exception if detector is already initialized
    void    ThrowAlreadyInitializedException(const PTCHAR pszMethod);

            // Throw exception if detector isn´t initialized
    void    ThrowNotInitializedException(const PTCHAR pszMethod);

};