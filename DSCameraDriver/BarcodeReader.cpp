// --------------------------------------------------------------------------
// BarcodeReader.cpp  (DTK v5 Migration - Unlimited License Key + Offline Activation Call)
// --------------------------------------------------------------------------
// This build uses a vendor provided UNLIMITED SERVER 1D license key:
//   PN8YV-1XUDV-LV8SY-W0U2H-NMXKL
//
// Vendor email requirement:
//   "Include the activation code in program code using
//    BarcodeReader.ActivateLicenseOffline ... you don't need to activate anything."
// Interpretation: The DLL expects a one-time call to BarcodeReader_ActivateLicenseOffline
// with the vendor's long offline activation token BEFORE creating the internal
// BarcodeReader instance (BarcodeReader_Create). We perform that call (if exported)
// prior to reader creation in Initialize()/Load sequence.
//
// Token handling:
//   - Provide token via compile-time constant DTK_OFFLINE_ACTIVATION_CODE (preferred empty here)
//     OR environment variable: DTK_BARCODE_OFFLINE_CODE
//   - We never log the full token; only masked tail.
//
// We still:
//   * Probe for license key setters (global/handle) and apply if exported.
//   * Validate license AFTER creating the internal reader handle (some builds mark valid only then).
//   * Support 1D + optional DataMatrix (remove if strictly 1D).
// --------------------------------------------------------------------------

#include "stdafx.h"
#include <string>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include "windows.h"
#include "BarcodeReader.h"
#include "ExceptionInfo.h"
#include "DTKBarReader5.h"
#include "SimpleLogger.h"

_CLASSNAME(_T("BarcodeReader"))

// External helpers assumed present
extern void CopyString(LPCTSTR src, LPTSTR& dst);
extern void ReleaseString(LPTSTR& str);

// ================= v5 function pointer typedefs =================
typedef BARREADERRESULT(DTKBARAPI* PFN_ReadFromImageBuffer)(BARREADER, void*, int, int, int, PixelFormatEnum);
typedef void (DTKBARAPI* PFN_set_RectX)(BARREADER, int);
typedef void (DTKBARAPI* PFN_set_RectY)(BARREADER, int);
typedef void (DTKBARAPI* PFN_set_RectW)(BARREADER, int);
typedef void (DTKBARAPI* PFN_set_RectH)(BARREADER, int);
typedef void (DTKBARAPI* PFN_set_BarcodeTypes)(BARREADER, BarcodeTypeEnum);
typedef void (DTKBARAPI* PFN_set_Orientation)(BARREADER, BarcodeOrientationEnum);
typedef int  (DTKBARAPI* PFN_get_Error)(BARREADERRESULT);
typedef int  (DTKBARAPI* PFN_get_Count)(BARREADERRESULT);
typedef BARCODE(DTKBARAPI* PFN_get_Barcode)(BARREADERRESULT, int);
typedef int  (DTKBARAPI* PFN_get_Text)(BARCODE, char*, int);
typedef BarcodeTypeEnum(DTKBARAPI* PFN_get_Type)(BARCODE);
typedef void (DTKBARAPI* PFN_destroy_Barcode)(BARCODE);
typedef void (DTKBARAPI* PFN_destroy_Result)(BARREADERRESULT);
typedef void (DTKBARAPI* PFN_GetLicenseInfo)(char*, int, char*, int, int*, time_t*, char*, int, int*);

// License related optional APIs
typedef int (DTKBARAPI* PFN_SetLicenseKey_Global)(const char*);
typedef int (DTKBARAPI* PFN_SetLicenseKey_Handle)(BARREADER, const char*);
typedef int (DTKBARAPI* PFN_ActivateLicenseOffline)(const char*);

// ================= Function pointers =================
static PFN_ReadFromImageBuffer  pReadFromImageBuffer = nullptr;
static PFN_set_RectX            pSetRectX = nullptr;
static PFN_set_RectY            pSetRectY = nullptr;
static PFN_set_RectW            pSetRectW = nullptr;
static PFN_set_RectH            pSetRectH = nullptr;
static PFN_set_BarcodeTypes     pSetBarcodeTypes = nullptr;
static PFN_set_Orientation      pSetOrientation = nullptr;
static PFN_get_Error            pGetResultError = nullptr;
static PFN_get_Count            pGetBarcodesCount = nullptr;
static PFN_get_Barcode          pGetBarcode = nullptr;
static PFN_get_Text             pGetBarcodeText = nullptr;
static PFN_get_Type             pGetBarcodeType = nullptr;
static PFN_destroy_Barcode      pDestroyBarcode = nullptr;
static PFN_destroy_Result       pDestroyResult = nullptr;
static PFN_GetLicenseInfo       pGetLicenseInfo = nullptr;
static PFN_SetLicenseKey_Global pSetLicenseKeyGlobal = nullptr;
static PFN_SetLicenseKey_Handle pSetLicenseKeyHandle = nullptr;
static PFN_ActivateLicenseOffline pActivateLicenseOffline = nullptr;

// ---- Unlimited license key + offline activation token ----
static const char* const DTK_LICENSE_KEY = "PN8YV-1XUDV-LV8SY-W0U2H-NMXKL";
// Put the LONG vendor offline activation token here if you want to compile it in,
// otherwise leave empty and supply via environment variable DTK_BARCODE_OFFLINE_CODE.
static const char* const DTK_OFFLINE_ACTIVATION_CODE = "nUccO+vHoeLqk1KXWOEo0XUvCTHhY1niBXiAaeg0lww+3ipzXg6DcqzO34deJ0llJRtIqC3+nqfpH2XtR+3H/G5EiLHJyMlqasOi6HaOx9T1jIRB5ftUKAxcqC+eeyVwepjlHSfouxmQIrWVkTMQjLNkqWjN+5cJVpycy3OmtEIf7sTGAlzfl0PfMswhMfe43KjcmpNbZWW3hWMTkfSxWRZIvh/+xiGt4b3vZXpE3r9pYfoq5VPfoedkhTosUwGnEb2rfLGwWw48gJ15cTfvd3oOKSPAOBs5vwY206RIAo2JejlDW53s1nPy4Xd4PhgNea/x4Pr5ygTnbZW1YZSGq4YdNJy2RbJvMdo7oPrBLQtM"; // TODO: paste vendor offline activation token OR use env var.

static bool g_LicenseChecked = false;
static bool g_OfflineActivationAttempted = false;
static bool g_LoggedNoKeyApi = false;

// Logger init (idempotent)
static void InitLogger()
{
    if (SimpleLogger::IsInitialized())
        return;
    CreateDirectoryW(L"C:\\SpotLight", nullptr);
    CreateDirectoryW(L"C:\\SpotLight\\Logs", nullptr);
    SimpleLogger::Initialize(L"C:\\SpotLight\\Logs\\BarcodeReaderLog.txt", true);
}

static const TCHAR* TypeToString(BarcodeTypeEnum t)
{
    switch (t) {
    case BT_Code39: return _T("Code39");
    case BT_Inter2of5: return _T("Interleaved2of5");
    case BT_UPCA: return _T("UPCA");
    case BT_UPCE: return _T("UPCE");
    case BT_EAN8: return _T("EAN8");
    case BT_EAN13: return _T("EAN13");
    case BT_Codabar: return _T("Codabar");
    case BT_Code128: return _T("Code128");
    case BT_Code93: return _T("Code93");
    case BT_Code11: return _T("Code11");
    case BT_DataMatrix: return _T("DataMatrix");
    case BT_QRCode: return _T("QRCode");
    case BT_PDF417: return _T("PDF417");
    default: return _T("Unknown");
    }
}

// Engine error text mapping
static const TCHAR* DescribeEngineError(int code)
{
    switch (code) {
    case 7101: return _T("Buffer is NULL");
    case 7102: return _T("Invalid scan rectangle");
    case 7103: return _T("Filename error");
    case 7104: return _T("Recognition timeout");
    case 7105: return _T("Image decode failure");
    case 7106: return _T("Unsupported pixel format");
    case 7107: return _T("TIFF decode error");
    case 7201: return _T("No license (unexpected for unlimited)");
    case 7202: return _T("Trial validation failed (unexpected for unlimited)");
    case 7301: return _T("PDF open error");
    case 7302: return _T("PDF wrong password");
    case 7303: return _T("PDF wrong page number");
    default:   return _T("Unknown engine error");
    }
}

// Mask license key for logs
static std::wstring MaskLicenseKey()
{
#ifdef UNICODE
    const char* k = DTK_LICENSE_KEY;
    if (!k) return L"(null)";
    size_t len = std::strlen(k);
    if (len <= 4) {
        int wl = MultiByteToWideChar(CP_UTF8, 0, k, -1, nullptr, 0);
        std::wstring w; w.resize(wl ? wl - 1 : 0);
        if (wl > 1) MultiByteToWideChar(CP_UTF8, 0, k, -1, &w[0], wl - 1);
        return w;
    }
    std::wstring masked = L"****-****-****-****-";
    const char* tail = k + (len - 4);
    int wl = MultiByteToWideChar(CP_UTF8, 0, tail, -1, nullptr, 0);
    std::wstring wtail; wtail.resize(wl ? wl - 1 : 0);
    if (wl > 1) MultiByteToWideChar(CP_UTF8, 0, tail, -1, &wtail[0], wl - 1);
    masked += wtail;
    return masked;
#else
    const char* k = DTK_LICENSE_KEY;
    if (!k) return "(null)";
    size_t len = std::strlen(k);
    if (len <= 4) return std::string(k);
    return std::string("****-****-****-****-") + std::string(k + len - 4);
#endif
}

// Mask activation token tail only
static std::wstring MaskActivationToken(const char* token)
{
#ifdef UNICODE
    if (!token || !*token) return L"(none)";
    size_t len = std::strlen(token);
    size_t tail = (len > 10) ? 10 : len;
    const char* t = token + (len - tail);
    int wl = MultiByteToWideChar(CP_UTF8, 0, t, (int)tail, nullptr, 0);
    std::wstring w; w.resize(wl);
    if (wl) MultiByteToWideChar(CP_UTF8, 0, t, (int)tail, &w[0], wl);
    return std::wstring(L"...") + w;
#else
    if (!token || !*token) return "(none)";
    size_t len = std::strlen(token);
    size_t tail = (len > 10) ? 10 : len;
    return std::string("...") + std::string(token + len - tail);
#endif
}

// Apply license key if API present
static void ApplyLicenseKeyIfPossible(BARREADER hReader)
{
#ifdef UNICODE
    std::wstring masked = MaskLicenseKey();
    const TCHAR* mk = masked.c_str();
#else
    std::string masked = MaskLicenseKey();
    const TCHAR* mk = masked.c_str();
#endif

    if (hReader && pSetLicenseKeyHandle) {
        int rc = pSetLicenseKeyHandle(hReader, DTK_LICENSE_KEY);
        if (rc == 0) {
            TCHAR buf[160]; _stprintf(buf, _T("Handle license key applied (%s)"), mk);
            LOG_INFO(_T("BarcodeReader"), _T("License"), buf);
        }
        else {
            TCHAR buf[160]; _stprintf(buf, _T("Handle license key apply failed rc=%d (%s)"), rc, mk);
            LOG_WARNING(_T("BarcodeReader"), _T("License"), buf);
        }
        return;
    }
    if (pSetLicenseKeyGlobal) {
        int rc = pSetLicenseKeyGlobal(DTK_LICENSE_KEY);
        if (rc == 0) {
            TCHAR buf[160]; _stprintf(buf, _T("Global license key applied (%s)"), mk);
            LOG_INFO(_T("BarcodeReader"), _T("License"), buf);
        }
        else {
            TCHAR buf[160]; _stprintf(buf, _T("Global license key apply failed rc=%d (%s)"), rc, mk);
            LOG_WARNING(_T("BarcodeReader"), _T("License"), buf);
        }
    }
    else if (!g_LoggedNoKeyApi) {
        LOG_INFO(_T("BarcodeReader"), _T("License"), _T("No license key API exported (assuming embedded unlimited build)"));
        g_LoggedNoKeyApi = true;
    }
}

// Offline activation attempt (if export available)
static void TryOfflineActivation()
{
    if (g_OfflineActivationAttempted) return;
    g_OfflineActivationAttempted = true;

    if (!pActivateLicenseOffline) {
        LOG_INFO(_T("BarcodeReader"), _T("License"), _T("Offline activation export not found (may be unnecessary)"));
        return;
    }

    // Acquire token
    const char* token = DTK_OFFLINE_ACTIVATION_CODE;
    if (!token || !*token) {
        token = std::getenv("DTK_BARCODE_OFFLINE_CODE");
    }
    if (!token || !*token) {
        LOG_INFO(_T("BarcodeReader"), _T("License"), _T("Offline activation export present but no token provided"));
        return;
    }

    int rc = pActivateLicenseOffline(token);
    if (rc == 0) {
        std::wstring tail = MaskActivationToken(token);
        TCHAR buf[160];
        _stprintf(buf, _T("Offline activation succeeded (token tail %s)"), tail.c_str());
        LOG_INFO(_T("BarcodeReader"), _T("License"), buf);
    }
    else {
        TCHAR buf[160];
        _stprintf(buf, _T("Offline activation failed rc=%d (continuing)"), rc);
        LOG_WARNING(_T("BarcodeReader"), _T("License"), buf);
    }
}

// Diagnostic license info (after create)
static void ValidateUnlimitedLicense()
{
    if (g_LicenseChecked) return;
    g_LicenseChecked = true;

    if (!pGetLicenseInfo) {
        LOG_INFO(_T("BarcodeReader"), _T("License"), _T("License info API not exported (assuming unlimited)"));
        return;
    }

    char licKey[128] = { 0 };
    char comments[128] = { 0 };
    int licenseType = 0;
    time_t exp = 0;
    char dongle[64] = { 0 };
    int valid = 0;

    pGetLicenseInfo(licKey, (int)sizeof(licKey),
        comments, (int)sizeof(comments),
        &licenseType,
        &exp,
        dongle, (int)sizeof(dongle),
        &valid);

    if (valid) {
        LOG_INFO(_T("BarcodeReader"), _T("License"), _T("LicenseInfo: valid=1"));
    }
    else {
        LOG_WARNING(_T("BarcodeReader"), _T("License"),
            _T("LicenseInfo: valid=0 (if decode succeeds and no 7201/7202 errors appear, this flag may be non-critical)"));
    }
}

// ===== BarcodeReader implementation =====
BarcodeReader::BarcodeReader()
{
    m_BcOrientation = 0;
    m_BarcodesChanged = FALSE;
    m_pszReaderLocation = nullptr;
    m_readBarcode = nullptr;
    m_readType = BT_Unknown;
    m_nextBufferItem = 0;
    m_barcodeBufferLength = 0;
    m_dwLastReadTime = 0;
    m_usBarcodeReadingInterval1 = 0;
    m_usBarcodeReadingInterval2 = 0;
    for (int i = 0; i < 3; i++) {
        m_enableScanRect[i] = false;
        m_scanRectLeft[i] = m_scanRectTop[i] = m_scanRectRight[i] = m_scanRectBottom[i] = 0;
    }
    for (int i = 0; i < BARCODE_BUFFER_MAX_LENGTH; i++)
        m_barcodeBuffer[i] = nullptr;
    m_minimumBarcodeHeight = 0;
    m_inStandby = false;
    m_hDTK5Lib = nullptr;
    m_pfnCreate5 = nullptr;
    m_pfnDestroy5 = nullptr;
    m_hBarReader5 = nullptr;
}

BarcodeReader::~BarcodeReader()
{
    Release();
}

bool BarcodeReader::LoadDTK5(const PTCHAR dllPath)
{
    InitLogger();
#ifdef UNICODE
    m_hDTK5Lib = LoadLibraryW(dllPath);
#else
    m_hDTK5Lib = LoadLibraryA(dllPath);
#endif
    if (!m_hDTK5Lib) {
        LOG_ERROR(CLASSNAME, _T("LoadDTK5"), _T("LoadLibrary failed"));
        return false;
    }

    m_pfnCreate5 = reinterpret_cast<PFN_BR5_CREATE>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_Create"));
    m_pfnDestroy5 = reinterpret_cast<PFN_BR5_DESTROY>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_Destroy"));
    if (!m_pfnCreate5 || !m_pfnDestroy5) {
        LOG_ERROR(CLASSNAME, _T("LoadDTK5"), _T("Create/Destroy exports missing"));
        UnloadDTK5();
        return false;
    }

    // Core decode exports
    pReadFromImageBuffer = reinterpret_cast<PFN_ReadFromImageBuffer>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_ReadFromImageBuffer"));
    pSetRectX = reinterpret_cast<PFN_set_RectX>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_ScanRectangleX"));
    pSetRectY = reinterpret_cast<PFN_set_RectY>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_ScanRectangleY"));
    pSetRectW = reinterpret_cast<PFN_set_RectW>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_ScanRectangleWidth"));
    pSetRectH = reinterpret_cast<PFN_set_RectH>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_ScanRectangleHeight"));
    pSetBarcodeTypes = reinterpret_cast<PFN_set_BarcodeTypes>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_BarcodeTypes"));
    pSetOrientation = reinterpret_cast<PFN_set_Orientation>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_Orientation"));
    pGetResultError = reinterpret_cast<PFN_get_Error>(GetProcAddress(m_hDTK5Lib, "BarReaderResult_get_ErrorCode"));
    pGetBarcodesCount = reinterpret_cast<PFN_get_Count>(GetProcAddress(m_hDTK5Lib, "BarReaderResult_get_BarcodesCount"));
    pGetBarcode = reinterpret_cast<PFN_get_Barcode>(GetProcAddress(m_hDTK5Lib, "BarReaderResult_get_Barcode"));
    pGetBarcodeText = reinterpret_cast<PFN_get_Text>(GetProcAddress(m_hDTK5Lib, "Barcode_get_Text"));
    pGetBarcodeType = reinterpret_cast<PFN_get_Type>(GetProcAddress(m_hDTK5Lib, "Barcode_get_Type"));
    pDestroyBarcode = reinterpret_cast<PFN_destroy_Barcode>(GetProcAddress(m_hDTK5Lib, "Barcode_Destroy"));
    pDestroyResult = reinterpret_cast<PFN_destroy_Result>(GetProcAddress(m_hDTK5Lib, "BarReaderResult_Destroy"));
    pGetLicenseInfo = reinterpret_cast<PFN_GetLicenseInfo>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_GetLicenseInfo"));

    // License-related optional exports
    pSetLicenseKeyGlobal = reinterpret_cast<PFN_SetLicenseKey_Global>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_SetLicenseKey"));
    if (!pSetLicenseKeyGlobal)
        pSetLicenseKeyGlobal = reinterpret_cast<PFN_SetLicenseKey_Global>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_SetLicense"));

    pSetLicenseKeyHandle = reinterpret_cast<PFN_SetLicenseKey_Handle>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_LicenseKey"));
    if (!pSetLicenseKeyHandle)
        pSetLicenseKeyHandle = reinterpret_cast<PFN_SetLicenseKey_Handle>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_set_License"));

    pActivateLicenseOffline = reinterpret_cast<PFN_ActivateLicenseOffline>(GetProcAddress(m_hDTK5Lib, "BarcodeReader_ActivateLicenseOffline"));

    if (!pReadFromImageBuffer || !pSetRectX || !pSetRectY || !pSetRectW || !pSetRectH ||
        !pSetBarcodeTypes || !pSetOrientation || !pGetResultError ||
        !pGetBarcodesCount || !pGetBarcode || !pGetBarcodeText ||
        !pGetBarcodeType || !pDestroyBarcode || !pDestroyResult)
    {
        LOG_ERROR(CLASSNAME, _T("LoadDTK5"), _T("Required exports missing"));
        UnloadDTK5();
        return false;
    }

    LOG_INFO(CLASSNAME, _T("LoadDTK5"), _T("Library loaded"));

    // Perform offline activation BEFORE any reader handle creation (if export + token)
    TryOfflineActivation();

    // Apply global key (harmless if not required)
    ApplyLicenseKeyIfPossible(nullptr);

    // Defer ValidateUnlimitedLicense() until after handle creation
    return true;
}

void BarcodeReader::UnloadDTK5()
{
    if (m_hBarReader5 && m_pfnDestroy5)
        m_pfnDestroy5(m_hBarReader5);
    m_hBarReader5 = nullptr;
    m_pfnCreate5 = nullptr;
    m_pfnDestroy5 = nullptr;

    if (m_hDTK5Lib) {
        FreeLibrary(m_hDTK5Lib);
        m_hDTK5Lib = nullptr;
    }
}

void BarcodeReader::Initialize(PBARCODEREADERSETTINGS pSettings)
{
    InitLogger();

    if (!pSettings)
        ExceptionInfo::Throw(CLASSNAME, _T("Initialize"), ERR_BARCODEREADER_SETTINGS_IS_NULL);
    if (!pSettings->pszReaderBinaryLocation || _tcslen(pSettings->pszReaderBinaryLocation) == 0)
        ExceptionInfo::Throw(CLASSNAME, _T("Initialize"), ERR_BARCODEREADER_READER_LOCATION_IS_MISSING);

    CopyString(pSettings->pszReaderBinaryLocation, m_pszReaderLocation);
    m_usBarcodeReadingInterval1 = pSettings->usBarcodeReadingInterval1;
    m_usBarcodeReadingInterval2 = pSettings->usBarcodeReadingInterval2;
    m_BcOrientation = pSettings->BcOrientation;

    m_enableScanRect[0] = !!pSettings->enableScanRect1;
    m_scanRectLeft[0] = pSettings->scanRect1Left;
    m_scanRectTop[0] = pSettings->scanRect1Top;
    m_scanRectRight[0] = pSettings->scanRect1Right;
    m_scanRectBottom[0] = pSettings->scanRect1Bottom;

    m_enableScanRect[1] = !!pSettings->enableScanRect2;
    m_scanRectLeft[1] = pSettings->scanRect2Left;
    m_scanRectTop[1] = pSettings->scanRect2Top;
    m_scanRectRight[1] = pSettings->scanRect2Right;
    m_scanRectBottom[1] = pSettings->scanRect2Bottom;

    m_enableScanRect[2] = !!pSettings->enableScanRect3;
    m_scanRectLeft[2] = pSettings->scanRect3Left;
    m_scanRectTop[2] = pSettings->scanRect3Top;
    m_scanRectRight[2] = pSettings->scanRect3Right;
    m_scanRectBottom[2] = pSettings->scanRect3Bottom;

    m_barcodeBufferLength = pSettings->barcodeRedundancy;
    if (m_barcodeBufferLength > BARCODE_BUFFER_MAX_LENGTH)
        m_barcodeBufferLength = BARCODE_BUFFER_MAX_LENGTH;
    ClearBarcodeBuffer();
    m_minimumBarcodeHeight = pSettings->minimumBarcodeHeight;

    if (!LoadDTK5(m_pszReaderLocation))
        ExceptionInfo::Throw(CLASSNAME, _T("Initialize"), ERR_BARCODEREADER_FAILED_TO_LOAD_READER);

    m_hBarReader5 = m_pfnCreate5 ? m_pfnCreate5(nullptr) : nullptr;
    if (!m_hBarReader5) {
        UnloadDTK5();
        ExceptionInfo::Throw(CLASSNAME, _T("Initialize"), ERR_BARCODEREADER_NOT_INITIALIZED);
    }

    // Apply handle-based key if available
    ApplyLicenseKeyIfPossible(m_hBarReader5);

    // Validate now (after activation + creation)
    ValidateUnlimitedLicense();

    // Enable 1D types only (adjust if you need 2D)
    unsigned int types = 0;
    if (pSettings->enableCode128)         types |= BT_Code128;
    if (pSettings->enableInterleaved2of5) types |= BT_Inter2of5;
    if (pSettings->enableCode39)          types |= BT_Code39;
    // Keep DataMatrix conditionally (remove if strictly 1D)
    if (pSettings->enableDataMatrix)      types |= BT_DataMatrix;

    pSetBarcodeTypes(m_hBarReader5, (BarcodeTypeEnum)types);

    int mask = 0;
    if (m_BcOrientation & BCR_RD_LEFT_TO_RIGHT)  mask |= BO_LeftToRight;
    if (m_BcOrientation & BCR_RD_RIGHT_TO_LEFT)  mask |= BO_RightToLeft;
    if (m_BcOrientation & BCR_RD_TOP_TO_BOTTOM)  mask |= BO_TopToBottom;
    if (m_BcOrientation & BCR_RD_BOTTOM_TO_TOP)  mask |= BO_BottomToTop;
    pSetOrientation(m_hBarReader5, (BarcodeOrientationEnum)mask);

    LOG_INFO(CLASSNAME, _T("Initialize"), _T("Initialized"));
}

void BarcodeReader::GetCurrentSettings(PBARCODEREADERSETTINGS pSettings)
{
    if (!pSettings) return;
    pSettings->maxBarcodeCount = 1;
    pSettings->usBarcodeReadingInterval1 = m_usBarcodeReadingInterval1;
    pSettings->usBarcodeReadingInterval2 = m_usBarcodeReadingInterval2;
    pSettings->BcOrientation = m_BcOrientation;

    pSettings->enableScanRect1 = m_enableScanRect[0] ? 1 : 0;
    pSettings->scanRect1Left = m_scanRectLeft[0];
    pSettings->scanRect1Top = m_scanRectTop[0];
    pSettings->scanRect1Right = m_scanRectRight[0];
    pSettings->scanRect1Bottom = m_scanRectBottom[0];

    pSettings->enableScanRect2 = m_enableScanRect[1] ? 1 : 0;
    pSettings->scanRect2Left = m_scanRectLeft[1];
    pSettings->scanRect2Top = m_scanRectTop[1];
    pSettings->scanRect2Right = m_scanRectRight[1];
    pSettings->scanRect2Bottom = m_scanRectBottom[1];

    pSettings->enableScanRect3 = m_enableScanRect[2] ? 1 : 0;
    pSettings->scanRect3Left = m_scanRectLeft[2];
    pSettings->scanRect3Top = m_scanRectTop[2];
    pSettings->scanRect3Right = m_scanRectRight[2];
    pSettings->scanRect3Bottom = m_scanRectBottom[2];

    pSettings->minimumBarcodeHeight = m_minimumBarcodeHeight;
    pSettings->barcodeRedundancy = m_barcodeBufferLength;

    if (pSettings->pszReaderBinaryLocation && m_pszReaderLocation)
        CopyString(m_pszReaderLocation, pSettings->pszReaderBinaryLocation);
}

void BarcodeReader::UpdateSettings(PBARCODEREADERSETTINGS pSettings)
{
    if (!pSettings) return;

    // Update timing intervals
        m_usBarcodeReadingInterval1 = pSettings->usBarcodeReadingInterval1;
    m_usBarcodeReadingInterval2 = pSettings->usBarcodeReadingInterval2;
    
    // Update orientation settings
    m_BcOrientation = pSettings->BcOrientation;

    // Update scan rectangle settings
    m_enableScanRect[0] = !!pSettings->enableScanRect1;
    m_scanRectLeft[0] = pSettings->scanRect1Left;
    m_scanRectTop[0] = pSettings->scanRect1Top;
    m_scanRectRight[0] = pSettings->scanRect1Right;
    m_scanRectBottom[0] = pSettings->scanRect1Bottom;

    m_enableScanRect[1] = !!pSettings->enableScanRect2;
    m_scanRectLeft[1] = pSettings->scanRect2Left;
    m_scanRectTop[1] = pSettings->scanRect2Top;
    m_scanRectRight[1] = pSettings->scanRect2Right;
    m_scanRectBottom[1] = pSettings->scanRect2Bottom;

    m_enableScanRect[2] = !!pSettings->enableScanRect3;
    m_scanRectLeft[2] = pSettings->scanRect3Left;
    m_scanRectTop[2] = pSettings->scanRect3Top;
    m_scanRectRight[2] = pSettings->scanRect3Right;
    m_scanRectBottom[2] = pSettings->scanRect3Bottom;

    // Update other parameters
    m_minimumBarcodeHeight = pSettings->minimumBarcodeHeight;
    m_barcodeBufferLength = pSettings->barcodeRedundancy;
    if (m_barcodeBufferLength > BARCODE_BUFFER_MAX_LENGTH)
        m_barcodeBufferLength = BARCODE_BUFFER_MAX_LENGTH;

    // Only update engine settings if we have a valid reader handle
    if (m_hBarReader5) {
        // Re-apply barcode types (matching Initialize behavior)
        unsigned int types = 0;
        if (pSettings->enableCode128)         types |= BT_Code128;
        if (pSettings->enableInterleaved2of5) types |= BT_Inter2of5;
        if (pSettings->enableCode39)          types |= BT_Code39;
        if (pSettings->enableDataMatrix)      types |= BT_DataMatrix;
        pSetBarcodeTypes(m_hBarReader5, (BarcodeTypeEnum)types);

        // Re-apply orientation mask
        int mask = 0;
        if (m_BcOrientation & BCR_RD_LEFT_TO_RIGHT)  mask |= BO_LeftToRight;
        if (m_BcOrientation & BCR_RD_RIGHT_TO_LEFT)  mask |= BO_RightToLeft;
        if (m_BcOrientation & BCR_RD_TOP_TO_BOTTOM)  mask |= BO_TopToBottom;
        if (m_BcOrientation & BCR_RD_BOTTOM_TO_TOP)  mask |= BO_BottomToTop;
        pSetOrientation(m_hBarReader5, (BarcodeOrientationEnum)mask);

        LOG_INFO(CLASSNAME, _T("UpdateSettings"), _T("Engine settings reapplied"));
    }
}

// Extract bitmap helper
static bool ExtractBitmap(HBITMAP hBitmap, std::vector<BYTE>& buf, int& w, int& h, int& stride, PixelFormatEnum& fmt)
{
    BITMAP bm{};
    if (!GetObject(hBitmap, sizeof(bm), &bm)) return false;
    w = bm.bmWidth; h = bm.bmHeight;

    BITMAPINFO bi{};
    bi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bi.bmiHeader.biWidth = w;
    bi.bmiHeader.biHeight = -h;
    bi.bmiHeader.biPlanes = 1;
    bi.bmiHeader.biBitCount = 32;
    bi.bmiHeader.biCompression = BI_RGB;

    stride = ((w * 32 + 31) / 32) * 4;
    buf.resize(stride * h);

    HDC hdc = GetDC(nullptr);
    int r = GetDIBits(hdc, hBitmap, 0, h, buf.data(), &bi, DIB_RGB_COLORS);
    ReleaseDC(nullptr, hdc);
    if (r == 0) return false;
    fmt = PIXFMT_BGRA;
    return true;
}

void BarcodeReader::Read(HBITMAP hBitmap, DWORD dwCurrentTime)
{
    if (!m_hBarReader5)
        return;

    m_BarcodesChanged = FALSE;
    m_dwLastReadTime = dwCurrentTime;

    std::vector<BYTE> pixels;
    int width = 0, height = 0, stride = 0;
    PixelFormatEnum pixFmt = PIXFMT_NONE;
    if (!ExtractBitmap(hBitmap, pixels, width, height, stride, pixFmt)) {
        LOG_WARNING(CLASSNAME, _T("Read"), _T("ExtractBitmap failed"));
        return;
    }

    if (!m_enableScanRect[0] && !m_enableScanRect[1] && !m_enableScanRect[2]) {
        LOG_INFO(CLASSNAME, _T("Read"), _T("No scan rectangles enabled - skipping"));
        return;
    }

    BARREADERRESULT res = nullptr;
    int found = 0;
    int usedRect = -1;

    for (int i = 0; i < 3; ++i) {
        if (!m_enableScanRect[i]) continue;

        int left = m_scanRectLeft[i];
        int top = m_scanRectTop[i];
        int w = m_scanRectRight[i] - left;
        int h = m_scanRectBottom[i] - top;
        if (w <= 0 || h <= 0) {
            TCHAR msg[128];
            _stprintf(msg, _T("Rect[%d] invalid dims (%d x %d)"), i, w, h);
            LOG_WARNING(CLASSNAME, _T("Read"), msg);
            continue;
        }

        pSetRectX(m_hBarReader5, left);
        pSetRectY(m_hBarReader5, top);
        pSetRectW(m_hBarReader5, w);
        pSetRectH(m_hBarReader5, h);

        res = pReadFromImageBuffer(m_hBarReader5, pixels.data(), width, height, stride, pixFmt);
        if (!res) {
            LOG_WARNING(CLASSNAME, _T("Read"), _T("Null result handle"));
            continue;
        }
        int ec = pGetResultError(res);
        if (ec != 0) {
            const TCHAR* desc = DescribeEngineError(ec);
            TCHAR msg[160];
            _stprintf(msg, _T("Rect[%d] engine error=%d (%s)"), i, ec, desc);
            LOG_WARNING(CLASSNAME, _T("Read"), msg);
            pDestroyResult(res);
            res = nullptr;
            continue;
        }

        found = pGetBarcodesCount(res);
        if (found > 0) {
            usedRect = i;
            TCHAR msg[96];
            _stprintf(msg, _T("Rect[%d] found=%d"), i, found);
            LOG_INFO(CLASSNAME, _T("Read"), msg);
            break;
        }

        pDestroyResult(res);
        res = nullptr;
    }

    pSetRectX(m_hBarReader5, 0);
    pSetRectY(m_hBarReader5, 0);
    pSetRectW(m_hBarReader5, 0);
    pSetRectH(m_hBarReader5, 0);

    if (found <= 0 || !res) {
        return;
    }

    BARCODE bc = pGetBarcode(res, 0);
    if (!bc) {
        LOG_WARNING(CLASSNAME, _T("Read"), _T("Null barcode handle"));
        pDestroyResult(res);
        return;
    }

    int need = pGetBarcodeText(bc, nullptr, 0);
    if (need <= 0) {
        pDestroyBarcode(bc);
        pDestroyResult(res);
        return;
    }

    std::string utf8; utf8.resize(need);
    pGetBarcodeText(bc, &utf8[0], need);

#ifdef UNICODE
    int wlen = MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), nullptr, 0);
    std::wstring wtext; wtext.resize(wlen);
    MultiByteToWideChar(CP_UTF8, 0, utf8.c_str(), (int)utf8.size(), &wtext[0], wlen);
    PTCHAR dup = _wcsdup(wtext.c_str());
    const PTCHAR candidateForLog = dup;
#else
    PTCHAR dup = _strdup(utf8.c_str());
    const PTCHAR candidateForLog = dup;
#endif

    SaveToBuffer(dup);

    if (CheckRedundancyOk()) {
        BarcodeTypeEnum t = pGetBarcodeType(bc);
        bool change = false;
        if (!m_readBarcode || _tcscmp(m_readBarcode, dup) != 0) {
            if (t != BT_Inter2of5 || m_readType == BT_Inter2of5) {
                if (t != BT_Inter2of5 || !m_readBarcode || _tcslen(m_readBarcode) < _tcslen(dup)) {
                    m_readBarcode = dup;
                    m_readType = t;
                    m_BarcodesChanged = TRUE;
                    change = true;
                }
            }
        }
        if (change) {
            TCHAR msg[256];
            _stprintf(msg, _T("ACCEPTED '%s' (type=%s)"), candidateForLog, TypeToString((BarcodeTypeEnum)m_readType));
            LOG_INFO(CLASSNAME, _T("Read"), msg);
        }
    }

    pDestroyBarcode(bc);
    pDestroyResult(res);
}

PTCHAR BarcodeReader::GetBarcodeType()
{
    switch (m_readType) {
    case BT_Code39: return (PTCHAR)_T("Code39");
    case BT_Inter2of5: return (PTCHAR)_T("Interleaved2of5");
    case BT_UPCA: return (PTCHAR)_T("UPCA");
    case BT_UPCE: return (PTCHAR)_T("UPCE");
    case BT_EAN8:
    case BT_EAN13: return (PTCHAR)_T("EAN");
    case BT_Codabar: return (PTCHAR)_T("Codabar");
    case BT_Code128: return (PTCHAR)_T("Code128");
    case BT_Code93: return (PTCHAR)_T("Code93");
    case BT_Code11: return (PTCHAR)_T("Code11");
    default: return (PTCHAR)_T("Unknown");
    }
}

// Add these helpers to BarcodeReader class (protected section in .h, implementation here):

void BarcodeReader::FreeString(PTCHAR& s)
{
    if (s) {
        free(s);
        s = nullptr;
    }
}

void BarcodeReader::FreeAllBarcodeStrings()
{
    FreeString(m_readBarcode);
    for (int i = 0; i < BARCODE_BUFFER_MAX_LENGTH; ++i) {
        FreeString(m_barcodeBuffer[i]);
    }
    m_nextBufferItem = 0;
}

void BarcodeReader::SaveToBuffer(PTCHAR s)
{
    if (m_barcodeBufferLength == 0) {
        FreeString(s);
        return;
    }
    FreeString(m_barcodeBuffer[m_nextBufferItem]);
    m_barcodeBuffer[m_nextBufferItem] = s;
    m_nextBufferItem = (m_nextBufferItem + 1) % m_barcodeBufferLength;
}

void BarcodeReader::ClearBarcodeBuffer()
{
    for (int i = 0; i < BARCODE_BUFFER_MAX_LENGTH; i++) {
        FreeString(m_barcodeBuffer[i]);
    }
    m_nextBufferItem = 0;
}

void BarcodeReader::ClearBarcodes()
{
    FreeString(m_readBarcode);
    ClearBarcodeBuffer();
}

void BarcodeReader::Release()
{
    ReleaseString(m_pszReaderLocation);
    FreeAllBarcodeStrings();
    UnloadDTK5();
}

BYTE BarcodeReader::IsReadRequired(DWORD dwCurrentTime, int searchingPunches)
{
    if (!m_hBarReader5)
        return FALSE;

    USHORT interval = searchingPunches ? m_usBarcodeReadingInterval2 : m_usBarcodeReadingInterval1;
    return (dwCurrentTime - m_dwLastReadTime) >= interval;
}

bool BarcodeReader::CheckRedundancyOk()
{
    if (m_barcodeBufferLength == 0)
        return true;

    // Count occurrences of the latest barcode in the buffer
    int count = 0;
    PTCHAR latest = m_barcodeBuffer[(m_nextBufferItem + m_barcodeBufferLength - 1) % m_barcodeBufferLength];
    
    if (!latest)
        return false;

    for (int i = 0; i < m_barcodeBufferLength; i++) {
        if (m_barcodeBuffer[i] && _tcscmp(m_barcodeBuffer[i], latest) == 0)
            count++;
    }

    // Need majority of buffer slots to contain the same barcode
    return count > (m_barcodeBufferLength / 2);
}

void BarcodeReader::EnterStandby()
{
    if (!m_inStandby) {
        m_inStandby = true;
        ClearBarcodes();
    }
}

void BarcodeReader::LeaveStandby()
{
    if (m_inStandby) {
        m_inStandby = false;
        ClearBarcodes();
    }
}