// --------------------------------------------------------------------------
// SimpleLogger.cpp
// --------------------------------------------------------------------------

#include "stdafx.h"
#include "SimpleLogger.h"
#include <cstdio>
#include <ctime>

HANDLE SimpleLogger::m_hLogFile                 = INVALID_HANDLE_VALUE;
CRITICAL_SECTION SimpleLogger::m_criticalSection;
BOOL   SimpleLogger::m_bInitialized             = FALSE;
TCHAR  SimpleLogger::m_szLogFilePath[MAX_PATH]  = {0};

static const DWORD LOG_SHARE_MODE = FILE_SHARE_READ | FILE_SHARE_WRITE;

int SimpleLogger::Utf16ToUtf8(LPCWSTR src, char* dst, int dstSize)
{
#ifdef UNICODE
    if (!src) return 0;
    int needed = WideCharToMultiByte(CP_UTF8, 0, src, -1, nullptr, 0, nullptr, nullptr);
    if (needed <= 0) return 0;
    if (dst && dstSize > 0)
        WideCharToMultiByte(CP_UTF8, 0, src, -1, dst, dstSize, nullptr, nullptr);
    return needed;
#else
    (void)src; (void)dst; (void)dstSize;
    return 0;
#endif
}

static void BuildRunHeaderLine(char* out, size_t cap, const SYSTEMTIME& st, bool start)
{
    // Format: ========== RUN START yyyy-mm-dd HH:MM:SS.mmm PID=#### TID=#### EXE=filename ==========
    _snprintf_s(out, cap, _TRUNCATE,
        "\r\n========== RUN %s %04u-%02u-%02u %02u:%02u:%02u.%03u PID=%lu TID=%lu ==========\r\n",
        start ? "START" : "END  ",
        st.wYear, st.wMonth, st.wDay,
        st.wHour, st.wMinute, st.wSecond, st.wMilliseconds,
        (unsigned long)GetCurrentProcessId(),
        (unsigned long)GetCurrentThreadId());
}

static void WriteRunBoundary(HANDLE hFile, bool start)
{
    if (hFile == INVALID_HANDLE_VALUE) return;
    SYSTEMTIME st; GetLocalTime(&st);
    char line[256];
    BuildRunHeaderLine(line, sizeof(line), st, start);
    DWORD written;
    WriteFile(hFile, line, (DWORD)strlen(line), &written, nullptr);
}

BOOL SimpleLogger::Initialize(LPCTSTR pszLogFilePath, bool append)
{
    if (m_bInitialized) return TRUE;
    if (!pszLogFilePath || !*pszLogFilePath) return FALSE;

    _tcsncpy_s(m_szLogFilePath, pszLogFilePath, _TRUNCATE);

    // Ensure directory exists
    TCHAR dir[MAX_PATH];
    _tcsncpy_s(dir, pszLogFilePath, _TRUNCATE);
    if (TCHAR* p = _tcsrchr(dir, _T('\\'))) {
        *p = 0;
        if (*dir) CreateDirectory(dir, nullptr);
    }

    InitializeCriticalSection(&m_criticalSection);

    DWORD creation = append ? OPEN_ALWAYS : CREATE_ALWAYS;

#ifdef UNICODE
    m_hLogFile = CreateFileW(pszLogFilePath, GENERIC_WRITE, LOG_SHARE_MODE,
                             nullptr, creation, FILE_ATTRIBUTE_NORMAL, nullptr);
#else
    m_hLogFile = CreateFileA(pszLogFilePath, GENERIC_WRITE, LOG_SHARE_MODE,
                             nullptr, creation, FILE_ATTRIBUTE_NORMAL, nullptr);
#endif
    if (m_hLogFile == INVALID_HANDLE_VALUE) {
        DeleteCriticalSection(&m_criticalSection);
        return FALSE;
    }

    if (append && creation == OPEN_ALWAYS) {
        SetFilePointer(m_hLogFile, 0, nullptr, FILE_END);
    }

    m_bInitialized = TRUE;

    // Write run start delimiter (NOT using log functions to avoid lock re-entry complications)
    WriteRunBoundary(m_hLogFile, true);

    // Also write a standard initialized info line
    FormatAndWrite("INFO ", _T("SimpleLogger"), _T("Initialize"), _T("Logger initialized"), 0, false);
    return TRUE;
}

void SimpleLogger::Cleanup()
{
    if (!m_bInitialized) return;

    EnterCriticalSection(&m_criticalSection);
    // Run end delimiter before closing
    WriteRunBoundary(m_hLogFile, false);
    FormatAndWrite("INFO ", _T("SimpleLogger"), _T("Cleanup"), _T("Logger shutting down"), 0, false);

    if (m_hLogFile != INVALID_HANDLE_VALUE) {
        CloseHandle(m_hLogFile);
        m_hLogFile = INVALID_HANDLE_VALUE;
    }
    m_bInitialized = FALSE;
    LeaveCriticalSection(&m_criticalSection);
    DeleteCriticalSection(&m_criticalSection);
}

void SimpleLogger::WriteRaw(const char* data, size_t len)
{
    if (!m_bInitialized || m_hLogFile == INVALID_HANDLE_VALUE || !data || !len)
        return;
    DWORD written;
    WriteFile(m_hLogFile, data, (DWORD)len, &written, nullptr);
}

void SimpleLogger::GetTimestamp(char* buffer, size_t bufferSize)
{
    SYSTEMTIME st; GetLocalTime(&st);
    _snprintf_s(buffer, bufferSize, _TRUNCATE,
                "%04u-%02u-%02u %02u:%02u:%02u.%03u",
                st.wYear, st.wMonth, st.wDay,
                st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
}

void SimpleLogger::FormatAndWrite(const char* level,
                                  LPCTSTR cls,
                                  LPCTSTR method,
                                  LPCTSTR msg,
                                  DWORD errorCode,
                                  bool hasError)
{
    if (!m_bInitialized) return;

    char timeBuf[40]; GetTimestamp(timeBuf, sizeof(timeBuf));

#ifdef UNICODE
    char clsUtf8[256]  = "Unknown";
    char methUtf8[256] = "Unknown";
    char msgUtf8[1024] = "(null)";
    if (cls)   WideCharToMultiByte(CP_UTF8, 0, cls,   -1, clsUtf8,  sizeof(clsUtf8),  nullptr, nullptr);
    if (method)WideCharToMultiByte(CP_UTF8, 0, method,-1, methUtf8, sizeof(methUtf8), nullptr, nullptr);
    if (msg)   WideCharToMultiByte(CP_UTF8, 0, msg,   -1, msgUtf8,  sizeof(msgUtf8),  nullptr, nullptr);
#else
    const char* clsUtf8  = cls   ? cls   : "Unknown";
    const char* methUtf8 = method? method: "Unknown";
    const char* msgUtf8  = msg   ? msg   : "(null)";
#endif

    char line[2048];
    if (hasError)
        _snprintf_s(line, sizeof(line), _TRUNCATE,
                    "[%s] %s [%s::%s] %s (Error: 0x%08X)\r\n",
                    timeBuf, level, clsUtf8, methUtf8, msgUtf8, errorCode);
    else
        _snprintf_s(line, sizeof(line), _TRUNCATE,
                    "[%s] %s [%s::%s] %s\r\n",
                    timeBuf, level, clsUtf8, methUtf8, msgUtf8);

    WriteRaw(line, strlen(line));
}

void SimpleLogger::LogInfo(LPCTSTR cls, LPCTSTR method, LPCTSTR msg)
{
    if (!m_bInitialized) return;
    EnterCriticalSection(&m_criticalSection);
    FormatAndWrite("INFO ", cls, method, msg, 0, false);
    LeaveCriticalSection(&m_criticalSection);
}

void SimpleLogger::LogWarning(LPCTSTR cls, LPCTSTR method, LPCTSTR msg)
{
    if (!m_bInitialized) return;
    EnterCriticalSection(&m_criticalSection);
    FormatAndWrite("WARN ", cls, method, msg, 0, false);
    LeaveCriticalSection(&m_criticalSection);
}

void SimpleLogger::LogError(LPCTSTR cls, LPCTSTR method, LPCTSTR msg)
{
    if (!m_bInitialized) return;
    EnterCriticalSection(&m_criticalSection);
    FormatAndWrite("ERROR", cls, method, msg, 0, false);
    LeaveCriticalSection(&m_criticalSection);
}

void SimpleLogger::LogError(LPCTSTR cls, LPCTSTR method, LPCTSTR msg, DWORD err)
{
    if (!m_bInitialized) return;
    EnterCriticalSection(&m_criticalSection);
    FormatAndWrite("ERROR", cls, method, msg, err, true);
    LeaveCriticalSection(&m_criticalSection);
}