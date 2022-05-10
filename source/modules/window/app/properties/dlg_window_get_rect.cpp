#include "stdafx.h"
#include "modules/builtin/component_config.h"
#include "modules/component_id.h"
#include "base/string_number_convertions.h"
#include "modules/window/app/properties/dlg_window_get_rect.h"
#include "modules/window/app/window_app_utils.h"

static TCHAR *s_vRectLabels[4] = { _T("X:"), _T("Y:"), _T("W:"), _T("H:") };
static TCHAR *s_vMarginLabels[4] = { _T("L:"), _T("T:"), _T("R:"), _T("B:") };

CDlgWindowGetRect::CDlgWindowGetRect()
{
    m_bIsGetRectDialog = true;
    memset(m_vOffset, 0, sizeof(m_vOffset));
}

CDlgWindowGetRect::~CDlgWindowGetRect()
{

}

VOID
CDlgWindowGetRect::SetWindowTitle(LPCTSTR strTitle)
{
    m_strWindowTitle = strTitle;

    return;
}

VOID
CDlgWindowGetRect::SetRect(RECT oRect)
{
    m_bIsGetRectDialog = true;

    m_vOffset[0] = oRect.left;
    m_vOffset[1] = oRect.top;
    m_vOffset[2] = oRect.right - oRect.left;
    m_vOffset[3] = oRect.bottom - oRect.top;

    return;
}

VOID
CDlgWindowGetRect::GetRect(RECT *pRect)
{
    if(NULL == pRect) {
        return;
    }

    pRect->left = m_vOffset[0];
    pRect->top = m_vOffset[1];
    pRect->right = m_vOffset[0] + m_vOffset[2];
    pRect->bottom = m_vOffset[1] + m_vOffset[3];

    return;
}

VOID
CDlgWindowGetRect::SetMargin(MARGINS oMargin)
{
    m_bIsGetRectDialog = false;

    m_vOffset[0] = oMargin.cxLeftWidth;
    m_vOffset[1] = oMargin.cyTopHeight;
    m_vOffset[2] = oMargin.cxRightWidth;
    m_vOffset[3] = oMargin.cyBottomHeight;

    return;
}

VOID
CDlgWindowGetRect::GetMargin(MARGINS *pMargin)
{
    if(NULL == pMargin) {
        return;
    }

    pMargin->cxLeftWidth = m_vOffset[0];
    pMargin->cyTopHeight = m_vOffset[1];
    pMargin->cxRightWidth = m_vOffset[2];
    pMargin->cyBottomHeight = m_vOffset[3];

    return;
}

LRESULT
CDlgWindowGetRect::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    SetWindowText(m_strWindowTitle);
    
    // Static
    m_vStaLable[0] = GetDlgItem(IDC_STATIC_WINDOW_GET_RECT_1);
    m_vStaLable[1] = GetDlgItem(IDC_STATIC_WINDOW_GET_RECT_2);
    m_vStaLable[2] = GetDlgItem(IDC_STATIC_WINDOW_GET_RECT_3);
    m_vStaLable[3] = GetDlgItem(IDC_STATIC_WINDOW_GET_RECT_4);

    TCHAR **pLables = m_bIsGetRectDialog ? s_vRectLabels : s_vMarginLabels;
    int i = 0;
    for(i = 0; i < 4; ++i) {
        m_vStaLable[i].SetWindowText(pLables[i]);
    }

    // Edit
    m_vEditInputOffset[0] = GetDlgItem(IDC_EDIT_WINDOW_GET_RECT_1);
    m_vEditInputOffset[1] = GetDlgItem(IDC_EDIT_WINDOW_GET_RECT_2);
    m_vEditInputOffset[2] = GetDlgItem(IDC_EDIT_WINDOW_GET_RECT_3);
    m_vEditInputOffset[3] = GetDlgItem(IDC_EDIT_WINDOW_GET_RECT_4);

    CString strOffset;
    for(i = 0; i < 4; ++i) {
        strOffset.Format(_T("%ld"), m_vOffset[i]);
        m_vEditInputOffset[i].SetWindowText(strOffset);
    }

    return TRUE;
}

VOID
CDlgWindowGetRect::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowGetRect::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgWindowGetRect::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CString strOffset;
    int i = 0;
    for(i = 0; i < 4; ++i) {
        m_vEditInputOffset[i].GetWindowText(strOffset);
        m_vOffset[i] = _ttoi(strOffset);
    }

    EndDialog(IDOK);

    return;
}

VOID
CDlgWindowGetRect::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}
