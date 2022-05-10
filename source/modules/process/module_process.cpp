#include "stdafx.h"
#include "modules/process/module_process.h"
#include "modules/framework.h"

MirModuleProcess::MirModuleProcess()
{
    
}

MirModuleProcess::~MirModuleProcess()
{

}

STDMETHODIMP
MirModuleProcess::Create()
{
    return S_OK;
}

STDMETHODIMP
MirModuleProcess::Destroy()
{
    return S_OK;
}

STDMETHODIMP
MirModuleProcess::Init()
{
    return S_OK;
}

STDMETHODIMP
MirModuleProcess::Uninit()
{
    return S_OK;
}


STDMETHODIMP
MirModuleProcess::RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI)
{
    ASSERT_RETURN(NULL != pTitle, E_INVALIDARG);
    ASSERT_RETURN(NULL != pUI, E_INVALIDARG);

    // 创建窗口
    /*
    m_oDlgProcess.Create(hMainWnd);
    ASSERT_RETURN(m_oDlgProcess.IsWindow(), E_FAIL);

    *pTitle = CComBSTR(L"进程信息").Detach();
    *pUI = m_oDlgProcess.m_hWnd;
    */

    return S_OK;
}

HRESULT
MirModuleProcess::CreateInstance(REFGUID pGUID, void **pObject)
{
    if(IsEqualGUID(pGUID, __uuidof(IWaitChain))) {
        return m_oWaitChainFactory.CreateWaitChain((IWaitChain **)pObject);
    }

    return E_NOINTERFACE;
}