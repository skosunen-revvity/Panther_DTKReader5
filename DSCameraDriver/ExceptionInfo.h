// --------------------------------------------------------------------------
// ExceptionInfo.h
// --------------------------------------------------------------------------
//
// Exception helper class
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

class ExceptionInfo {

  public:
    ExceptionInfo(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode);
    ExceptionInfo(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode, HRESULT result);
    ~ExceptionInfo();

    inline UINT         GetErrorCode()                { return m_uiErrorCode; }
    inline HRESULT      GetHResult()                  { return m_HResult; }  

    inline const PTCHAR GetClass()                    { return m_pszClass; }
    inline const PTCHAR GetMethod()                   { return m_pszMethod; }

    inline void         SetHResult(HRESULT result)    { m_HResult = result; }

    static void         Throw(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode);
    static void         Throw(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode, HRESULT result);

  protected:

    UINT      m_uiErrorCode;
    PTCHAR    m_pszClass;
    PTCHAR    m_pszMethod;

    HRESULT   m_HResult;

    ExceptionInfo();


};