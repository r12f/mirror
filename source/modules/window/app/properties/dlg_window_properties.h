#pragma once

#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/window/app/properties/dlg_window_basic_info.h"
#include "modules/window/app/properties/dlg_window_class.h"
#include "modules/window/app/properties/dlg_window_prop.h"
#include "modules/window/app/properties/dlg_window_op.h"
#include "modules/window/app/properties/dlg_window_input.h"
#include "modules/window/app/properties/dlg_window_dwm.h"

class CButtonWindowPropertiesAutoRefresh :
    public CMenuButton
{
public:
    CButtonWindowPropertiesAutoRefresh();
    virtual ~CButtonWindowPropertiesAutoRefresh();

protected:
    virtual VOID BeforeContextMenuPopup(CMenuHandle &oMenu);
};

class CDlgWindowProperties :
    public CDialogImpl<CDlgWindowProperties>,
    public CMessageFilter,
    public CLayout,
    public CUIControl
{
    enum {
        TIMER_ID_AUTO_REFRESH   = 100,
        TIMER_ID_TRACK_WINDOW   = 101,
    };

public:
    enum { IDD = IDD_DIALOG_WINDOW_PROPERTIES };

    CDlgWindowProperties();
    virtual ~CDlgWindowProperties();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP_EX(CDlgWindowProperties)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_PROPERTIES_REFRESH, OnBtnRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_PROPERTIES_NO_AUTO_REFRESH, OnBtnMenuAutoRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_SECOND, OnBtnMenuAutoRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_THREE_SECONDS, OnBtnMenuAutoRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_FIVE_SECONDS, OnBtnMenuAutoRefresh)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WINDOW_TRACK_MODE, OnCheckWindowTrackMode)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnTimer(UINT_PTR nTimerId);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnBtnRefresh(UINT nNotifyCode, UINT nId, HWND hCtrlWnd);
    VOID OnBtnMenuAutoRefresh(UINT nNotifyCode, UINT nId, HWND hCtrlWnd);
    VOID OnCheckWindowTrackMode(UINT nNotifyCode, UINT nId, HWND hCtrlWnd);

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL RefreshChildren(BOOL bForceReload, DWORD nParam);

protected:
    VOID ProvideHotkey();

private:
    CButtonWindowPropertiesAutoRefresh m_oBtnAutoRefresh;
    CEdit m_oEditAutoRefreshInterval;
    CButton m_oCheckTrackMode;
    CTabPanel m_oTabProperties;
    CDlgWindowBasicInfo m_oWindowBasicInfo;
    CDlgWindowClass m_oWindowClass;
    CDlgWindowProp m_oWindowProp;
    CDlgWindowOp m_oWindowOp;
    CDlgWindowInput m_oWindowInput;
    CDlgWindowDwm m_oWindowDwm;
};