#pragma once

MIDL_INTERFACE("78E662F4-B0B5-43ab-85BB-7CB0B013C176")
IMirModule : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Create() = 0;
    virtual HRESULT STDMETHODCALLTYPE Destroy() = 0;

    virtual HRESULT STDMETHODCALLTYPE Init() = 0;
    virtual HRESULT STDMETHODCALLTYPE Uninit() = 0;

    virtual HRESULT STDMETHODCALLTYPE RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI) = 0;

    virtual HRESULT STDMETHODCALLTYPE CreateInstance(REFGUID pGUID, void **pObject) = 0;
};