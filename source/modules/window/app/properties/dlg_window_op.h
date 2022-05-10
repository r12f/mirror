#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/menu_button.h"
#include "ui/ui_engine/controls/filter_combo_box_ex.h"

class CDlgWindowOp :
    public CDialogImpl<CDlgWindowOp>,
    public CLayout,
    public CUIControl
{
    typedef std::map<CString, DWORD> WMList;

public:
    enum { IDD = IDD_DIALOG_WINDOW_OP };

    CDlgWindowOp();
    virtual ~CDlgWindowOp();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowOp)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_SEND_MESSAGE, OnSendMessage)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_POST_MESSAGE, OnPostMessage)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOW, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWNORMAL, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWDEFAULT, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_MAXIMIZE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWMAXIMIZED, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_MINIMIZE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWMINIMIZED, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWMINNOACTIVE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_FORCEMINIMIZE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_RESTORE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWNA, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_SHOWNOACTIVATE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SHOW_WINDOW_SW_HIDE, OnBtnShowWindowMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_ENABLE_WINDOW_TRUE, OnBtnEnableWindow)
        COMMAND_ID_HANDLER_EX(ID_WI_ENABLE_WINDOW_FALSE, OnBtnEnableWindow)
        COMMAND_ID_HANDLER_EX(ID_WI_ZORDER_TOPMOST, OnBtnZorderMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_ZORDER_TOP, OnBtnZorderMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_ZORDER_BOTTOM, OnBtnZorderMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_CLOSE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_CONTEXTHELP, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_DEFAULT, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_HOTKEY, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_HSCROLL, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_KEYMENU, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_MAXIMIZE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_MINIMIZE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_MONITORPOWER, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_MOUSEMENU, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_MOVE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_NEXTWINDOW, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_PREVWINDOW, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_RESTORE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_SCREENSAVE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_SIZE, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_TASKLIST, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(ID_WI_SYSCOMMAND_SC_VSCROLL, OnBtnSysCommandMenu)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_SET_FOCUS, OnBtnSetFocus)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_SET_ACTIVE, OnBtnSetActive)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_SET_FOREGROUND, OnBtnSetForeground)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WI_PARSE_WAIT_CHAIN, OnBtnParseWaitChain)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnSendMessage(UINT nNotifyCode, UINT nId, HWND hCtrlWnd);
    VOID OnPostMessage(UINT nNotifyCode, UINT nId, HWND hCtrlWnd);
    VOID OnBtnShowWindowMenu(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnEnableWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnZorderMenu(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSysCommandMenu(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetFocus(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetActive(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetForeground(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnParseWaitChain(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID SendMessageToWindow(BOOL bSync);

protected:
    VOID ProvideHotkey();

private:
    CFilterComboBoxEx m_oComboMessages;
    CEdit m_oEditWParam;
    CEdit m_oEditLParam;
    WMList m_vWindowMessages;

    CMenuButton m_oBtnShowWindow;
    CMenuButton m_oBtnEnableWindow;
    CMenuButton m_oBtnModifyZorder;
    CMenuButton m_oBtnSysCommand;
    CButton m_oBtnSetFocus;
    CButton m_oBtnSetActive;
    CButton m_oBtnSetForeground;
};
