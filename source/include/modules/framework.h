#pragma once

#include "modules/mir_module.h"

MIDL_INTERFACE("866445D1-B97A-4787-AEE2-7045506A5A1C")
IFramework : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE RegisterModule(BSTR pModuleName, IMirModule *pModule) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterModule(BSTR pModuleName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModule(BSTR pModuleName, IMirModule **ppModule) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModulesCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModules(IMirModule **pModule, DWORD nMaxCount) = 0;

    virtual HRESULT STDMETHODCALLTYPE RegisterComponent(BSTR pName, IUnknown *pComponent) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterComponent(BSTR pName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComponent(BSTR pName, IUnknown **ppComponent) = 0;
};

MIDL_INTERFACE("760D632E-EA4F-4235-B92E-C89E6BB527C8")
IFrameworkControl : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Init() = 0;
    virtual HRESULT STDMETHODCALLTYPE Uninit() = 0;

    virtual HRESULT STDMETHODCALLTYPE InitModules() = 0;
    virtual HRESULT STDMETHODCALLTYPE UninitModules() = 0;
};

IFramework * GetFramework();

HRESULT MirrorCreateObject(LPCTSTR pModule, REFGUID pGUID, void **pObject);

template <class IInterface>
CComPtr<IInterface> GetComponentInterface(LPCTSTR pName)
{
    CComPtr<IUnknown> pComponent;
    if(FAILED(GetFramework()->GetComponent(CComBSTR(pName), &pComponent)) || NULL == pComponent) {
        return NULL;
    }

    CComQIPtr<IInterface> pInterface = pComponent;

    return pInterface;
}