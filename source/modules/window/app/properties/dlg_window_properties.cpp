#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_properties.h"
#include "modules/window/app/dlg_window_list.h"
#include "modules/builtin/component_config.h"
#include "modules/builtin/component_config_id.h"
#include "modules/component_id.h"
#include "modules/window/app/window_app_utils.h"

CButtonWindowPropertiesAutoRefresh::CButtonWindowPropertiesAutoRefresh()
{

}

CButtonWindowPropertiesAutoRefresh::~CButtonWindowPropertiesAutoRefresh()
{

}

VOID
CButtonWindowPropertiesAutoRefresh::BeforeContextMenuPopup(CMenuHandle &oMenu)
{
    BOOL bAutoRefresh = FALSE;
    UINT nAutoRefreshInterval = 0;
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL != pConfig) {
        pConfig->GetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_AUTO_REFRESH), &bAutoRefresh);
        pConfig->GetUInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_AUTO_REFRESH_INTERVAL), &nAutoRefreshInterval);
    }

    // 自动刷新
    oMenu.CheckMenuItem(ID_WINDOW_PROPERTIES_NO_AUTO_REFRESH, MF_BYCOMMAND | (!bAutoRefresh ? MF_CHECKED : MF_UNCHECKED));
    if(bAutoRefresh) {
        switch(nAutoRefreshInterval) {
        case 1000:
            oMenu.CheckMenuItem(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_SECOND, MF_BYCOMMAND | MF_CHECKED);
            break;
        case 3000:
            oMenu.CheckMenuItem(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_THREE_SECONDS, MF_BYCOMMAND | MF_CHECKED);
            break;
        case 5000:
            oMenu.CheckMenuItem(ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_FIVE_SECONDS, MF_BYCOMMAND | MF_CHECKED);
            break;
        }
    }

    return;
}


CDlgWindowProperties::CDlgWindowProperties()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_PROPERTIES, this);
}

CDlgWindowProperties::~CDlgWindowProperties()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_PROPERTIES);
}

BOOL
CDlgWindowProperties::PreTranslateMessage(MSG* pMsg)
{
    return ::IsDialogMessage(m_hWnd, pMsg);
}

LRESULT
CDlgWindowProperties::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    HWND hWatchWindow = GetWatchWindow();

    CString strTitle;
    strTitle.Format(_T("%08x - 详细信息"), hWatchWindow);
    SetWindowText(strTitle);

    m_oBtnAutoRefresh.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_PROPERTIES_AUTO_REFRESH));
    m_oBtnAutoRefresh.SetDropDownMenu(IDR_MENU_WINDOW_PROPERTIES_AUTO_REFRESH);
    m_oCheckTrackMode = GetDlgItem(IDC_CHECK_WINDOW_TRACK_MODE);

    CenterWindow();

    return TRUE;
}

VOID
CDlgWindowProperties::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowProperties::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowProperties::OnTimer(UINT_PTR nTimerId)
{
    switch(nTimerId) {
    case TIMER_ID_AUTO_REFRESH:
        Refresh();
        return;
    case TIMER_ID_TRACK_WINDOW:
        Refresh(TRUE, TRUE, REFRESH_PARAM_TRACK_MODE);
        return;
    }

    SetMsgHandled(FALSE);

    return;
}


VOID
CDlgWindowProperties::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowProperties::OnDestroy()
{
    SetMsgHandled(FALSE);

    m_oBtnAutoRefresh.UnsubclassWindow();
    m_oTabProperties.RemoveAllPages();
    m_oTabProperties.DestroyWindow();

    return;
}

VOID
CDlgWindowProperties::OnBtnRefresh(UINT nNotifyCode, UINT nId, HWND hCtrlWnd)
{
    Refresh();
}

VOID
CDlgWindowProperties::OnBtnMenuAutoRefresh(UINT nNotifyCode, UINT nId, HWND hCtrlWnd)
{
    BOOL bAutoRefresh = FALSE;
    UINT nAutoRefreshInterval = 0;
    BOOL bEnableTrackMode = FALSE;

    switch(nId) {
    case ID_WINDOW_PROPERTIES_NO_AUTO_REFRESH:
        bAutoRefresh = FALSE;
        nAutoRefreshInterval = 0;
        break;
    case ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_SECOND:
        bAutoRefresh = TRUE;
        nAutoRefreshInterval = 1 * 1000;
        break;
    case ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_THREE_SECONDS:
        bAutoRefresh = TRUE;
        nAutoRefreshInterval = 3 * 1000;
        break;
    case ID_WINDOW_PROPERTIES_AUTO_REFRESH_PER_FIVE_SECONDS:
        bAutoRefresh = TRUE;
        nAutoRefreshInterval = 5 * 1000;
        break;
    }

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL != pConfig) {
        pConfig->SetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_AUTO_REFRESH), bAutoRefresh);
        pConfig->SetUInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_AUTO_REFRESH_INTERVAL), nAutoRefreshInterval);
        pConfig->GetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_ENABLE_TRACK_MODE), &bEnableTrackMode);
    }

    KillTimer(TIMER_ID_AUTO_REFRESH);
    KillTimer(TIMER_ID_TRACK_WINDOW);

    if(!bAutoRefresh || nAutoRefreshInterval == 0) {
        return;
    }

    SetTimer(TIMER_ID_AUTO_REFRESH, nAutoRefreshInterval);

    if(bEnableTrackMode) {
        SetTimer(TIMER_ID_TRACK_WINDOW, 50, NULL);
    }

    return;
}

VOID
CDlgWindowProperties::OnCheckWindowTrackMode(UINT nNotifyCode, UINT nId, HWND hCtrlWnd)
{
    if(nNotifyCode != BN_CLICKED || IDC_CHECK_WINDOW_TRACK_MODE != nId) {
        SetMsgHandled(FALSE);
        return;
    }

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return;
    }

    BOOL bEnableTrackMode = m_oCheckTrackMode.GetCheck();
    pConfig->SetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_ENABLE_TRACK_MODE), bEnableTrackMode);

    BOOL bAutoRefresh = FALSE;
    pConfig->GetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_PROPERTIES_AUTO_REFRESH), &bAutoRefresh);

    if(bAutoRefresh && bEnableTrackMode) {
        SetTimer(TIMER_ID_TRACK_WINDOW, 50, NULL);
    } else {
        KillTimer(TIMER_ID_TRACK_WINDOW);
    }

    return;
}

HWND
CDlgWindowProperties::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;

    if(0 == strResId.CompareNoCase("IDC_TAB_WINDOW_PROPERTIES")) {
        m_oTabProperties.Create(m_hWnd);

        m_oWindowBasicInfo.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowBasicInfo, _T("基本信息"));

        m_oWindowClass.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowClass, _T("窗口类"));

        m_oWindowProp.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowProp, _T("窗口Prop"));

        m_oWindowOp.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowOp, _T("窗口操作"));

        m_oWindowInput.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowInput, _T("模拟输入"));

        m_oWindowDwm.Create(m_oTabProperties.m_tab);
        m_oTabProperties.AddPage(m_oWindowDwm, _T("DWM"));

        m_oTabProperties.SetActivePage(0);

        m_oTabProperties.ShowWindow(SW_SHOW);

        ProvideHotkey();

        return m_oTabProperties;
    }

    return NULL;
}

BOOL
CDlgWindowProperties::DoOperation(UINT nOpId, LPARAM lParam)
{
    switch(nOpId) {
    case UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND:
        SwitchToMe(COMPONENT_UI_DLG_WINDOW);
        SwitchToTabByHWND(m_oTabProperties, (HWND)lParam);
        break;
    }

    return TRUE;
}

BOOL
CDlgWindowProperties::RefreshChildren(BOOL bForceReload, DWORD nParam)
{
    m_oWindowBasicInfo.RequestRefresh(TRUE, bForceReload, nParam);

    if(REFRESH_PARAM_TRACK_MODE == nParam) {
        return TRUE;
    }

    m_oWindowClass.RequestRefresh(TRUE, bForceReload, nParam);
    m_oWindowProp.RequestRefresh(TRUE, bForceReload, nParam);
    m_oWindowInput.RequestRefresh(TRUE, bForceReload, nParam);
    m_oWindowDwm.RequestRefresh(TRUE, bForceReload, nParam);

    return TRUE;
}

VOID
CDlgWindowProperties::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_BASIC_INFO, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowBasicInfo.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_CLASS, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowClass.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_PROP, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowProp.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_OP, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowOp.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowInput.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oWindowDwm.m_hWnd },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}