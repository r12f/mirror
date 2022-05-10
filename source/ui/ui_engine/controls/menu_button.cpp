#include "stdafx.h"
#include "ui/ui_engine/controls/menu_button.h"

CMenuButton::CMenuButton()
    : m_nMenuId(0)
    , m_bHandleClick(FALSE)
{

}

CMenuButton::CMenuButton(BOOL bHandleClick)
    : m_nMenuId(0)
    , m_bHandleClick(bHandleClick)
{

}

CMenuButton::~CMenuButton()
{

}

BOOL
CMenuButton::SubclassWindow(HWND hWnd)
{
    if (!::IsWindow(hWnd)) {
        return FALSE;
    }

    m_strText.GetBuffer(1024);
    ::GetWindowText(hWnd, m_strText.GetBuffer(), 1024);
    m_strText.ReleaseBuffer();

    //::SetWindowText(hWnd, _T(""));

    if (__super::SubclassWindow(hWnd)) {
        ::SetWindowText(hWnd, m_strText);
        m_strText.Empty();
        return FALSE;
    }

    return TRUE;
}

VOID
CMenuButton::OnLButtonUp(UINT nFlag, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);

    CMenu oMenu;
    CMenuHandle oPopupMenu;
    if(0 == m_nMenuId) {
        oMenu = CreateContextMenu();
        oPopupMenu = oMenu;
    } else {
        if(oMenu.LoadMenu(m_nMenuId)) {
            oPopupMenu = oMenu.GetSubMenu(0);
        }
    }

    if(NULL == oPopupMenu) {
        return;
    }

    SetMsgHandled(TRUE);

    if(!GetCheck()) {
        SetCheck(TRUE);
    }

    BeforeContextMenuPopup(oPopupMenu);

    WTL::CRect oRect;
    GetWindowRect(&oRect);

    oPopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, oRect.left + 1, oRect.bottom + 1, (m_bHandleClick ? m_hWnd : GetParent()), NULL);

    AfterContextMenuPopup(oPopupMenu);

    SetCheck(FALSE);

    return;
}

VOID
CMenuButton::OnCommand(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    if(OnMenuItemClick(nId)) {
        return;
    }

    SetMsgHandled(FALSE);
}

/*
LRESULT
CMenuButton::OnSetText(LPCTSTR strText)
{
    if (NULL != strText) {
        m_strText = strText;
    }

    SetMsgHandled(TRUE);

    return TRUE;
}
*/
