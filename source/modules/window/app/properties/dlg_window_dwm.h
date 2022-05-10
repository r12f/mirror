#pragma once

#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "modules/window/app/properties/dlg_window_dwm_op.h"
#include "modules/window/app/properties/dlg_window_dwm_timing.h"

class CDlgWindowDwm :
    public CDialogImpl<CDlgWindowDwm>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_DWM };

    CDlgWindowDwm();
    virtual ~CDlgWindowDwm();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowDwm)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_DWM_ENABLE_COMPOSITION, OnBtnDWMEnableComposition)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnBtnDWMEnableComposition(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);
    virtual BOOL RefreshChildren(BOOL bForceReload, DWORD nParam);

    VOID ProvideHotkey();

private:
    CStatic m_oStaIsDwmEnabled;
    CButton m_oBtnEnableDwmComposition;
    CStatic m_oStaDwmIsRemoteTransport;
    CStatic m_oStaDwmIsConnected;
    CStatic m_oStaDwmTransportGenerationValue;
    CStatic m_oStaDwmColorizationColor;

    CTabPanel m_oTabDwm;
    CDlgWindowDwmOp m_oDlgDwmOp;
    CDlgWindowDwmTiming m_oDlgDwmTiming;

    BOOL m_bIsDwmEnabled;
    BOOL m_bDwmIsRemoteTransport;
    BOOL m_bDwmIsConnected;
    DWORD m_nDwmTransportGenerationValue;
    DWORD m_nDwmColorizationColor;
    BOOL m_bOpaqueBlend;
};