#pragma once


class CTreeCtrlEx :
    public ATL::CWindowImpl<CTreeCtrlEx, CTreeViewCtrlEx>
{
public:
    CTreeCtrlEx();
    virtual ~CTreeCtrlEx();

    VOID SetContextMenu(UINT nMenuId);

    BEGIN_MSG_MAP_EX(CTreeCtrlExImpl)
        MSG_WM_RBUTTONDOWN(OnRButtonDown)
    END_MSG_MAP()

    VOID OnRButtonDown(UINT nFlag, WTL::CPoint oPoint);
    LRESULT OnCommand(UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
    virtual HMENU CreateContextMenu(HTREEITEM hMenuItem, UINT nHTFlags, WTL::CPoint oPoint);
    virtual VOID BeforeContextMenuPopup(CMenuHandle &oMenu);
    virtual VOID AfterContextMenuPopup(CMenuHandle &oMenu);

private:
    UINT m_nMenuId;
};