#include "stdafx.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"

CEditableListCtrl::CEditableListCtrl()
: m_nMenuId(0)
{

}

CEditableListCtrl::~CEditableListCtrl()
{

}

VOID
CEditableListCtrl::SetContextMenu(UINT nMenuId)
{
    m_nMenuId = nMenuId;
}

VOID
CEditableListCtrl::ClearAllSelection()
{
    int nIdx = GetNextItem(-1, LVNI_SELECTED);
    while(-1 != nIdx) {
        SetItemState(nIdx, 0, LVIS_SELECTED);
        nIdx = GetNextItem(nIdx,LVNI_SELECTED);
    }
}

VOID
CEditableListCtrl::OnMsgLButtonDblClk(UINT nFlag, WTL::CPoint oPoint)
{
    memset(&m_oHitInfo, 0, sizeof(HitInfo));

    LVHITTESTINFO oHitTestInfo = {0};
    oHitTestInfo.pt = oPoint;
    SubItemHitTestEx(&oHitTestInfo);
    MakeHitTestInfo(&oHitTestInfo);

    if(!OnLButtonDblClk(&m_oHitInfo)) {
        SetMsgHandled(FALSE);
    }

    return;
}

VOID
CEditableListCtrl::OnMsgRButtonDown(UINT nFlag, WTL::CPoint oPoint)
{
    memset(&m_oHitInfo, 0, sizeof(HitInfo));

    LVHITTESTINFO oHitTestInfo = {0};
    oHitTestInfo.pt = oPoint;
    SubItemHitTestEx(&oHitTestInfo);
    MakeHitTestInfo(&oHitTestInfo);

    ClearAllSelection();
    if(0 <= oHitTestInfo.iItem) {
        SetItemState(oHitTestInfo.iItem, LVIS_SELECTED, LVIS_SELECTED | LVIS_FOCUSED);
    }

    CMenu oMenu;
    CMenuHandle oMenuHandle;
    if(0 == m_nMenuId) {
        HMENU hMenu = CreateContextMenu(&m_oHitInfo);
        if(NULL != hMenu) {
            oMenu = hMenu;
            oMenuHandle = oMenu;
        }
    } else {
        if(oMenu.LoadMenu(m_nMenuId)) {
            oMenuHandle = oMenu.GetSubMenu(0);
        }
    }

    if(NULL == oMenuHandle) {
        SetMsgHandled(FALSE);
        return;
    }

    BeforeMenuPopup(oMenuHandle);

    WTL::CPoint oScreenPoint = oPoint;
    ::ClientToScreen(m_hWnd, &oScreenPoint);
    oMenuHandle.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, oScreenPoint.x, oScreenPoint.y, GetParent(), NULL);

    AfterMenuPopup(oMenuHandle);

    return;
}

HMENU
CEditableListCtrl::CreateContextMenu(HitInfo *pHitInfo)
{
    return NULL;
}

VOID
CEditableListCtrl::BeforeMenuPopup(HMENU oMenuHandle)
{

}

VOID
CEditableListCtrl::AfterMenuPopup(HMENU oMenuHandle)
{

}

BOOL
CEditableListCtrl::OnLButtonDblClk(HitInfo *pHitInfo)
{
    return TRUE;
}

VOID
CEditableListCtrl::MakeHitTestInfo(LVHITTESTINFO *pHitTestInfo)
{
    m_oHitInfo.m_oPoint = pHitTestInfo->pt;
    m_oHitInfo.nItemId = pHitTestInfo->iItem;
    m_oHitInfo.nSubItemId = pHitTestInfo->iSubItem;

    if(0 > m_oHitInfo.nItemId || 0 > m_oHitInfo.nSubItemId) {
        return;
    }

    GetSubItemRect(m_oHitInfo.nItemId, m_oHitInfo.nSubItemId, LVIR_BOUNDS, &m_oHitInfo.m_oItemRect);

    int i, nColumnWidth = 0;
    for(i = 0; i < m_oHitInfo.nSubItemId; ++i) {
        nColumnWidth += GetColumnWidth(i);
    }

    m_oHitInfo.m_oItemRect.left = nColumnWidth;
    m_oHitInfo.m_oItemRect.right = nColumnWidth + GetColumnWidth(m_oHitInfo.nSubItemId);

    return;
}
