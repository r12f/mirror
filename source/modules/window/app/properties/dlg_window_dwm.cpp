#include "stdafx.h"
#include "modules/builtin/component_hotkey_id.h"
#include "modules/window/utils/dwmapi_adapter.h"
#include "modules/window/app/properties/dlg_window_dwm.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindowDwm::CDlgWindowDwm()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_DWM, this);
}

CDlgWindowDwm::~CDlgWindowDwm()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_DWM);
}

LRESULT
CDlgWindowDwm::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oStaIsDwmEnabled = GetDlgItem(IDC_STATIC_IS_DWM_ENABLED);
    m_oBtnEnableDwmComposition = GetDlgItem(IDC_BUTTON_DWM_ENABLE_COMPOSITION);
    m_oStaDwmIsRemoteTransport = GetDlgItem(IDC_STATIC_DWM_IS_REMOTE_TRANSPORT);
    m_oStaDwmIsConnected = GetDlgItem(IDC_STATIC_DWM_CONNECT_STATUS);
    m_oStaDwmTransportGenerationValue = GetDlgItem(IDC_STATIC_DWM_TRANSPORT_GENERATION_VALUE);
    m_oStaDwmColorizationColor = GetDlgItem(IDC_STATIC_DWM_COLORIZATION_COLOR);

    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowDwm::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    m_oTabDwm.ShowWindow(SW_SHOW);
    return;
}

VOID
CDlgWindowDwm::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowDwm::OnBtnDWMEnableComposition(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    AdaDwmEnableComposition(m_bIsDwmEnabled ? DWM_EC_DISABLECOMPOSITION : DWM_EC_ENABLECOMPOSITION);
    RefreshAllWindowInfo();
    return;
}

HWND
CDlgWindowDwm::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;
    if(0 == strResId.CompareNoCase("IDC_TAB_WINDOW_DWM")) {
        m_oTabDwm.Create(m_hWnd);

        m_oDlgDwmOp.Create(m_oTabDwm.m_tab);
        m_oTabDwm.AddPage(m_oDlgDwmOp, _T("基本操作"));

        //m_oDlgDwmTiming.Create(m_oTabDwm.m_tab);
        //m_oTabDwm.AddPage(m_oDlgDwmTiming, _T("性能数据"));

        m_oTabDwm.SetActivePage(0);

        return m_oTabDwm;
    }

    return NULL;
}

BOOL
CDlgWindowDwm::DoOperation(UINT nOpId, LPARAM lParam)
{
    switch(nOpId) {
    case UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND:
        SwitchToMe(COMPONENT_UI_DLG_WINDOW_PROPERTIES);
        SwitchToTabByHWND(m_oTabDwm, (HWND)lParam);
        break;
    }

    return TRUE;
}

BOOL
CDlgWindowDwm::ResetData(DWORD nParam)
{
    m_bIsDwmEnabled = FALSE;
    m_bDwmIsRemoteTransport = FALSE;
    m_bDwmIsConnected = FALSE;
    m_nDwmTransportGenerationValue = 0;
    m_nDwmColorizationColor = 0;
    m_bOpaqueBlend = FALSE;

    return TRUE;
}

BOOL
CDlgWindowDwm::FetchData(DWORD nParam)
{
    AdaDwmIsCompositionEnabled(&m_bIsDwmEnabled);
    AdaDwmGetTransportAttributes(&m_bDwmIsRemoteTransport, &m_bDwmIsConnected, &m_nDwmTransportGenerationValue);
    AdaDwmGetColorizationColor(&m_nDwmColorizationColor, &m_bOpaqueBlend);

    return TRUE;
}

BOOL
CDlgWindowDwm::UpdateUI(DWORD nParam)
{
    CString strDwmColorizationColor;
    strDwmColorizationColor.Format(_T("A: %03lu, R: %03lu, G: %03lu, B: %03lu, Blend: %s"),
        ((m_nDwmColorizationColor & 0xFF000000) >> 24),
        ((m_nDwmColorizationColor & 0x00FF0000) >> 16),
        ((m_nDwmColorizationColor & 0x0000FF00) >> 8),
        ((m_nDwmColorizationColor & 0x000000FF)),
        m_bOpaqueBlend ? _T("on") : _T("off"));
    m_oStaDwmColorizationColor.SetWindowText(strDwmColorizationColor);

    m_oStaIsDwmEnabled.SetWindowText(m_bIsDwmEnabled ? _T("是") : _T("否"));
    m_oBtnEnableDwmComposition.SetWindowText(m_bIsDwmEnabled ? _T("禁用") : _T("启用"));
    m_oStaDwmIsRemoteTransport.SetWindowText(m_bDwmIsRemoteTransport ? _T("远程") : _T("本地"));
    m_oStaDwmIsConnected.SetWindowText(m_bDwmIsConnected ? _T("已连接") : _T("未连接"));

    CString strDwmTransportGenerationValue;
    strDwmTransportGenerationValue.Format(_T("%08x"), m_nDwmTransportGenerationValue);
    m_oStaDwmTransportGenerationValue.SetWindowText(strDwmTransportGenerationValue);

    return TRUE;
}

BOOL
CDlgWindowDwm::RefreshChildren(BOOL bForceReload, DWORD nParam)
{
    m_oDlgDwmOp.RequestRefresh(TRUE, bForceReload, nParam);
    m_oDlgDwmTiming.RequestRefresh(TRUE, bForceReload, nParam);

    return TRUE;
}

VOID
CDlgWindowDwm::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM_OP, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgDwmOp.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM_TIMING, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgDwmTiming.m_hWnd },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_ENABLE_COMPOSITION, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_DWM_ENABLE_COMPOSITION, 0},
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}