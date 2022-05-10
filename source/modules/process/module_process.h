#pragma once

#include "base/com/com_class.h"
#include "modules/mir_module.h"
#include "modules/process/app/dlg_process.h"
#include "modules/process/wait_chain/wait_chain_factory.h"

class MirModuleProcess :
    public IMirModule
{
public:
    MirModuleProcess();
    virtual ~MirModuleProcess();

    COM_CLASS_FACTORY(MirModuleProcess)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(MirModuleProcess)
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
    WaitChainFactory m_oWaitChainFactory;
    CDlgProcess m_oDlgProcess;
};