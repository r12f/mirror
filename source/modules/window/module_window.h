#pragma once

#include <map>
#include "modules/mir_module.h"
#include "modules/builtin/component_config.h"
#include "base/com/com_class.h"
#include "modules/window/app/dlg_window.h"

class MirModuleWindow :
    public IMirModule,
    public IConfigProvider
{
    typedef std::map<CComBSTR, CComBSTR> ConfigMap;

public:
    MirModuleWindow();
    virtual ~MirModuleWindow();

    COM_CLASS_FACTORY(MirModuleWindow)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(MirModuleWindow)
            QI_MAPPING(IMirModule)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP Create();
    STDMETHODIMP Destroy();
    STDMETHODIMP Init();
    STDMETHODIMP Uninit();
    STDMETHODIMP RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI);
    STDMETHODIMP CreateInstance(REFGUID pGUID, void **pObject);

    STDMETHODIMP GetRawConfig(BSTR strSection, BSTR strKey, BSTR *pRawValue);
    STDMETHODIMP SetRawConfig(BSTR strSection, BSTR strKey, BSTR strRawValue);

private:
    CDlgWindow m_oDlgWindow;
    ConfigMap m_vWindowConfigs;
};