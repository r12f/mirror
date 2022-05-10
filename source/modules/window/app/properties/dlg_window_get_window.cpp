#include "stdafx.h"
#include "modules/builtin/component_config.h"
#include "modules/component_id.h"
#include "base/string_number_convertions.h"
#include "modules/window/app/properties/dlg_window_get_window.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindowGetWindow::CDlgWindowGetWindow()
{

}

CDlgWindowGetWindow::~CDlgWindowGetWindow()
{

}

VOID
CDlgWindowGetWindow::SetWindowTitle(LPCTSTR strTitle)
{
    m_strWindowTitle = strTitle;
    return;
}

VOID
CDlgWindowGetWindow::SetWindowHWND(HWND hWindow)
{
    m_hWindow = hWindow;
    return;
}

HWND
CDlgWindowGetWindow::GetWindowHWND()
{
    return m_hWindow;
}

LRESULT
CDlgWindowGetWindow::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oEditInputWindow = GetDlgItem(IDC_EDIT_WINDOW_GET_WINDOW_HWND);

    CString strInputWindow;
    strInputWindow.Format(_T("%08x"), m_hWindow);

    m_oEditInputWindow.SetWindowText(strInputWindow);

    SetWindowText(m_strWindowTitle);

    return TRUE;
}

VOID
CDlgWindowGetWindow::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowGetWindow::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgWindowGetWindow::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CString strWindow;
    m_oEditInputWindow.GetWindowText(strWindow);
    m_hWindow = (HWND)(DWORD_PTR)HexToInt(strWindow);

    EndDialog(IDOK);

    return;
}

VOID
CDlgWindowGetWindow::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}
