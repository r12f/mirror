#include "stdafx.h"
#include "app/dlg_main.h"
#include "app/dlg_about.h"
#include "modules/builtin/component_update_service.h"
#include "modules/builtin/component_config.h"

CDlgMain::CDlgMain()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_MAIN, (IUIControl *)this);

    CComPtr<ILogCore> pLogCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    if(NULL != pLogCore) {
        pLogCore->RegisterConsumer(this);
    }
}

CDlgMain::~CDlgMain()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_MAIN);

    CComPtr<ILogCore> pLogCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    if(NULL != pLogCore) {
        pLogCore->UnregisterConsumer(this);
    }
}

HRESULT
CDlgMain::OnLog(ILogRecord *pLogRecord)
{
    if(NULL == pLogRecord) {
        return E_INVALIDARG;
    }

    if(!m_oStatusBar.IsWindow()) {
        return S_OK;
    }

    KillTimer(TIMER_ID_CLEAR_LOG);

    CComBSTR strContent;
    pLogRecord->GetContent(&strContent);
    m_oStatusBar.SetText(0, strContent);

    SetTimer(TIMER_ID_CLEAR_LOG, 5000);

    return S_OK;
}

BOOL
CDlgMain::PreTranslateMessage(MSG* pMsg)
{
    return ::IsDialogMessage(m_hWnd, pMsg);
}

LRESULT
CDlgMain::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    m_oHomeLink = GetDlgItem(IDC_SYSLINK_DONATE);
    m_oReportLink = GetDlgItem(IDC_SYSLINK_REPORT);

    CMenuHandle oSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

    MENUITEMINFO oMenuItem = {0};
    oMenuItem.cbSize = sizeof(MENUITEMINFO);

    oMenuItem.fMask = MIIM_TYPE;
    oMenuItem.fType = MFT_SEPARATOR;
    oMenuItem.wID = 0;
    oSysMenu.InsertMenuItem(0, TRUE, &oMenuItem);

    oMenuItem.fMask = MIIM_ID | MIIM_STRING;
    oMenuItem.wID = MIRROR_MAIN_MENU_ABOUT;
    oMenuItem.dwTypeData = _T("关于(&A)");
    oSysMenu.InsertMenuItem(0, TRUE, &oMenuItem);

    oMenuItem.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
    oMenuItem.wID = MIRROR_MAIN_MENU_SET_TOPMOST;
    oMenuItem.dwTypeData = _T("置顶(&T)");
    oMenuItem.fState = MFS_CHECKED;
    oSysMenu.InsertMenuItem(0, TRUE, &oMenuItem);

    HICON hIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINFRAME));
    CIcon oOldIcon = (HICON)SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    SetTimer(TIMER_ID_CHECK_UPDATE, 3000);

    return TRUE;
}

LRESULT
CDlgMain::OnSysCommand(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL &bHandled)
{
    switch(wParam) {
    case MIRROR_MAIN_MENU_SET_TOPMOST:
        OnSysMenuSetTopMost();
        return 0;
    case MIRROR_MAIN_MENU_ABOUT:
        OnSysMenuAbout();
        return 0;
    }

    bHandled = FALSE;
    return 1;
}

LRESULT
CDlgMain::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    
    m_oTabMain.ShowWindow(SW_SHOW);

    BOOL bShowInTopmost = TRUE;
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL != pConfig) {
        if (FAILED(pConfig->GetBool(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_SHOW_IN_TOPMOST), &bShowInTopmost))) {
            pConfig->SetBool(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_SHOW_IN_TOPMOST), bShowInTopmost);
        }
    }

    if (bShowInTopmost) {
        SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    return 1;
}

LRESULT
CDlgMain::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    if(wParam == TIMER_ID_CHECK_UPDATE) {
        KillTimer(TIMER_ID_CHECK_UPDATE);
        CheckUpdate();
        return 0;
    } else if(wParam == TIMER_ID_CLEAR_LOG) {
        KillTimer(TIMER_ID_CLEAR_LOG);
        m_oStatusBar.SetText(0, _T(""));
        return 0;
    }

    return 1;
}

LRESULT
CDlgMain::OnClose(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    SaveLastWindowRect();
    DestroyWindow();
    return 1;
}

LRESULT
CDlgMain::OnDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    m_oTabMain.RemoveAllPages();
    m_oTabMain.DestroyWindow();
    m_oStatusBar.DestroyWindow();
    return 1;
}

void
CDlgMain::OnFinalMessage(HWND hWnd)
{
    ::PostQuitMessage(0);
}

BOOL
CDlgMain::DoOperation(UINT nOpId, LPARAM lParam)
{
    switch(nOpId) {
    case UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND:
        SwitchToTabByHWND(m_oTabMain, (HWND)lParam);
        break;
    }

    return TRUE;
}

HWND
CDlgMain::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;
    if(0 == strResId.CompareNoCase("IDC_MAIN_TAB")) {
        BuildMainTab();
        return m_oTabMain;
    } else if(0 == strResId.CompareNoCase("IDC_MAIN_STATUSBAR")) {
        m_oStatusBar.Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        return m_oStatusBar;
    }

    return NULL;
}

void
CDlgMain::BuildMainTab()
{
    m_oTabMain.Create(m_hWnd);

    DWORD nModuleCount = 0;
    std::vector<CComPtr<IMirModule>> vModules;
    GetFramework()->GetModulesCount(&nModuleCount);
    if(nModuleCount == 0) {
        return;
    }

    vModules.resize(nModuleCount);
    GetFramework()->GetModules(&(vModules[0]), nModuleCount);

    DWORD i = 0;
    HWND hTabHwnd = NULL;
    CComBSTR strTabTitle;
    for(i = 0; i < nModuleCount; ++i) {
        strTabTitle.Empty();
        hTabHwnd = NULL;

        if(FAILED(vModules[i]->RequestUI(m_oTabMain.m_tab, &strTabTitle, &hTabHwnd)) || strTabTitle == NULL || hTabHwnd == NULL) {
            continue;
        }

        m_oTabMain.AddPage(hTabHwnd, strTabTitle);
    }

    if(m_oTabMain.GetPageCount() > 0) {
        m_oTabMain.SetActivePage(0);
    }

    return;
}

VOID
CDlgMain::OnSysMenuSetTopMost()
{
    BOOL bTargetTopMostState = !(GetWindowLong(GWL_EXSTYLE) & WS_EX_TOPMOST);
    HWND hDesireZorder = bTargetTopMostState ? HWND_TOPMOST : HWND_NOTOPMOST;
    SetWindowPos(hDesireZorder, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if (NULL != pConfig) {
        pConfig->SetBool(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_SHOW_IN_TOPMOST), bTargetTopMostState);
    }

    CMenuHandle oSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

    MENUITEMINFO oInfo = {0};
    oInfo.cbSize = sizeof(MENUITEMINFO);
    oInfo.fMask = MIIM_STATE;
    oInfo.fState = bTargetTopMostState ? MFS_CHECKED : MFS_DEFAULT ;

    oSysMenu.SetMenuItemInfo(MIRROR_MAIN_MENU_SET_TOPMOST, FALSE, &oInfo);

    return;
}

VOID
CDlgMain::OnSysMenuAbout()
{
    CDlgAbout oDlgAbout;
    oDlgAbout.DoModal();
    return;
}

LRESULT CDlgMain::OnNMClickSyslink(int /*idCtrl*/, LPNMHDR pNMHDR, BOOL& /*bHandled*/)
{
    if(pNMHDR->hwndFrom == m_oHomeLink) {
        // https://me.alipay.com/bigasp
        ::ShellExecute(NULL, _T("open"), _T("https://me.alipay.com/bigasp"), NULL, NULL, SW_SHOW);
        return 0;
    } else if(pNMHDR->hwndFrom == m_oReportLink) {
        // https://bitbucket.org/bigasp/mirror/issues/new
        ::ShellExecute(NULL, _T("open"), _T("https://bitbucket.org/bigasp/mirror/issues/new"), NULL, NULL, SW_SHOW);
        return 0;
    }

    return 1;
}

void
CDlgMain::CheckUpdate()
{
    CComPtr<IUpdateService> pUpdateService = GetComponentInterface<IUpdateService>(COMPONENT_UPDATE_SERVICE);
    if(NULL == pUpdateService) {
        return;
    }

    if(FAILED(pUpdateService->CheckUpdate(TRUE))) {
        MLOG_INFO("UpdateService", "检查更新失败！ Error Code: %lu", ::GetLastError());
    }

    return;
}

BOOL
CDlgMain::SaveLastWindowRect()
{
    CRect oWindowRect;
    WINDOWPLACEMENT oPlacement = {0};
    GetWindowPlacement(&oPlacement);
    oWindowRect = oPlacement.rcNormalPosition;

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    UINT nIsMaxmized = (UINT)IsZoomed();

    CString strWindowPos;
    strWindowPos.Format(_T("%d,%d,%d,%d,%d"), oWindowRect.left, oWindowRect.top, oWindowRect.right, oWindowRect.bottom, nIsMaxmized);

    if(FAILED(pConfig->SetString(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_LAST_WINDOW_POSITION), CComBSTR(strWindowPos)))) {
        return FALSE;
    }

    return TRUE;
}
