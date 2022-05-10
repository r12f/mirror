#include "stdafx.h"
#include "ui/ui_engine/controls/tree_ctrl_ex.h"

CTreeCtrlEx::CTreeCtrlEx()
    : m_nMenuId(0)
{

}

CTreeCtrlEx::~CTreeCtrlEx()
{

}

VOID
CTreeCtrlEx::SetContextMenu(UINT nMenuId)
{
    m_nMenuId = nMenuId;
}

VOID
CTreeCtrlEx::OnRButtonDown(UINT nFlag, WTL::CPoint oPoint)
{
    UINT nHTFlags = 0;
    HTREEITEM hMenuItem = HitTest(oPoint, &nHTFlags);
    SelectItem(hMenuItem);

    CMenu oMenu;
    CMenuHandle oPopupMenu;
    if(0 == m_nMenuId) {
        HMENU hMenu = CreateContextMenu(hMenuItem, nHTFlags, oPoint);
        if(NULL != hMenu) {
            oMenu = hMenu;
            oPopupMenu = oMenu;
        }
    } else {
        if(oMenu.LoadMenu(m_nMenuId)) {
            oPopupMenu = oMenu.GetSubMenu(0);
        }
    }

    if(NULL == oPopupMenu) {
        SetMsgHandled(FALSE);
        return;
    }

    BeforeContextMenuPopup(oPopupMenu);

    WTL::CPoint oScreenPoint = oPoint;
    ::ClientToScreen(m_hWnd, &oScreenPoint);
    oPopupMenu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, oScreenPoint.x, oScreenPoint.y, GetParent(), NULL);

    AfterContextMenuPopup(oPopupMenu);

    return;
}

HMENU
CTreeCtrlEx::CreateContextMenu(HTREEITEM hMenuItem, UINT nHTFlags, WTL::CPoint oPoint)
{
    return NULL;
}

VOID
CTreeCtrlEx::BeforeContextMenuPopup(CMenuHandle &oMenu)
{
    return;
}

VOID
CTreeCtrlEx::AfterContextMenuPopup(CMenuHandle &oMenu)
{
    return;
}