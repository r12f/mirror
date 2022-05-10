#include "stdafx.h"
#include "modules/process/wait_chain/wait_chain_factory.h"
#include "modules/process/wait_chain/wait_chain.h"

WaitChainFactory::WaitChainFactory()
{

}

WaitChainFactory::~WaitChainFactory()
{

}

HRESULT
WaitChainFactory::CreateWaitChain(IWaitChain **pWaitChain)
{
    if(!InitCOMAccess()) {
        return E_FAIL;
    }

    return WaitChain::CreateObject(__uuidof(IWaitChain), (void **)pWaitChain);
}

BOOL
WaitChainFactory::InitCOMAccess()
{
    if(NULL != m_hOle32Dll) {
        return TRUE;
    }

    PCOGETCALLSTATE       CallStateCallback;
    PCOGETACTIVATIONSTATE ActivationStateCallback;

    // Get a handle to OLE32.DLL. You must keep this handle around
    // for the life time for any WCT session.
    ScopedModule hOle32Dll = LoadLibrary(L"ole32.dll");
    if(NULL == hOle32Dll) {
        MLOG_DEBUG("WaitChainFactory", "无法初始化等待链，获取ole32.dll失败：0x%X\n", GetLastError());
        return FALSE;
    }

    // Retrieve the function addresses for the COM helper APIs.
    CallStateCallback = (PCOGETCALLSTATE)GetProcAddress(hOle32Dll, "CoGetCallState");
    if (NULL == CallStateCallback) {
        MLOG_DEBUG("WaitChainFactory", "无法初始化等待链，获取ole32!CoGetCallState失败：0x%X\n", GetLastError());
        return FALSE;
    }

    ActivationStateCallback = (PCOGETACTIVATIONSTATE)GetProcAddress(hOle32Dll, "CoGetActivationState");
    if (NULL == ActivationStateCallback) {
        MLOG_DEBUG("WaitChainFactory", "无法初始化等待链，获取ole32!CoGetActivationState失败：0x%X\n", GetLastError());
        return FALSE;
    }

    // Register these functions with WCT.
    AdaRegisterWaitChainCOMCallback(CallStateCallback, ActivationStateCallback);

    m_hOle32Dll.Attach(hOle32Dll.Detach());

    return TRUE;
}