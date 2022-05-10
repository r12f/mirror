#pragma once

#include <vector>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/builtin/component_log.h"

class CDlgMain :
    public CDialogImpl<CDlgMain>,
    public CMessageFilter,
    public CLayout,
    public CUIControl,
    public ILogConsumer
{
    enum {
        MIRROR_MAIN_MENU_SET_TOPMOST        = 0x100,
        MIRROR_MAIN_MENU_ABOUT,
    };

    enum {
        TIMER_ID_CHECK_UPDATE   = 0x100,
        TIMER_ID_CLEAR_LOG,
    };

public:
    enum { IDD = IDD_DIALOG_MAIN };

    CDlgMain();
    virtual ~CDlgMain();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    STDMETHOD(OnLog)(ILogRecord *pLogRecord);

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP(CDlgMain)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
        MESSAGE_HANDLER(WM_SYSCOMMAND, OnSysCommand)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        MESSAGE_HANDLER(WM_TIMER, OnTimer)
        MESSAGE_HANDLER(WM_CLOSE, OnClose)
        MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
        NOTIFY_CODE_HANDLER(NM_CLICK, OnNMClickSyslink)
    END_MSG_MAP()
    
    LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNMClickSyslink(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/);

protected:
    virtual void OnFinalMessage(HWND hWnd);
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);

protected:
    void BuildMainTab();
    void OnSysMenuSetTopMost();
    void OnSysMenuAbout();
    void CheckUpdate();
    BOOL SaveLastWindowRect();

private:
    CTabPanel m_oTabMain;
    CStatusBarCtrl m_oStatusBar;
    CLinkCtrl m_oHomeLink;
    CLinkCtrl m_oReportLink;
};