// --------------------------------------------------------------------------
// ImageComparer
// --------------------------------------------------------------------------
//
// TODO: add desc
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C#.NET 2008
//
// $Header: $
// Created: 21.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

using System;
using System.Drawing;

namespace PKI.Puncher.CameraWrapper {

  public class ImageComparerSettings {

    #region Constructors

    public ImageComparerSettings() {
    }

    #endregion


    #region Public properties

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Bitmap Image1 {
      get {
        return m_Image1;
      }
      set {
        m_Image1 = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Bitmap Image2 {
      get {
        return m_Image2;
      }
      set {
        m_Image2 = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Point SearchAreaCenter {
      get {
        return m_SearchAreaCenter;
      }
      set {
        m_SearchAreaCenter = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public ushort SearchAreaRadius {
      get {
        return m_SearchAreaRadius;
      }
      set {
        m_SearchAreaRadius = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int DetectionLimitMin {
      get {
        return m_DetectionLimitMin;
      }
      set {
        m_DetectionLimitMin = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int DetectionLimitMax {
      get {
        return m_DetectionLimitMax;
      }
      set {
        m_DetectionLimitMax = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool GenerateResultImage {
      get {
        return m_GenerateResultImage;
      }
      set {
        m_GenerateResultImage = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool DrawInvalidPixels {
      get {
        return m_DrawInvalidPixels;
      }
      set {
        m_DrawInvalidPixels = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Color InvalidPixelColor {
      get {
        return m_InvalidPixelColor;
      }
      set {
        m_InvalidPixelColor = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool DrawCenter {
      get {
        return m_DrawCenter;
      }
      set {
        m_DrawCenter = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Color CenterColor {
      get {
        return m_CenterColor;
      }
      set {
        m_CenterColor = value;
      }
    }

    #endregion


    #region Private data

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Bitmap m_Image1 = null;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Bitmap m_Image2 = null;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Point m_SearchAreaCenter = Point.Empty;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private ushort m_SearchAreaRadius = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_DetectionLimitMin = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_DetectionLimitMax = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_GenerateResultImage = true;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_DrawInvalidPixels = true;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Color m_InvalidPixelColor = Color.Black;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_DrawCenter = true;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Color m_CenterColor = Color.White;

    #endregion

  }

}