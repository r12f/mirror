#pragma once

#include "base/scoped_handle.h"
#include "modules/process/wait_chain/wait_chain_api.h"
#include "modules/process/component_wait_chain.h"

class WaitChainFactory
{
public:
    WaitChainFactory();
    ~WaitChainFactory();

    HRESULT CreateWaitChain(IWaitChain **pWaitChain);

protected:
    BOOL InitCOMAccess();

private:
    ScopedModule m_hOle32Dll;
};