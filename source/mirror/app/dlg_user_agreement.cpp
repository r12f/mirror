#include "stdafx.h"
#include "app/dlg_user_agreement.h"
#include "mirror_license.h"

CDlgUserAgreement::CDlgUserAgreement()
    : m_bIsAgreed(FALSE)
{

}

CDlgUserAgreement::~CDlgUserAgreement()
{

}

BOOL
CDlgUserAgreement::IsAgreed()
{
    return m_bIsAgreed;
}

LRESULT
CDlgUserAgreement::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CEdit oEditUserAgreement = GetDlgItem(IDC_EDIT_USER_AGREEMENT);
    if(!oEditUserAgreement.IsWindow()) {
        return TRUE;
    }

    CString strContent = MIRROR_USER_AGREEMENT;

    oEditUserAgreement.SetWindowText(strContent);

    return TRUE;
}

VOID
CDlgUserAgreement::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgUserAgreement::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgUserAgreement::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CButton oBtnAgreed = GetDlgItem(IDC_CHECK_AGREE_USER_AGREEMENT);
    if(oBtnAgreed.IsWindow()) {
        m_bIsAgreed = oBtnAgreed.GetCheck();
    } 

    EndDialog(IDOK);

    return;
}

VOID
CDlgUserAgreement::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}
