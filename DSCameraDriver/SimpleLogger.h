// --------------------------------------------------------------------------
// SimpleLogger.h
// --------------------------------------------------------------------------
//
// Simple file logger implementation for DSCamera
// Thread safe, supports UNICODE (converts UTF-16 -> UTF-8 before writing)
// --------------------------------------------------------------------------

#pragma once

#include "stdafx.h"
#include <windows.h>

class SimpleLogger {
private:
    static HANDLE           m_hLogFile;
    static CRITICAL_SECTION m_criticalSection;
    static BOOL             m_bInitialized;
    static TCHAR            m_szLogFilePath[MAX_PATH];

    SimpleLogger() = delete;

    static void   WriteRaw(const char* data, size_t len);
    static void   GetTimestamp(char* buffer, size_t bufferSize);
    static void   FormatAndWrite(const char* level,
                                 LPCTSTR cls,
                                 LPCTSTR method,
                                 LPCTSTR msg,
                                 DWORD errorCode,
                                 bool hasError);
    static int    Utf16ToUtf8(LPCWSTR src, char* dst, int dstSize);

public:
    static BOOL Initialize(LPCTSTR pszLogFilePath, bool append = true);
    static void Cleanup();

    // Public log helpers are now no-ops if not initialized (prevent AV before init / after cleanup)
    static void LogInfo   (LPCTSTR cls, LPCTSTR method, LPCTSTR msg);
    static void LogWarning(LPCTSTR cls, LPCTSTR method, LPCTSTR msg);
    static void LogError  (LPCTSTR cls, LPCTSTR method, LPCTSTR msg);
    static void LogError  (LPCTSTR cls, LPCTSTR method, LPCTSTR msg, DWORD errorCode);

    static BOOL IsInitialized() { return m_bInitialized; }
};

// Convenience macros
#define LOG_INFO(className, methodName, message) \
    SimpleLogger::LogInfo(className, methodName, message)

#define LOG_WARNING(className, methodName, message) \
    SimpleLogger::LogWarning(className, methodName, message)

#define LOG_ERROR(className, methodName, message) \
    SimpleLogger::LogError(className, methodName, message)

#define LOG_ERROR_CODE(className, methodName, message, errorCode) \
    SimpleLogger::LogError(className, methodName, message, errorCode)