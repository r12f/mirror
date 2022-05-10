#include "stdafx.h"
#include "modules/builtin/app/dlg_config.h"

CDlgConfig::CDlgConfig()
    : CLayout(IDD)
    , CUIControl(this)
{
    
}

CDlgConfig::~CDlgConfig()
{

}

LRESULT
CDlgConfig::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    return TRUE;
}

VOID
CDlgConfig::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgConfig::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgConfig::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgConfig::OnDestroy()
{
    SetMsgHandled(FALSE);

    m_oTabConfig.RemoveAllPages();
    m_oTabConfig.DestroyWindow();

    return;
}

void
CDlgConfig::OnFinalMessage(HWND)
{
    return;
}

HWND
CDlgConfig::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;

    if(0 == strResId.CompareNoCase("IDC_TAB_CONFIG")) {
        m_oTabConfig.Create(m_hWnd);

        m_oDlgHotkey.Create(m_oTabConfig.m_tab);
        m_oTabConfig.AddPage(m_oDlgHotkey, _T("热键设置"));

        m_oDlgLog.Create(m_oTabConfig.m_tab);
        m_oTabConfig.AddPage(m_oDlgLog, _T("程序日志"));

        m_oTabConfig.SetActivePage(0);

        m_oTabConfig.ShowWindow(SW_SHOW);

        return m_oTabConfig;
    }

    return NULL;
}