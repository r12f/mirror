#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_set_layered_window.h"

CDlgWindowSetLayeredWindow::CDlgWindowSetLayeredWindow()
    : CLayout(IDD)
    , CUIControl(this)
    , m_nColor(0)
    , m_nAlpha(0)
    , m_nFlag(0)
{
}

CDlgWindowSetLayeredWindow::~CDlgWindowSetLayeredWindow()
{

}

VOID
CDlgWindowSetLayeredWindow::SetColor(COLORREF nColor)
{
    m_nColor = nColor;
}

COLORREF
CDlgWindowSetLayeredWindow::GetColor()
{
    return m_nColor;
}

VOID
CDlgWindowSetLayeredWindow::SetAlpha(BYTE nAlpha)
{
    m_nAlpha = nAlpha;
}

BYTE
CDlgWindowSetLayeredWindow::GetAlpha()
{
    return m_nAlpha;
}

VOID
CDlgWindowSetLayeredWindow::SetFlag(DWORD nFlag)
{
    m_nFlag = nFlag;
}

DWORD
CDlgWindowSetLayeredWindow::GetFlag()
{
    return m_nFlag;
}

LRESULT
CDlgWindowSetLayeredWindow::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CString strOutput;
    m_oEditColorR = GetDlgItem(IDC_EDIT_WINDOW_SET_LAYERED_WINDOW_COLOR_R);
    strOutput.Format(_T("%lu"), (m_nColor >> 16) & 0xFF);
    m_oEditColorR.SetWindowText(strOutput);

    m_oEditColorG = GetDlgItem(IDC_EDIT_WINDOW_SET_LAYERED_WINDOW_COLOR_G);
    strOutput.Format(_T("%lu"), (m_nColor >> 8) & 0xFF);
    m_oEditColorG.SetWindowText(strOutput);

    m_oEditColorB = GetDlgItem(IDC_EDIT_WINDOW_SET_LAYERED_WINDOW_COLOR_B);
    strOutput.Format(_T("%lu"), m_nColor & 0xFF);
    m_oEditColorB.SetWindowText(strOutput);

    m_oEditAlpha = GetDlgItem(IDC_EDIT_WINDOW_SET_LAYERED_WINDOW_COLOR_ALPHA);
    strOutput.Format(_T("%lu"), m_nAlpha);
    m_oEditAlpha.SetWindowText(strOutput);

    m_oCheckUseColor = GetDlgItem(IDC_CHECK_WINDOW_SET_LAYERED_WINDOW_LWA_COLORKEY);
    m_oCheckUseColor.SetCheck(m_nFlag & LWA_COLORKEY);

    m_oCheckUseAlpha = GetDlgItem(IDC_CHECK_WINDOW_SET_LAYERED_WINDOW_LWA_ALPHA);
    m_oCheckUseAlpha.SetCheck(m_nFlag & LWA_ALPHA);

    return TRUE;
}

VOID
CDlgWindowSetLayeredWindow::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowSetLayeredWindow::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgWindowSetLayeredWindow::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    m_nColor = 0;

    CString strInput;
    m_oEditColorR.GetWindowText(strInput);
    m_nColor |= _ttoi(strInput) << 16;

    m_oEditColorG.GetWindowText(strInput);
    m_nColor |= _ttoi(strInput) << 8;

    m_oEditColorB.GetWindowText(strInput);
    m_nColor |= _ttoi(strInput);

    m_oEditAlpha.GetWindowText(strInput);
    m_nAlpha |= _ttoi(strInput);

    m_nFlag = 0;
    if(m_oCheckUseColor.GetCheck()) {
        m_nFlag |= LWA_COLORKEY;
    }

    if(m_oCheckUseAlpha.GetCheck()) {
        m_nFlag |= LWA_ALPHA;
    }

    EndDialog(IDOK);

    return;
}

VOID
CDlgWindowSetLayeredWindow::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}
