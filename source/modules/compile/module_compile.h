#pragma once

#include "modules/mir_module.h"
#include "base/com/com_class.h"
#include "modules/compile/ui/dlg_compile.h"

class MirModuleCompile :
    public IMirModule
{
public:
    MirModuleCompile();
    virtual ~MirModuleCompile();

    COM_CLASS_FACTORY(MirModuleCompile)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(MirModuleCompile)
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
    CDlgCompile m_oDlgCompile;
};