// --------------------------------------------------------------------------
// ExceptionInfo.cpp
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


#include "stdafx.h"
#include "ExceptionInfo.h"


ExceptionInfo::ExceptionInfo(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode) {

  m_uiErrorCode = uiErrorCode;
  m_HResult = S_OK;

  CopyString(pszClass, m_pszClass);
  CopyString(pszMethod, m_pszMethod);

}

ExceptionInfo::ExceptionInfo(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode, HRESULT result) {

  m_uiErrorCode = uiErrorCode;
  m_HResult = result;

  CopyString(pszClass, m_pszClass);
  CopyString(pszMethod, m_pszMethod);

}

ExceptionInfo::~ExceptionInfo() {

  ReleaseString(m_pszClass);
  ReleaseString(m_pszMethod);

}

void ExceptionInfo::Throw(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode) {
  throw new ExceptionInfo(pszClass, pszMethod, uiErrorCode);
}

void ExceptionInfo::Throw(const PTCHAR pszClass, const PTCHAR pszMethod, UINT uiErrorCode, HRESULT result) {
  throw new ExceptionInfo(pszClass, pszMethod, uiErrorCode, result);
}
