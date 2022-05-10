#pragma once


class CEditableListCtrl :
    public ATL::CWindowImpl<CEditableListCtrl, CListViewCtrl>
{
public:
    struct HitInfo
    {
        WTL::CPoint m_oPoint;
        int nItemId;
        int nSubItemId;
        CRect m_oItemRect;
    };

public:
    CEditableListCtrl();
    virtual ~CEditableListCtrl();

    VOID SetContextMenu(UINT nMenuId);
    VOID ClearAllSelection();

    BEGIN_MSG_MAP_EX(CEditableListCtrl)
        MSG_WM_LBUTTONDBLCLK(OnMsgLButtonDblClk)
        MSG_WM_RBUTTONDOWN(OnMsgRButtonDown)
    END_MSG_MAP()

    VOID OnMsgLButtonDblClk(UINT nFlag, WTL::CPoint oPoint);
    VOID OnMsgRButtonDown(UINT nFlag, WTL::CPoint oPoint);

protected:
    virtual HMENU CreateContextMenu(HitInfo *pHitInfo);
    virtual VOID BeforeMenuPopup(HMENU oMenuHandle);
    virtual VOID AfterMenuPopup(HMENU oMenuHandle);
    virtual BOOL OnLButtonDblClk(HitInfo *pHitInfo);

protected:
    VOID MakeHitTestInfo(LVHITTESTINFO *pHitTestInfo);

private:
    UINT m_nMenuId;
    HitInfo m_oHitInfo;
};