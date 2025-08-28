// --------------------------------------------------------------------------
// Hotspot.h
// --------------------------------------------------------------------------
//
// Helper class that is used to cache part of the image, and later compare it
// to new image, and tell if they are different. When comparing images, index
// value is used. When the image type (m_ImageType) is color, all color channels
// are add together and that value is used as a index value. When the type is
// grayscale, only the value in the red channel is used as a index value.
//
// Detection accuracy values (m_DetectionAccuracyX/m_DetectionAccuracyY) can be
// used to reduce the CPU load - those values specify how many pixels are skipped
// when detecting whether the data has changed or not.
//
// Min/max offset values (m_Accepted) are used to specify how much the index value
// can vary from the reference data.
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C++ 2008
//
// $Header: $
// Created: 11.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

#pragma once


class Hotspot {

  public:
    Hotspot();
    ~Hotspot();

                      // Create (/cache) hotspot data and cache variables that are used later, when IsSame-
                      // method is called, to check if the data has changed
    void              Create(const PPOINT pTopLeft, const PPOINT pBottomRight, const PINDEXVALUERANGE pAccepted,
                             eInputImageType imageType, BYTE invalidPercent, USHORT sourceWidth,
                             BYTE detectionAccuracyX, BYTE detectionAccuracyY, const PBYTE pSource);

                      // Returns FALSE if the area in the passed image (pSource) has changed from the one that
                      // was cached during initialization. If m_InvalidPercent is 0, FALSE is returned if one
                      // of the processed pixels have changed, otherwise the number of the processed pixels that
                      // have changed (percent) must be greater than the value in m_InvalidPercent
    BYTE              IsSame(const PBYTE pSource);

                      // Get hotspots top left corner
    inline POINT      GetTopLeftCorner()      { return m_TopLeft; }

                      // Get hotspots bottom right corner
    inline POINT      GetBottomRightCorner()  { return m_BottomRight; }

                      // Get pixels index value. If the imageType is color, all channels are add together, and
                      // the sum is returned. When the imageType is grayscale, only the value in the red channel
                      // is returned.
    static USHORT     GetIndex(eInputImageType imageType, const PSPOTLIGHTCOLOR color);



  private:

                      // Type of the used image. Used when calculating index values
    eInputImageType   m_ImageType;

                      // Hotspots top left corner
    POINT             m_TopLeft;

                      // Hotspots bottom right corner
    POINT             m_BottomRight;

                      // Number of pixels, in X axis, that is skipped when detecting if
                      // the image has changed
    BYTE              m_DetectionAccuracyX;

                      // Number of pixels, in Y axis, that is skipped when detecting if
                      // the image has changed
    BYTE              m_DetectionAccuracyY;

                      // Cached image data. Contains index values that are calculated from
                      // reference image
    PUSHORT           m_pHotspotData;

                      // Width of the reference/tested image
    USHORT            m_ImageWidth;

                      // Min/max offset values that are used to detect if specific pixel have changed
    INDEXVALUERANGE   m_Accepted;

                      // Number of changed processed pixels (percent) that the cached hotspot data must 
                      // contain before it is considered to be changed (return value for the IsSame-method).
                      // If this is 0, IsSame-method will return FALSE if only 1 of the pixel is changed,
    BYTE              m_RequiredChangePercent;


            // Releas hotspot data
    void    ReleaseHotspotData();

            // Allocate space for the hotspot data
    void    CreateHotspotData(const PPOINT pTopLeft, const PPOINT pBottomRight);

            // Copy data from the image (pSource) to hotspot cache.
    void    CopyHotspotData(USHORT sourceWidth, const PBYTE pSource);

};