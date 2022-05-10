#pragma once

#include <Wct.h>
#include "base/scoped_handle.h"

VOID
AdaRegisterWaitChainCOMCallback(
                                __in PCOGETCALLSTATE CallStateCallback,
                                __in PCOGETACTIVATIONSTATE ActivationStateCallback
                                );

HWCT
AdaOpenThreadWaitChainSession(
                              __in DWORD Flags,
                              __in_opt PWAITCHAINCALLBACK callback
                              );

VOID
AdaCloseThreadWaitChainSession(
                               __in HWCT WctHandle
                               );

BOOL
AdaGetThreadWaitChain(
                      __in HWCT WctHandle,
                      __in_opt DWORD_PTR Context,
                      __in DWORD Flags,
                      __in DWORD ThreadId,
                      __inout LPDWORD NodeCount,
                      __out_ecount(*NodeCount) PWAITCHAIN_NODE_INFO NodeInfoArray,
                      __out LPBOOL IsCycle
                      );

struct ScopedHandleTraitHWCT {
    static BOOL Destroy(HWCT h) { AdaCloseThreadWaitChainSession(h); return TRUE; }
    static HWCT NullHandle() { return NULL; }
};

typedef ScopedHandleBase<HWCT, ScopedHandleTraitHWCT> ScopedWCT;