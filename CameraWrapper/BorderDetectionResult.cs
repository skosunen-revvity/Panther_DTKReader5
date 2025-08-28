// --------------------------------------------------------------------------
// BorderDetector
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
using System.Collections.Generic;
using System.Drawing;

namespace PKI.Puncher.CameraWrapper {
  
  public class BorderDetectionResult {

    #region Constructors

    public BorderDetectionResult() {
    }

    #endregion


    #region Public properties

    public Point Top {
      get {
        return m_Top;
      }
      set {
        m_Top = value;
      }
    }
    public List<Point> TopBorder {
      get {
        return m_TopBorder;
      }
      set {
        m_TopBorder = value;
      }
    }

    public Point Left {
      get {
        return m_Left;
      }
      set {
        m_Left = value;
      }
    }
    public List<Point> LeftBorder {
      get {
        return m_LeftBorder;
      }
      set {
        m_LeftBorder = value;
      }
    }

    public Point Bottom {
      get {
        return m_Bottom;
      }
      set {
        m_Bottom = value;
      }
    }
    public List<Point> BottomBorder {
      get {
        return m_BottomBorder;
      }
      set {
        m_BottomBorder = value;
      }
    }

    public Point Right {
      get {
        return m_Right;
      }
      set {
        m_Right = value;
      }
    }
    public List<Point> RightBorder {
      get {
        return m_RightBorder;
      }
      set {
        m_RightBorder = value;
      }
    }

    public bool Succeeded {
      get {

        if (m_TopBorder != null && m_LeftBorder != null && m_BottomBorder != null && m_RightBorder != null) {
          if (m_TopBorder.Count > 0 && m_LeftBorder.Count > 0 &&
            m_BottomBorder.Count > 0 && m_RightBorder.Count > 0) {

            if (Left.X <= Right.X && Top.Y <= Bottom.Y) {
              return true;
            }
          }
        }

        return false;

      }
    }

    #endregion


    #region Private data

    private Point m_Top = Point.Empty;
    private List<Point> m_TopBorder = null;

    private Point m_Left = Point.Empty;
    private List<Point> m_LeftBorder = null;

    private Point m_Bottom = Point.Empty;
    private List<Point> m_BottomBorder = null;

    private Point m_Right = Point.Empty;
    private List<Point> m_RightBorder = null;

    #endregion

  }

}