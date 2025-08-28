// --------------------------------------------------------------------------
// ImageComparerException
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

namespace PKI.Puncher.CameraWrapper {
 
  public class ImageComparerException : Exception {

    #region Constructors

    public ImageComparerException(string className, string methodName, int errorCode)
      : base() {

      m_ClassName = className;
      m_MethodName = methodName;
      m_ErrorCode = errorCode;

    }

    #endregion


    #region Public properties

    public string ClassName {
      get {
        return m_ClassName;
      }
      set {
        m_ClassName = value;
      }
    }
    public string MethodName {
      get {
        return m_MethodName;
      }
      set {
        m_MethodName = value;
      }
    }
    public int ErrorCode {
      get {
        return m_ErrorCode;
      }
      set {
        m_ErrorCode = value;
      }
    }

    #endregion


    #region Private data

    private string m_ClassName = string.Empty;
    private string m_MethodName = string.Empty;
    private int m_ErrorCode = 0;

    #endregion

  }

}