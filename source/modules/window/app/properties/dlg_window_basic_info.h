#pragma once

#include <list>
#include "base/kv/kv.h"
#include "modules/window/utils/style_translator.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"
#include "modules/window/app/properties/add_style_menu.h"

class CDlgWindowBasicInfo :
    public CDialogImpl<CDlgWindowBasicInfo>,
    public CLayout,
    public CUIControl
{
    enum {
        WBI_LIST_PID_INDEX               = 0,
        WBI_LIST_WINDOW_PROC_INDEX,
        WBI_LIST_RECT,
        WBI_LIST_CLIENT_RECT,
        WBI_LIST_USER_DATA,
        WBI_LIST_STYLE_INDEX,
        WBI_LIST_EXSTYLE_INDEX,
        WBI_LIST_PARENT_INDEX,
        WBI_LIST_OWNER_INDEX,
        WBI_LIST_CHILD_INDEX,
        WBI_LIST_HWNDPREV_INDEX,
        WBI_LIST_HWNDNEXT_INDEX,
        WBI_LIST_OPACITY_INDEX,
        WBI_LIST_FOCUS_INDEX,
        WBI_LIST_ACTIVE_INDEX,
        WBI_LIST_CAPTURE_INDEX,
        WBI_LIST_CARET_INDEX,
        WBI_LIST_FOREGROUND_INDEX,
        WBI_LIST_GLOBAL_FOCUS_INDEX,
        WBI_LIST_GLOBAL_ACTIVE_INDEX,
        WBI_LIST_GLOBAL_CAPTURE_INDEX,
        WBI_LIST_GLOBAL_CARET_INDEX,
        WBI_LIST_NUM,
    };

    enum {
        UIOP_WBI_TRACK_FOCUS              = 0x100,
        UIOP_WBI_TRACK_ACTIVE,
        UIOP_WBI_TRACK_CAPTURE,
        UIOP_WBI_TRACK_FOREGROUND,
        UIOP_WBI_TRACK_GLOBAL_FOCUS,
        UIOP_WBI_TRACK_GLOBAL_ACTIVE,
        UIOP_WBI_TRACK_GLOBAL_CAPTURE,
    };

public:
    enum { IDD = IDD_DIALOG_WINDOW_BASIC_INFO };

    CDlgWindowBasicInfo();
    virtual ~CDlgWindowBasicInfo();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowBasicInfo)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_MOVE_WINDOW, OnBtnMoveWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_SET_PARENT, OnBtnSetParent)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_PARENT, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_OWNER, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_CHILD, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_HWNDPREV, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_HWNDNEXT, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_FOCUS, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_ACTIVE, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_CAPTURE, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_CARET, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_FOREGROUND, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_GLOBAL_FOCUS, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_GLOBAL_ACTIVE, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_GLOBAL_CAPTURE, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_GOTO_GLOBAL_CARET, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_FOCUS, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_ACTIVE, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_CAPTURE, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_CARET, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_FOREGROUND, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_GLOBAL_FOCUS, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_GLOBAL_ACTIVE, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_GLOBAL_CAPTURE, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WBI_TRACK_GLOBAL_CARET, OnBtnTrackWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_REMOVE_STYLE, OnBtnRemoveStyle)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_REMOVE_EXSTYLE, OnBtnRemoveExStyle)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WBI_SET_LAYERED_WINDOW, OnBtnSetLayeredWindow)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnBtnMoveWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetParent(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetWatchWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnTrackWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnRemoveStyle(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnRemoveExStyle(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetLayeredWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

    BOOL UpdateWindowInfo(int nIndex, HWND hWindow);
    BOOL UpdateRectInfo(int nIndex, WTL::CRect *pRect);
    BOOL UpdateBasicInfo(int nIndex, CString &strInfo);
    BOOL UpdateWatchWindow(HWND hWindow);
    BOOL UpdateTrackWindow(UINT nCheckId);
    
    VOID ProvideHotkey();

private:
    CEdit m_vEditList[WBI_LIST_NUM];
    CListBox m_oListStyle;
    CListBox m_oListExStyle;
    AddStyleMenu m_oBtnAddStyle;
    AddStyleMenu m_oBtnAddExStyle;

    DWORD m_nPid;
    CString m_strExeName;
    DWORD m_nTid;
    CString m_strThreadModule;
    CString m_strClassName;
    BOOL m_bIsUnicode;
    LONG_PTR m_nWndProc;
    LONG_PTR m_nUserData;
    DWORD m_nStyle;
    StyleInfoList m_vStyles;
    DWORD m_nExStyle;
    StyleInfoList m_vExStyles;
    WTL::CRect m_oWindowRect;
    WTL::CRect m_oClientRect;
    HWND m_hParent;
    HWND m_hOwner;
    HWND m_hChild;
    HWND m_hHwndPrev;
    HWND m_hHwndNext;
    COLORREF m_nLayeredColor;
    BYTE m_nLayeredAlpha;
    DWORD m_nLayeredFlag;
    GUITHREADINFO m_oThreadInfo;
    HWND m_hForeground;
    GUITHREADINFO m_oForegroundThreadInfo;
};