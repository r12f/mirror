#pragma once

#include "base/scoped_handle.h"
#include "base/com/simple_com_class.h"
#include "modules/process/component_wait_chain.h"
#include "modules/process/wait_chain/wait_chain_api.h"

class WaitChain :
    public SimpleComClassT<WaitChain, ComRef, IWaitChain>
{
public:
    WaitChain();
    virtual ~WaitChain();

    // Override IWaitChain
    STDMETHODIMP SetNeedUI(BOOL bNeed);
    STDMETHODIMP AnalyzeThread(DWORD nTid, IWaitChainListener *pListener);

protected:
    BOOL CreateSession();
    BOOL DestroySession();

private:
    BOOL m_bNeedUI;
    ScopedModule m_hOle32Dll;
    ScopedWCT m_hWct;
};