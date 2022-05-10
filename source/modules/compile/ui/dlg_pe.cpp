#include "stdafx.h"
#include "dlg_pe.h"
#include "base/string_number_convertions.h"
#include "modules/builtin/component_config.h"

CDlgPE::CDlgPE()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgPE::~CDlgPE()
{

}

HRESULT
CDlgPE::AnalyzePE(LPCTSTR strFilePath)
{
    if (NULL == strFilePath) {
        return E_INVALIDARG;
    }

    LibPEPtr<IPEFile> pPEFile;
    HRESULT hr = LibPE::ParsePEFromDiskFile(strFilePath, &pPEFile);
    if (FAILED(hr)) {
        return hr;
    }

    m_strPEFilePath = strFilePath;
    m_pPEFile = pPEFile;

    m_oDlgPEBasic.SetPEFile(pPEFile);

    return S_OK;
}

LRESULT
CDlgPE::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    // Set title
    CString strTitle;
    strTitle.Format(_T("PE分析: %s"), m_strPEFilePath);
    SetWindowText(strTitle);

    // Set Icon
    HICON hIcon = ::LoadIcon(::GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_MAINFRAME));
    CIcon oOldIcon = (HICON)SendMessage(WM_SETICON, ICON_SMALL, (LPARAM)hIcon);

    // Set topmost
    BOOL bIsShowInTopmost = TRUE;
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if (NULL != pConfig) {
        pConfig->GetBool(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_SHOW_IN_TOPMOST), &bIsShowInTopmost);
    }

    if (bIsShowInTopmost) {
        SetWindowPos(HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    }

    // Update system menu
    CMenuHandle oSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

    MENUITEMINFO oMenuItem = {0};
    oMenuItem.cbSize = sizeof(MENUITEMINFO);

    oMenuItem.fMask = MIIM_TYPE;
    oMenuItem.fType = MFT_SEPARATOR;
    oMenuItem.wID = 0;
    oSysMenu.InsertMenuItem(0, TRUE, &oMenuItem);

    oMenuItem.fMask = MIIM_ID | MIIM_STRING | MIIM_STATE;
    oMenuItem.wID = MIRROR_DIALOG_PE_MENU_SET_TOPMOST;
    oMenuItem.dwTypeData = _T("置顶(&T)");
    oMenuItem.fState = bIsShowInTopmost ? MFS_CHECKED : MFS_DEFAULT;
    oSysMenu.InsertMenuItem(0, TRUE, &oMenuItem);

    return TRUE;
}

VOID
CDlgPE::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);

    switch (nId) {
    case MIRROR_DIALOG_PE_MENU_SET_TOPMOST:
        OnSysMenuSetTopMost();
        break;
    default:
        return;
    }

    SetMsgHandled(TRUE);

    return;
}

VOID
CDlgPE::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPE::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgPE::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPE::OnSysMenuSetTopMost()
{
    BOOL bTargetTopMostState = !(GetWindowLong(GWL_EXSTYLE) & WS_EX_TOPMOST);
    HWND hDesireZorder = bTargetTopMostState ? HWND_TOPMOST : HWND_NOTOPMOST;
    SetWindowPos(hDesireZorder, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

    CMenuHandle oSysMenu = ::GetSystemMenu(m_hWnd, FALSE);

    MENUITEMINFO oInfo = {0};
    oInfo.cbSize = sizeof(MENUITEMINFO);
    oInfo.fMask = MIIM_STATE;
    oInfo.fState = bTargetTopMostState ? MFS_CHECKED : MFS_DEFAULT ;

    oSysMenu.SetMenuItemInfo(MIRROR_DIALOG_PE_MENU_SET_TOPMOST, FALSE, &oInfo);

    return;
}

void
CDlgPE::OnFinalMessage(_In_ HWND /*hWnd*/)
{
    delete this;
}

HWND
CDlgPE::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;

    if(0 == strResId.CompareNoCase("IDC_TAB_PE_INFO")) {
        m_oTabPEInfo.Create(m_hWnd);

        m_oDlgPEBasic.Create(m_oTabPEInfo.m_tab);
        m_oTabPEInfo.AddPage(m_oDlgPEBasic, _T("基本信息"));

        m_oTabPEInfo.SetActivePage(0);

        m_oTabPEInfo.ShowWindow(SW_SHOW);

        return m_oTabPEInfo;
    }

    return NULL;
}