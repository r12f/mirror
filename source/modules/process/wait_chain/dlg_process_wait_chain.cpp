#include "stdafx.h"
#include "modules/process/wait_chain/dlg_process_wait_chain.h"
#include "modules/process/wait_chain/wait_chain_node_parser.h"

CDlgProcessWaitChain::CDlgProcessWaitChain()
    : CUIControl(this)
    , m_nPid(0)
    , m_nTid(0)
    , m_pNodes(NULL)
    , m_nNodeCount(0)
    , m_bIsDeadLock(FALSE)
{

}

CDlgProcessWaitChain::~CDlgProcessWaitChain()
{

}

VOID
CDlgProcessWaitChain::SetThreadInfo(LPCTSTR pProcName, DWORD nPid, DWORD nTid)
{
    m_strProcName = pProcName;
    m_nPid = nPid;
    m_nTid = nTid;
}

VOID
CDlgProcessWaitChain::SetWaitChainNodes(WAITCHAIN_NODE_INFO *pNodes, DWORD nNodeCount, BOOL bIsDeadLock)
{
    m_pNodes = pNodes;
    m_nNodeCount = nNodeCount;
    m_bIsDeadLock = bIsDeadLock;
}

LRESULT
CDlgProcessWaitChain::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CString strTitle;
    strTitle.Format(_T("等待链 - %s - [%lu:%lu]"), m_strProcName.GetBuffer(), m_nPid, m_nTid);
    SetWindowText(strTitle);

    return TRUE;
}

VOID
CDlgProcessWaitChain::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgProcessWaitChain::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgProcessWaitChain::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDOK);
    return;
}

VOID
CDlgProcessWaitChain::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}

BOOL
CDlgProcessWaitChain::ResetData(DWORD nParam)
{
    m_strChainInfo = _T("");
    return TRUE;
}

BOOL
CDlgProcessWaitChain::FetchData(DWORD nParam)
{
    if(NULL == m_pNodes) {
        return FALSE;
    }

    if(m_bIsDeadLock) {
        m_strChainInfo += _T("检测结果：检测到死锁!!!\r\n\r\n详细信息：\r\n");
    } else {
        m_strChainInfo += _T("检测结果：未检测到死锁\r\n\r\n详细信息：\r\n");
    }

    DWORD i = 0;
    CString strNodeInfo;
    for(i = 0; i < m_nNodeCount; ++i) {
        strNodeInfo.Format(_T("节点 #%lu："), i);
        m_strChainInfo += strNodeInfo;
        m_strChainInfo += WaitChainNodeParser(&(m_pNodes[i])).Info();
        m_strChainInfo += _T("\r\n");
    }

    return TRUE;
}

BOOL
CDlgProcessWaitChain::UpdateUI(DWORD nParam)
{
    CEdit oWaitChainInfo = GetDlgItem(IDC_EDIT_PROCESS_WAIT_CHAIN_INFO);
    if(!oWaitChainInfo.IsWindow()) {
        return FALSE;
    }

    oWaitChainInfo.SetWindowText(m_strChainInfo);

    return TRUE;
}