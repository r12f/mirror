#pragma once

#include "modules/mir_module.h"
#include "base/com/com_class.h"
#include "modules/builtin/app/dlg_config.h"

class MirModuleBuiltin :
    public IMirModule
{
public:
    MirModuleBuiltin();
    virtual ~MirModuleBuiltin();

    COM_CLASS_FACTORY(MirModuleBuiltin)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(MirModuleBuiltin)
            QI_MAPPING(IMirModule)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP Create();
    STDMETHODIMP Destroy();
    STDMETHODIMP Init();
    STDMETHODIMP Uninit();
    STDMETHODIMP RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI);
    STDMETHODIMP CreateInstance(REFGUID pGUID, void **pObject);

private:
    CDlgConfig m_oDlgConfig;
};