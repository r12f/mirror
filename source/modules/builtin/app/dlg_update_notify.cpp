#include "stdafx.h"
#include "modules/builtin/app/dlg_update_notify.h"

CDlgUpdateNotify::CDlgUpdateNotify()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgUpdateNotify::~CDlgUpdateNotify()
{

}

VOID
CDlgUpdateNotify::SetVersion(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial)
{
    m_nVerMajor = nVerMajor;
    m_nVerMinor = nVerMinor;
    m_nVerBuild = nVerBuild;
    m_nVerSpecial = nVerSpecial;

    return;
}

VOID
CDlgUpdateNotify::SetWhatsNew(LPCTSTR pWhatsNew)
{
    m_strWhatsNew = pWhatsNew;
}

LRESULT
CDlgUpdateNotify::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CString strNotify;
    strNotify.Format(_T("发现更新的版本：%lu.%lu.%lu.%lu，现在是否更新？"), m_nVerMajor, m_nVerMinor, m_nVerBuild, m_nVerSpecial);

    m_oStaNotify = GetDlgItem(IDC_STATIC_UPDATE_NOTIFY);
    m_oStaNotify.SetWindowText(strNotify);

    m_oEditWhatsNew = GetDlgItem(IDC_EDIT_UPDATE_WHATS_NEW);
    m_oEditWhatsNew.SetWindowText(m_strWhatsNew);

    return TRUE;
}

VOID
CDlgUpdateNotify::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgUpdateNotify::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgUpdateNotify::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDOK);
    return;
}

VOID
CDlgUpdateNotify::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgUpdateNotify::OnSkipCurrentVersion(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDC_BUTTON_SKIP_CURRENT_VERSION);
    return;
}
