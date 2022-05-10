#include "stdafx.h"
#include "modules/process/app/dlg_process.h"

CDlgProcess::CDlgProcess()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgProcess::~CDlgProcess()
{

}

LRESULT
CDlgProcess::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oTabProcess.Create(m_hWnd);

    m_oDlgProcessList.Create(m_oTabProcess.m_tab);
    m_oTabProcess.AddPage(m_oDlgProcessList, _T("进程线程"));

    m_oTabProcess.SetActivePage(0);

    return TRUE;
}

VOID
CDlgProcess::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    m_oTabProcess.ShowWindow(SW_SHOW);
    return;
}

VOID
CDlgProcess::OnClose()
{
    SetMsgHandled(FALSE);
    m_oTabProcess.DestroyWindow();
    DestroyWindow();
    return;
}

VOID
CDlgProcess::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

BOOL
CDlgProcess::DoCustomLayout()
{
    WTL::CRect oRect;
    GetClientRect(&oRect);

    WTL::CRect oTabRect = oRect;
    oTabRect.DeflateRect(5, 5, 5, 5);

    m_oTabProcess.SetWindowPos(NULL , &oTabRect, SWP_NOZORDER);

    return TRUE;
}