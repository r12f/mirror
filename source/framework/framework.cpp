#include "stdafx.h"
#include "modules/framework.h"
#include "framework/framework.h"

static Framework s_oFramework;

IFramework *
GetFramework()
{
    return &s_oFramework;
}

Framework::Framework()
{

}

Framework::~Framework()
{

}

STDMETHODIMP
Framework::Init()
{
    return S_OK;
}

STDMETHODIMP
Framework::Uninit()
{
    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        oModuleIt->m_pModule->Destroy();
        ++oModuleIt;
    }

    m_vModules.clear();

    return S_OK;
}

STDMETHODIMP
Framework::InitModules()
{
    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        oModuleIt->m_pModule->Init();
        ++oModuleIt;
    }

    return S_OK;
}

STDMETHODIMP
Framework::UninitModules()
{
    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        oModuleIt->m_pModule->Uninit();
        ++oModuleIt;
    }

    return S_OK;
}

STDMETHODIMP
Framework::RegisterModule(BSTR pModuleName, IMirModule *pModule)
{
    ASSERT_RETURN(NULL != pModuleName, E_INVALIDARG);
    ASSERT_RETURN(NULL != pModule, E_POINTER);

    ModuleInfo oInfo;
    oInfo.m_strModuleName = pModuleName;
    oInfo.m_pModule = pModule;
    oInfo.m_pModule->AddRef();
    oInfo.m_pModule->Create();

    m_vModules.push_back(oInfo);

    return S_OK;
}

STDMETHODIMP
Framework::UnregisterModule(BSTR pModuleName)
{
    ASSERT_RETURN(NULL != pModuleName, E_INVALIDARG);

    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        if(oModuleIt->m_strModuleName == pModuleName) {
            oModuleIt->m_pModule->Release();
            m_vModules.erase(oModuleIt);
            return S_OK;
        }
        ++oModuleIt;
    }

    return E_FAIL;
}

STDMETHODIMP
Framework::GetModule(BSTR pModuleName, IMirModule **ppModule)
{
    ASSERT_RETURN(NULL != pModuleName, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppModule, E_POINTER);

    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        if(oModuleIt->m_strModuleName == pModuleName) {
            oModuleIt->m_pModule->AddRef();
            *ppModule = oModuleIt->m_pModule;
            return S_OK;
        }
        ++oModuleIt;
    }

    return E_NOINTERFACE;
}

STDMETHODIMP
Framework::GetModulesCount(DWORD *pCount)
{
    ASSERT_RETURN(NULL != pCount, E_INVALIDARG);
    *pCount = (DWORD)m_vModules.size();
    return S_OK;
}

STDMETHODIMP
Framework::GetModules(IMirModule **pModule, DWORD nMaxCount)
{
    DWORD i = 0;
    ModuleList::iterator oModuleIt = m_vModules.begin();
    while(i < nMaxCount && oModuleIt != m_vModules.end()) {
        pModule[i] = oModuleIt->m_pModule;
        pModule[i]->AddRef();
        ++i;
        ++oModuleIt;
    }

    return S_OK;
}

STDMETHODIMP
Framework::RegisterComponent(BSTR pName, IUnknown *pComponent)
{
    ASSERT_RETURN(NULL != pName, E_INVALIDARG);
    ASSERT_RETURN(NULL != pComponent, E_INVALIDARG);

    pComponent->AddRef();
    m_vComponents[CComBSTR(pName)] = pComponent;

    return S_OK;
}

STDMETHODIMP
Framework::UnregisterComponent(BSTR pName)
{
    ASSERT_RETURN(NULL != pName, E_INVALIDARG);

    ComponentList::iterator oComponentIt = m_vComponents.find(pName);
    if(oComponentIt == m_vComponents.end()) {
        return E_NOINTERFACE;
    }

    oComponentIt->second->Release();
    m_vComponents.erase(oComponentIt);

    return S_OK;
}

STDMETHODIMP
Framework::GetComponent(BSTR pName, IUnknown **ppComponent)
{
    ASSERT_RETURN(NULL != pName, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppComponent, E_INVALIDARG);

    *ppComponent = NULL;

    ComponentList::iterator oComponentIt = m_vComponents.find(pName);
    if(oComponentIt == m_vComponents.end()) {
        return E_NOINTERFACE;
    }

    *ppComponent = oComponentIt->second;
    (*ppComponent)->AddRef();

    return S_OK;
}

HRESULT MirrorCreateObject(LPCTSTR pModule, REFGUID pGUID, void **pObject)
{
    CComPtr<IMirModule> pMirModule;
    if(FAILED(GetFramework()->GetModule(CComBSTR(pModule), &pMirModule)) || NULL == pMirModule) {
        return E_NOINTERFACE;
    }

    return pMirModule->CreateInstance(pGUID, pObject);
}