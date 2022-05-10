#pragma once

#include "base/com/com_class.h"
#include "modules/ui_interface.h"
#include "ui/ui_engine/controls/tab_panel.h"

class CUIControl :
    public IUIControl
{
public:
    enum {
        NO_AUTO_RELOAD_DATA             = 1,
        NO_DELAY_RELOAD_DATA            = (1 << 1),
    };

public:
    CUIControl(CWindow *pWindow, DWORD nOptions = 0);
    virtual ~CUIControl();

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(CUIControl)
            QI_MAPPING(IUIControl)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(RequestDoOperation)(UINT nOpId, LPARAM lParam);
    STDMETHOD(RequestRefresh)(BOOL bRecursive = TRUE, BOOL bForceReload = TRUE, DWORD nParam = 0);

    BEGIN_MSG_MAP(CUIControl)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnMsgShowWindow)
        MESSAGE_HANDLER(WM_DESTROY, OnMsgDestroy)
        MESSAGE_HANDLER(WM_UI_CONTROL_DO_OPERATION, OnMsgDoOperation)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMsgShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMsgDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnMsgDoOperation(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL Refresh(BOOL bRecursive = TRUE, BOOL bForceReload = TRUE, DWORD nParam = 0);
    virtual BOOL BeforeRefresh();
    virtual BOOL AfterRefresh();
    virtual BOOL AfterRefreshChildren();
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);
    virtual BOOL RefreshChildren(BOOL bForceReload, DWORD nParam);
    
protected:
    BOOL SwitchToTabByHWND(CTabPanel &oTab, HWND hTabWindow);
    BOOL SwitchToTabByID(CTabPanel &oTab, INT nId);
    BOOL SwitchToMe(LPCTSTR pUIComponent);

private:
    CWindow *m_pWindow;
    BOOL m_bShouldRefresh;
    BOOL m_bShouldRefreshRecursive;
    DWORD m_nOptions;
};