#include "stdafx.h"
#include "modules/process/wait_chain/wait_chain_api.h"

typedef VOID (WINAPI * FuncRegisterWaitChainCOMCallback)(
    __in PCOGETCALLSTATE CallStateCallback,
    __in PCOGETACTIVATIONSTATE ActivationStateCallback
    );

typedef HWCT (WINAPI * FuncOpenThreadWaitChainSession)(
    __in DWORD Flags,
    __in_opt PWAITCHAINCALLBACK callback
    );

typedef VOID (WINAPI * FuncCloseThreadWaitChainSession)(
    __in HWCT WctHandle
    );

typedef BOOL (WINAPI * FuncGetThreadWaitChain)(
    __in HWCT WctHandle,
    __in_opt DWORD_PTR Context,
    __in DWORD Flags,
    __in DWORD ThreadId,
    __inout LPDWORD NodeCount,
    __out_ecount(*NodeCount) PWAITCHAIN_NODE_INFO NodeInfoArray,
    __out LPBOOL IsCycle
    );

static HMODULE s_hAdvapi32Dll = NULL;
static FuncRegisterWaitChainCOMCallback s_pFuncRegisterWaitChainCOMCallback = NULL;
static FuncOpenThreadWaitChainSession s_pFuncOpenThreadWaitChainSession = NULL;
static FuncCloseThreadWaitChainSession s_pFuncCloseThreadWaitChainSession = NULL;
static FuncGetThreadWaitChain s_pFuncGetThreadWaitChain = NULL;

class WaitChainApiIniter
{
public:
    WaitChainApiIniter() {
        s_hAdvapi32Dll = ::LoadLibrary(_T("Advapi32.dll"));
        if(NULL != s_hAdvapi32Dll) {
            s_pFuncRegisterWaitChainCOMCallback = (FuncRegisterWaitChainCOMCallback)GetProcAddress(s_hAdvapi32Dll, "RegisterWaitChainCOMCallback");
            s_pFuncOpenThreadWaitChainSession = (FuncOpenThreadWaitChainSession)GetProcAddress(s_hAdvapi32Dll, "OpenThreadWaitChainSession");
            s_pFuncCloseThreadWaitChainSession = (FuncCloseThreadWaitChainSession)GetProcAddress(s_hAdvapi32Dll, "CloseThreadWaitChainSession");
            s_pFuncGetThreadWaitChain = (FuncGetThreadWaitChain)GetProcAddress(s_hAdvapi32Dll, "GetThreadWaitChain");
        }
    }

    ~WaitChainApiIniter() {}
};

static BOOL
InitWaitChainApi()
{
    static WaitChainApiIniter s_oWaitChainApiIniter;
    return (NULL != s_hAdvapi32Dll);
}

VOID
AdaRegisterWaitChainCOMCallback(
                                __in PCOGETCALLSTATE CallStateCallback,
                                __in PCOGETACTIVATIONSTATE ActivationStateCallback
                                )
{
    InitWaitChainApi();

    if(NULL == s_pFuncRegisterWaitChainCOMCallback) {
        return;
    }
    
    s_pFuncRegisterWaitChainCOMCallback(CallStateCallback, ActivationStateCallback);

    return;
}

HWCT
AdaOpenThreadWaitChainSession(
                              __in DWORD Flags,
                              __in_opt PWAITCHAINCALLBACK callback
                              )
{
    InitWaitChainApi();

    if(NULL == s_pFuncOpenThreadWaitChainSession) {
        return NULL;
    }

    return s_pFuncOpenThreadWaitChainSession(Flags, callback);
}

VOID
AdaCloseThreadWaitChainSession(
                               __in HWCT WctHandle
                               )
{
    InitWaitChainApi();

    if(NULL == s_pFuncCloseThreadWaitChainSession) {
        return;
    }

    s_pFuncCloseThreadWaitChainSession(WctHandle);

    return;
}

BOOL
AdaGetThreadWaitChain (
                       __in HWCT WctHandle,
                       __in_opt DWORD_PTR Context,
                       __in DWORD Flags,
                       __in DWORD ThreadId,
                       __inout LPDWORD NodeCount,
                       __out_ecount(*NodeCount) PWAITCHAIN_NODE_INFO NodeInfoArray,
                       __out LPBOOL IsCycle
                       )
{
    InitWaitChainApi();

    if(NULL == s_pFuncGetThreadWaitChain) {
        return FALSE;
    }

    return s_pFuncGetThreadWaitChain(WctHandle, Context, Flags, ThreadId, NodeCount, NodeInfoArray, IsCycle);
}