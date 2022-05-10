#pragma once

#include <list>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"

class CDlgProcessList :
    public CDialogImpl<CDlgProcessList>,
    public CLayout,
    public CUIControl
{
    struct ProcessInfo {
        DWORD m_nPid;
        CString m_strProcName;
        CString m_strCmdLine;
        CString m_strCompany;
        CString m_strDescription;
    };

public:
    enum { IDD = IDD_DIALOG_PROCESS_LIST };

    CDlgProcessList();
    virtual ~CDlgProcessList();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgProcessList)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

private:
    CEditableListCtrl m_oListProcesses;
};