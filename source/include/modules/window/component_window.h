#pragma once

#include <dwmapi.h>

// Window operator base
MIDL_INTERFACE("49B220FA-E6C2-4efe-B65B-06AF48505A58")
IWindowOpBase : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetHWND(HWND hWnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND *pWnd) = 0;
};

// Basic window operator
MIDL_INTERFACE("04937D3D-3A3B-478a-A9A3-B982BA840079")
IWindowBasicOp : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetProcessId(DWORD *pPid) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetThreadId(DWORD *pTid) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetClassName(BSTR *ppClassName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetWindowText(BSTR *ppText) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetWindowText(BSTR pText) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetParent(HWND *pWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetParent(HWND hWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetOwner(HWND *pWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetThreadInfo(GUITHREADINFO *pInfo) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFocus() = 0;
    virtual HRESULT STDMETHODCALLTYPE SetActive() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetForegroundWindow(HWND *pWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetForegroundWindow() = 0;
    virtual HRESULT STDMETHODCALLTYPE SetWindowPos(HWND hInsertAfter, RECT *pRect, DWORD nFlag) = 0;
};

MIDL_INTERFACE("1CD667E7-205D-45fc-AEB4-328B0C4A1AA5")
IWindowPropEnumSink : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnProp(BSTR pKey, HANDLE hValue, BOOL bIsAtom) = 0;
};

// Window prop operator
MIDL_INTERFACE("21BD2E4A-7423-4b60-A9B7-B7CABCC63C61")
IWindowPropOp : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetProp(BSTR pKey, HANDLE hValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProp(BSTR pKey, HANDLE *pValue, BOOL *pIsAtom) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAllProps(IWindowPropEnumSink *pSink) = 0;
};

// Dwm operator
MIDL_INTERFACE("C8A00C6E-F565-43b4-8D5D-BD60D62DCA24")
IWindowDwmOp : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE IsDwmCompositionEnabled(BOOL *pEnabled) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAttribute(DWORD nId, PVOID pValue, DWORD nValueSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetAttribute(DWORD nId, PVOID pValue, DWORD nValueSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE ExtendFrameIntoChildArea(MARGINS *pMargins) = 0;
};