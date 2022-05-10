#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_modify_prop.h"
#include "modules/window/app/window_app_utils.h"
#include "base/string_number_convertions.h"

CDlgWindowModifyProp::CDlgWindowModifyProp()
    : CLayout(IDD)
    , CUIControl(this)
    , m_bIsAddPropDialog(FALSE)
    , m_bIsAtomProp(FALSE)
    , m_hPropValue(NULL)
{
    
}

CDlgWindowModifyProp::~CDlgWindowModifyProp()
{
    
}

VOID
CDlgWindowModifyProp::SetDialogMethod(BOOL bUseForAdd)
{
    m_bIsAddPropDialog = bUseForAdd;
}

VOID
CDlgWindowModifyProp::SetInitProp(BOOL bIsAtomProp, CString &strPropKey, HANDLE hPropValue)
{
    m_bIsAtomProp = bIsAtomProp;
    m_strPropKey = strPropKey;
    m_hPropValue = hPropValue;

    return;
}

VOID
CDlgWindowModifyProp::GetInputProp(CString &strPropKey, HANDLE &hPropValue)
{
    strPropKey = m_strPropKey;
    hPropValue = m_hPropValue;
    return;
}

LRESULT
CDlgWindowModifyProp::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oPropKey = GetDlgItem(IDC_EDIT_WINDOW_PROP_KEY);
    m_oPropValue = GetDlgItem(IDC_EDIT_WINDOW_PROP_VALUE);

    if(!m_bIsAddPropDialog) {
        CString strPropValue;
        strPropValue.Format(_T("%08x"), m_hPropValue);
        m_oPropKey.SetWindowText(m_strPropKey);
        m_oPropValue.SetWindowText(strPropValue);
    }

    if(m_bIsAtomProp) {
        m_oPropKey.SetReadOnly(TRUE);
    }

    return TRUE;
}

VOID
CDlgWindowModifyProp::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgWindowModifyProp::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    m_oPropKey.GetWindowText(m_strPropKey);

    CString strPropValue;
    m_oPropValue.GetWindowText(strPropValue);
    m_hPropValue = (HANDLE)(DWORD_PTR)HexToInt(strPropValue);

    EndDialog(IDOK);
}

VOID
CDlgWindowModifyProp::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
}