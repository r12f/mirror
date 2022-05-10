#pragma once

#include <list>
#include <map>
#include "base/com/com_class.h"
#include "modules/framework.h"
#include "modules/mir_module.h"

class Framework :
    public IFramework,
    public IFrameworkControl
{
    struct ModuleInfo {
        CComBSTR m_strModuleName;
        IMirModule * m_pModule;
    };
    typedef std::list<ModuleInfo> ModuleList;
    typedef std::map<CComBSTR, IUnknown *> ComponentList;

public:
    Framework();
    virtual ~Framework();

    COM_CLASS_FACTORY(Framework)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(Framework)
            QI_MAPPING(IFramework)
            QI_MAPPING(IFrameworkControl)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Control
    STDMETHODIMP Init();
    STDMETHODIMP Uninit();
    STDMETHODIMP InitModules();
    STDMETHODIMP UninitModules();

    // Module
    STDMETHODIMP RegisterModule(BSTR pModuleName, IMirModule *pModule);
    STDMETHODIMP UnregisterModule(BSTR pModuleName);
    STDMETHODIMP GetModule(BSTR pModuleName, IMirModule **ppModule);
    STDMETHODIMP GetModulesCount(DWORD *pCount);
    STDMETHODIMP GetModules(IMirModule **pModule, DWORD nMaxCount);

    // Component
    STDMETHODIMP RegisterComponent(BSTR pName, IUnknown *pComponent);
    STDMETHODIMP UnregisterComponent(BSTR pName);
    STDMETHODIMP GetComponent(BSTR pName, IUnknown **ppComponent);

private:
    ModuleList m_vModules;
    ComponentList m_vComponents;
};