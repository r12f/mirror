#pragma once

#include "modules/window/utils/snapshot/window_filter.h"

enum {
    WINDOW_SNAPSHOT_METHOD_GET_WINDOW               = 0,
    WINDOW_SNAPSHOT_METHOD_ENUM_WINDOW,
};

enum {
    WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED       = 0,
    WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE,
    WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE,
};

MIDL_INTERFACE("B4518359-2102-4fa4-8A68-036A7F8E32FD")
IWindowSnapshot : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE MakeSnapshot(HWND hWindow, BOOL bRecursive, DWORD nMethod) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND *pWnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetClassName(BSTR *pClassName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTitle(BSTR *pClassName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetVisible(BOOL *pVisible) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPid(DWORD *pPid) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetTid(DWORD *pTid) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChildrenCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChildren(IWindowSnapshot **pChildren, DWORD nMaxCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPrivateData(PVOID pData) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPrivateData(PVOID *ppData) = 0;
    virtual HRESULT STDMETHODCALLTYPE SaveToFile(BSTR strFile, DWORD nFileType) = 0;
    virtual HRESULT STDMETHODCALLTYPE RunFilter(IWindowFilter **pFilters, DWORD nCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLastFilterStatus(DWORD *pTestResult) = 0;
};

CComPtr<IWindowSnapshot> CreateWindowSnapshot();