#include "stdafx.h"
#include "ui/ui_engine/controls/float_edit.h"

CDlgFloatEdit::CDlgFloatEdit()
    : CLayout(IDD)
{

}

CDlgFloatEdit::~CDlgFloatEdit()
{

}

BOOL
CDlgFloatEdit::PreTranslateMessage(MSG* pMsg)
{
    return ::IsDialogMessage(m_hWnd, pMsg);
}

LRESULT
CDlgFloatEdit::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oEditInput = GetDlgItem(IDC_EDIT_FLOAT_EDIT);
    return TRUE;
}

VOID
CDlgFloatEdit::OnCommand(UINT nNotify, UINT nId, HWND hWndCtrl)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgFloatEdit::OnKillFocus(HWND hFocus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgFloatEdit::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    m_oEditInput.SetFocus();
    return;
}


VOID
CDlgFloatEdit::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgFloatEdit::OnDestroy()
{
    m_oEditInput.Detach();
    SetMsgHandled(FALSE);
    return;
}

void
CDlgFloatEdit::OnFinalMessage(HWND)
{
    return;
}

BOOL
CDlgFloatEdit::DoCustomLayout()
{
    RECT oRect = {0};
    GetClientRect(&oRect);

    m_oEditInput.SetWindowPos(NULL, &oRect, SWP_NOZORDER);

    return TRUE;
}