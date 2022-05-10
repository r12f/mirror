#pragma once

#include "modules/builtin/component_hotkey.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/window/app/dlg_window_list.h"
#include "modules/window/app/properties/dlg_window_properties.h"

class CDlgWindow :
    public CDialogImpl<CDlgWindow>,
    public CLayout,
    public CUIControl,
    public IHotkeyNotifyHandler
{
public:
    enum { IDD = IDD_DIALOG_WINDOW };

    enum {
        UIOP_WBI_TRACK_WATCH_WINDOW     = 0x100,
    };

    CDlgWindow();
    virtual ~CDlgWindow();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
        BEGIN_QI_MAP(CDlgWindow)
            QI_MAPPING(IHotkeyNotifyHandler)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override IHotkeyNotifyHandler
    STDMETHODIMP OnRegister(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey);
    STDMETHODIMP OnUnregister(BSTR strSection, BSTR strName, IHotkey *pHotkey);

    BEGIN_MSG_MAP_EX(CDlgWindow)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_LBUTTONDOWN(OnLButtonDown)
        MSG_WM_LBUTTONUP(OnLButtonUp)
        MSG_WM_MOUSEMOVE(OnMouseMove)
        MSG_WM_TIMER(OnTimer)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_SET_WATCH_WINDOW, OnBtnSetWatchWindow)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_SET_WINDOW_TITLE, OnBtnSetWindowTitle)
        COMMAND_ID_HANDLER_EX(IDC_CHECK_WINDOW_TRACK_WATCH_WINDOW, OnBtnTrackWindow)
    END_MSG_MAP()
    
protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnLButtonDown(UINT nFlag, WTL::CPoint oPoint);
    VOID OnLButtonUp(UINT nFlag, WTL::CPoint oPoint);
    VOID OnMouseMove(UINT nFlag, WTL::CPoint oPoint);
    VOID OnTimer(UINT_PTR nTimerId);
    VOID OnBtnSetWatchWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnSetWindowTitle(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnTrackWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);
    virtual BOOL RefreshChildren(BOOL bForceReload, DWORD nParam);

    void StartCaptureWatchWindow();
    BOOL StopCaptureWatchWindow();
    VOID CaptureWatchWindow(BOOL bHighlight = TRUE);
    VOID ProvideHotkey();
    BOOL UpdateTrackWindow(UINT nCheckId);
    BOOL GetWatchWindowHotkey(CString &strHotkey);

private:
    CTabPanel m_oTabWindow;
    CDlgWindowList m_oDlgWindowList;
    CDlgWindowProperties m_oDlgWindowProperties;
    BOOL m_bIsInCapture;
    HCURSOR m_hOldCursor;
    CStatic m_oPicCapture;
    CStatic m_oStaCaptureNotice;
    CEdit m_oEditWatchWindow;
    CEdit m_oEditWatchWindowOffsetX;
    CEdit m_oEditWatchWindowOffsetY;
    CEdit m_oEditWatchWindowClass;
    CEdit m_oEditWatchWindowTitle;
};