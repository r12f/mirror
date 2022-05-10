#include "stdafx.h"
#include "base/system/process_control.h"
#include "base/system/thread_control.h"
#include "modules/process/wait_chain/wait_chain.h"
#include "modules/process/wait_chain/dlg_process_wait_chain.h"

WaitChain::WaitChain()
    : m_bNeedUI(TRUE)
{
    CreateSession();
}

WaitChain::~WaitChain()
{
    DestroySession();
}

HRESULT
WaitChain::SetNeedUI(BOOL bNeed)
{
    m_bNeedUI = bNeed;
    return S_OK;
}

HRESULT
WaitChain::AnalyzeThread(DWORD nTid, IWaitChainListener *pListener)
{
    ASSERT_RETURN(0 != nTid, E_INVALIDARG);
    ASSERT_RETURN(NULL != m_hWct, E_FAIL);

    CComPtr<IWaitChainListener> spListener = pListener;

    WAITCHAIN_NODE_INFO *vWaitChain = NULL;;
    DWORD nNodeCount = WCT_MAX_NODE_COUNT;
    BOOL bIsDeadLock = FALSE;

    for(;;) {
        vWaitChain = new WAITCHAIN_NODE_INFO[nNodeCount];
        if(NULL == vWaitChain) {
            return E_OUTOFMEMORY;
        }

        if(AdaGetThreadWaitChain(m_hWct, NULL, WCTP_GETINFO_ALL_FLAGS, nTid, &nNodeCount, vWaitChain, &bIsDeadLock)) {
            break;
        }

        DWORD nErrCode = ::GetLastError();
        if(nErrCode != ERROR_MORE_DATA) {
            MLOG_DEBUG("WaitChain", "获取等待链失败：0x%08x", nErrCode);
            return E_FAIL;
        }

        delete vWaitChain;
        vWaitChain = NULL;
    }

    if(NULL != spListener) {
        DWORD i = 0;
        for(i = 0; i < nNodeCount; ++i) {
            spListener->OnWait(&(vWaitChain[i]));
        }

        spListener->OnDeadLock(bIsDeadLock);
    }

    MLOG_INFO("WaitChain", "获取等待链成功！");

    if(m_bNeedUI) {
        CString strProcName;

        ThreadControl oThread(nTid, THREAD_QUERY_INFORMATION);
        DWORD nPid = oThread.GetProcessId();
        if(0 != nPid) {
            ProcessControl oProc(nPid, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION);
            oProc.GetExeFileName(strProcName);
        }

        CDlgProcessWaitChain oWaitChain;
        oWaitChain.SetThreadInfo(strProcName, nPid, nTid);
        oWaitChain.SetWaitChainNodes(vWaitChain, nNodeCount, bIsDeadLock);
        oWaitChain.DoModal();
    }

    delete vWaitChain;
    vWaitChain = NULL;

    return S_OK;
}

BOOL
WaitChain::CreateSession()
{
    m_hWct.Attach(AdaOpenThreadWaitChainSession(0, NULL));
    if(NULL == m_hWct) {
        return FALSE;
    }

    return TRUE;
}

BOOL
WaitChain::DestroySession()
{
    m_hWct.Reset();

    return TRUE;
}
