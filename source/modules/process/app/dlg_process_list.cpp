#include "stdafx.h"
#include "modules/process/app/dlg_process_list.h"

CDlgProcessList::CDlgProcessList()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgProcessList::~CDlgProcessList()
{

}

LRESULT
CDlgProcessList::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oListProcesses.SubclassWindow(GetDlgItem(IDC_TREE_PROCESS_LIST));
    m_oListProcesses.SetExtendedListViewStyle(m_oListProcesses.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    return TRUE;
}

VOID
CDlgProcessList::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgProcessList::OnClose()
{
    SetMsgHandled(FALSE);
    m_oListProcesses.UnsubclassWindow();
    DestroyWindow();
    return;
}

VOID
CDlgProcessList::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}
