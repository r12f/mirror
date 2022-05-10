#include "stdafx.h"
#include "modules/window/utils/dwmapi_adapter.h"

typedef HRESULT (WINAPI *DwmIsCompositionEnabledPtr)(BOOL* pfEnabled);
typedef HRESULT (WINAPI *DwmEnableCompositionPtr)(UINT uCompositionAction);
typedef HRESULT (WINAPI *DwmGetWindowAttributePtr)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
typedef HRESULT (WINAPI *DwmSetWindowAttributePtr)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute );
typedef HRESULT (WINAPI *DwmExtendFrameIntoClientAreaPtr)(HWND hWnd, const MARGINS* pMarInset);
typedef HRESULT (WINAPI *DwmGetTransportAttributesPtr)(BOOL *pfIsRemoting, BOOL *pfIsConnected, DWORD *pDwGeneration);
typedef HRESULT (WINAPI *DwmGetColorizationColorPtr)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
typedef HRESULT (WINAPI *DwmInvalidateIconicBitmapsPtr)(HWND hwnd);
typedef HRESULT (WINAPI *DwmGetCompositionTimingInfoPtr)(HWND hwnd, DWM_TIMING_INFO *pTimingInfo);

static HMODULE m_hDwmDll = NULL;
static DwmIsCompositionEnabledPtr s_pFuncDwmIsCompositionEnabled;
static DwmEnableCompositionPtr s_pFuncDwmEnableComposition;
static DwmGetWindowAttributePtr s_pFuncDwmGetWindowAttribute;
static DwmSetWindowAttributePtr s_pFuncDwmSetWindowAttribute;
static DwmExtendFrameIntoClientAreaPtr s_pFuncDwmExtendFrameIntoClientArea;
static DwmGetTransportAttributesPtr s_pFuncDwmGetTransportAttributes;
static DwmGetColorizationColorPtr s_pFuncDwmColorizationColor;
static DwmInvalidateIconicBitmapsPtr s_pFuncDwmInvalidateIconicBitmaps;
static DwmGetCompositionTimingInfoPtr s_pFuncDwmGetCompositionTimingInfo;

struct DwmDllAutoLoad {
    DwmDllAutoLoad() {
        m_hDwmDll = ::LoadLibrary(_T("dwmapi.dll"));
        if(NULL != m_hDwmDll) {
            s_pFuncDwmIsCompositionEnabled = (DwmIsCompositionEnabledPtr)::GetProcAddress(m_hDwmDll, "DwmIsCompositionEnabled");
            s_pFuncDwmEnableComposition = (DwmEnableCompositionPtr)::GetProcAddress(m_hDwmDll, "DwmEnableComposition");
            s_pFuncDwmGetWindowAttribute = (DwmGetWindowAttributePtr)::GetProcAddress(m_hDwmDll, "DwmGetWindowAttribute");
            s_pFuncDwmSetWindowAttribute = (DwmSetWindowAttributePtr)::GetProcAddress(m_hDwmDll, "DwmSetWindowAttribute");
            s_pFuncDwmExtendFrameIntoClientArea = (DwmExtendFrameIntoClientAreaPtr)::GetProcAddress(m_hDwmDll, "DwmExtendFrameIntoClientArea");
            s_pFuncDwmGetTransportAttributes = (DwmGetTransportAttributesPtr)::GetProcAddress(m_hDwmDll, "DwmGetTransportAttributes");
            s_pFuncDwmColorizationColor = (DwmGetColorizationColorPtr)::GetProcAddress(m_hDwmDll, "DwmGetColorizationColor");
            s_pFuncDwmInvalidateIconicBitmaps = (DwmInvalidateIconicBitmapsPtr)::GetProcAddress(m_hDwmDll, "DwmInvalidateIconicBitmaps");
            s_pFuncDwmGetCompositionTimingInfo = (DwmGetCompositionTimingInfoPtr)::GetProcAddress(m_hDwmDll, "DwmGetCompositionTimingInfo");
        }
    }

    ~DwmDllAutoLoad() {
        if(NULL != m_hDwmDll) {
            ::FreeLibrary(m_hDwmDll);
        }
    }
};

static DwmDllAutoLoad s_oDwmDll;

HRESULT
AdaDwmIsCompositionEnabled(BOOL* pfEnabled)
{
    if(NULL == s_pFuncDwmIsCompositionEnabled) {
        return E_FAIL;
    }

    return s_pFuncDwmIsCompositionEnabled(pfEnabled);
}

HRESULT
AdaDwmEnableComposition(UINT uCompositionAction)
{
    if(NULL == s_pFuncDwmEnableComposition) {
        return E_FAIL;
    }

    return s_pFuncDwmEnableComposition(uCompositionAction);
}

HRESULT
AdaDwmGetWindowAttribute(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute)
{
    if(NULL == s_pFuncDwmGetWindowAttribute) {
        return E_FAIL;
    }

    return s_pFuncDwmGetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT
AdaDwmSetWindowAttribute(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute)
{
    if(NULL == s_pFuncDwmSetWindowAttribute) {
        return E_FAIL;
    }

    return s_pFuncDwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT
AdaDwmExtendFrameIntoClientArea(HWND hWnd, const MARGINS* pMarInset)
{
    if(NULL == s_pFuncDwmExtendFrameIntoClientArea) {
        return E_FAIL;
    }

    return s_pFuncDwmExtendFrameIntoClientArea(hWnd, pMarInset);
}

HRESULT
AdaDwmGetTransportAttributes(BOOL *pfIsRemoting, BOOL *pfIsConnected, DWORD *pDwGeneration)
{
    if(NULL == s_pFuncDwmExtendFrameIntoClientArea) {
        return E_FAIL;
    }

    return s_pFuncDwmGetTransportAttributes(pfIsRemoting, pfIsConnected, pDwGeneration);
}

HRESULT
AdaDwmGetColorizationColor(DWORD *pcrColorization, BOOL *pfOpaqueBlend)
{
    if(NULL == s_pFuncDwmColorizationColor) {
        return E_FAIL;
    }

    return s_pFuncDwmColorizationColor(pcrColorization, pfOpaqueBlend);
}

HRESULT
AdaDwmInvalidateIconicBitmaps(HWND hwnd)
{
    if(NULL == s_pFuncDwmInvalidateIconicBitmaps) {
        return E_FAIL;
    }

    return s_pFuncDwmInvalidateIconicBitmaps(hwnd);
}

HRESULT
AdaDwmGetCompositionTimingInfo(HWND hwnd, DWM_TIMING_INFO *pTimingInfo)
{
    if(NULL == s_pFuncDwmGetCompositionTimingInfo) {
        return E_FAIL;
    }

    return s_pFuncDwmGetCompositionTimingInfo(hwnd, pTimingInfo);
}