#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/process/wait_chain/wait_chain_api.h"

class CDlgProcessWaitChain:
    public CDialogImpl<CDlgProcessWaitChain>,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PROCESS_WAIT_CHAIN };

    CDlgProcessWaitChain();
    virtual ~CDlgProcessWaitChain();

    VOID SetThreadInfo(LPCTSTR pProcName, DWORD nPid, DWORD nTid);
    VOID SetWaitChainNodes(WAITCHAIN_NODE_INFO *pNodes, DWORD nNodeCount, BOOL bIsDeadLock);

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgProcessWaitChain)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    
protected:
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

private:
    CString m_strProcName;
    DWORD m_nPid;
    DWORD m_nTid;
    WAITCHAIN_NODE_INFO *m_pNodes;
    DWORD m_nNodeCount;
    BOOL m_bIsDeadLock;

    CString m_strChainInfo;
};