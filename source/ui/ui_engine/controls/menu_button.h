#pragma once

// Menu Button Style
enum MenuButtonStyle {
    MBS_MENUONLY = 0,
    MBS_COMBO
};

class CMenuButton :
    public CWindowImpl<CMenuButton, CButton>
{
public:
    CMenuButton();
    CMenuButton(BOOL bHandleClick);
    virtual ~CMenuButton();

    BOOL SubclassWindow(HWND hWnd);
    VOID SetMenuButtonStyle(MenuButtonStyle nStyle) { m_nStyle = nStyle; }
    VOID SetDropDownMenu(UINT nMenuId) { m_nMenuId = nMenuId; }

    BEGIN_MSG_MAP_EX(CMenuButton)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_COMMAND(OnCommand)
        //MSG_WM_SETTEXT(OnSetText)
    END_MSG_MAP()

protected:
    VOID OnLButtonUp(UINT nFlag, WTL::CPoint oPoint);
    VOID OnCommand(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    //LRESULT OnSetText(LPCTSTR strText);

protected:
    virtual HMENU CreateContextMenu() { return NULL; }
    virtual VOID BeforeContextMenuPopup(CMenuHandle &oMenu) { return; }
    virtual VOID AfterContextMenuPopup(CMenuHandle &oMenu) { return; }
    virtual BOOL OnMenuItemClick(UINT nId) { return FALSE; }

private:
    CString m_strText;
    MenuButtonStyle m_nStyle;

    UINT m_nMenuId;
    BOOL m_bHandleClick;
};