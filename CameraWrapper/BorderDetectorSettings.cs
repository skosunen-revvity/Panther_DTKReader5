// --------------------------------------------------------------------------
// BorderDetectorSettings
// --------------------------------------------------------------------------
//
// TODO: add desc
//
// Copyright © 2010 PerkinElmer, Wallac Oy
// Microsoft Visual C#.NET 2008
//
// $Header: $
// Created: 02.06.2010 by KiLa
//
// --------------------------------------------------------------------------
// $Nokeywords: $

using System;
using System.Drawing;

namespace PKI.Puncher.CameraWrapper {

  public class BorderDetectorSettings {

    #region Constructors

    public BorderDetectorSettings() {
    }

    #endregion


    #region Public properties

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Bitmap Image {
      get {
        return m_Image;
      }
      set {
        m_Image = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Color BorderColorMin {
      get {
        return m_BorderColorMin;
      }
      set {
        m_BorderColorMin = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Color BorderColorMax {
      get {
        return m_BorderColorMax;
      }
      set {
        m_BorderColorMax = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int TopDetectionOffset {
      get {
        return m_TopDetectionOffset;
      }
      set {
        m_TopDetectionOffset = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int LeftDetectionOffset {
      get {
        return m_LeftDetectionOffset;
      }
      set {
        m_LeftDetectionOffset = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int BottomDetectionOffset {
      get {
        return m_BottomDetectionOffset;
      }
      set {
        m_BottomDetectionOffset = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int RightDetectionOffset {
      get {
        return m_RightDetectionOffset;
      }
      set {
        m_RightDetectionOffset = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int HorizontalDetectionAccuracy {
      get {
        return m_HorizontalDetectionAccuracy;
      }
      set {
        m_HorizontalDetectionAccuracy = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int VerticalDetectionAccuracy {
      get {
        return m_VerticalDetectionAccuracy;
      }
      set {
        m_VerticalDetectionAccuracy = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool UseMaxDetectionPointTop {
      get {
        return m_UseMaxDetectionPointTop;
      }
      set {
        m_UseMaxDetectionPointTop = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool UseMaxDetectionPointLeft {
      get {
        return m_UseMaxDetectionPointLeft;
      }
      set {
        m_UseMaxDetectionPointLeft = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool UseMaxDetectionPointBottom {
      get {
        return m_UseMaxDetectionPointBottom;
      }
      set {
        m_UseMaxDetectionPointBottom = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public bool UseMaxDetectionPointRight {
      get {
        return m_UseMaxDetectionPointRight;
      }
      set {
        m_UseMaxDetectionPointRight = value;
      }
    }

    #endregion


    #region Private data

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Bitmap m_Image = null;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Color m_BorderColorMin = Color.Black;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Color m_BorderColorMax = Color.White;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_TopDetectionOffset = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_LeftDetectionOffset = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_BottomDetectionOffset = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_RightDetectionOffset = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_HorizontalDetectionAccuracy = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_VerticalDetectionAccuracy = 0;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_UseMaxDetectionPointTop = false;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_UseMaxDetectionPointLeft = false;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_UseMaxDetectionPointBottom = false;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private bool m_UseMaxDetectionPointRight = false;

    #endregion

  }

}