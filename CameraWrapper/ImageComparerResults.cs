// --------------------------------------------------------------------------
// ImageComparerResults
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
  
  public class ImageComparerResults {

    #region Constructors

    public ImageComparerResults() {
    }

    #endregion


    #region Public properties

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Point Center {
      get {
        return m_Center;
      }
      set {
        m_Center = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public int Radius {
      get {
        return m_Radius;
      }
      set {
        m_Radius = value;
      }
    }

    /// <summary>
    /// TODO: add desc
    /// </summary>
    public Bitmap ResultImage {
      get {
        return m_ResultImage;
      }
      set {
        m_ResultImage = value;
      }
    }

    #endregion


    #region Public data

    public const int UNKNOWN_X = -1;
    public const int UNKNOWN_Y = -1;
    public const int UNKNOWN_RADIUS = -1;

    #endregion


    #region Private data

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Point m_Center = new Point(UNKNOWN_X, UNKNOWN_Y);

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private Bitmap m_ResultImage = null;

    /// <summary>
    /// TODO: add desc
    /// </summary>
    private int m_Radius = UNKNOWN_RADIUS;

    #endregion

  }

}