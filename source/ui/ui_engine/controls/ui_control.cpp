#include "stdafx.h"
#include "ui/ui_engine/controls/ui_control.h"

CUIControl::CUIControl(CWindow *pWindow, DWORD nOptions)
    : m_pWindow(pWindow)
    , m_bShouldRefresh(TRUE)
    , m_bShouldRefreshRecursive(TRUE)
    , m_nOptions(nOptions)
{

}

CUIControl::~CUIControl()
{

}

HRESULT
CUIControl::RequestDoOperation(UINT nOpId, LPARAM lParam)
{
    if(!m_pWindow->IsWindow()) {
        return FALSE;
    }

    DoOperation(nOpId, lParam);

    return S_OK;
}

HRESULT
CUIControl::RequestRefresh(BOOL bRecursive, BOOL bForceReload, DWORD nParam)
{
    if(!m_pWindow->IsWindow()) {
        return E_FAIL;
    }

    if(!bForceReload && !m_pWindow->IsWindowVisible() && 0 == (m_nOptions & NO_DELAY_RELOAD_DATA) ) {
        m_bShouldRefresh = TRUE;
        m_bShouldRefreshRecursive = bRecursive;
        return TRUE;
    }

    Refresh(bRecursive, bForceReload, nParam);

    m_bShouldRefresh = FALSE;
    m_bShouldRefreshRecursive = FALSE;

    return S_OK;
}

LRESULT
CUIControl::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    ResetData(0);
    return 0;
}

LRESULT
CUIControl::OnMsgShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;

    if((1 == (m_nOptions & NO_AUTO_RELOAD_DATA)) || NULL == m_pWindow) {
        return TRUE;
    }

    if(m_bShouldRefresh) {
        Refresh(m_bShouldRefreshRecursive, FALSE, 0);
        m_bShouldRefresh = FALSE;
        m_bShouldRefreshRecursive = FALSE;
    }

    return TRUE;
}

LRESULT
CUIControl::OnMsgDoOperation(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = TRUE;
    DoOperation((UINT)wParam, lParam);
    return TRUE;
}

LRESULT
CUIControl::OnMsgDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    m_bShouldRefresh = TRUE;
    m_bShouldRefreshRecursive = TRUE;
    return TRUE;
}

BOOL
CUIControl::DoOperation( UINT nOpId, LPARAM lParam )
{
    return FALSE;
}

BOOL
CUIControl::Refresh(BOOL bRecursive, BOOL bForceReload, DWORD nParam)
{
    BeforeRefresh();

    ResetData(nParam);
    FetchData(nParam);
    UpdateUI(nParam);

    AfterRefresh();

    if(!bRecursive) {
        return TRUE;
    }

    RefreshChildren(bForceReload, nParam);

    AfterRefreshChildren();

    return TRUE;
}

BOOL
CUIControl::BeforeRefresh()
{
    return TRUE;
}

BOOL
CUIControl::AfterRefresh()
{
    return TRUE;
}

BOOL
CUIControl::AfterRefreshChildren()
{
    return TRUE;
}

BOOL
CUIControl::ResetData(DWORD nParam)
{
    return TRUE;
}

BOOL
CUIControl::FetchData(DWORD nParam)
{
    return TRUE;
}

BOOL
CUIControl::UpdateUI(DWORD nParam)
{
    return TRUE;
}

BOOL
CUIControl::RefreshChildren(BOOL bForceReload, DWORD nParam)
{
    return TRUE;
}

BOOL
CUIControl::SwitchToTabByHWND(CTabPanel &oTab, HWND hTabWindow)
{
    int nActivePage = oTab.GetActivePage();
    if(0 <= nActivePage) {
        if(hTabWindow == oTab.GetPageHWND(nActivePage)) {
            return TRUE;
        }
    }

    int i = 0, nPageCount = oTab.GetPageCount();
    for(i = 0; i < nPageCount; ++i) {
        if(oTab.GetPageHWND(i) == hTabWindow) {
            oTab.SetActivePage(i);
            break;
        }
    }

    return TRUE;
}

BOOL
CUIControl::SwitchToTabByID(CTabPanel &oTab, INT nId)
{
    HWND hPage = NULL;
    int nActivePage = oTab.GetActivePage();
    if(0 <= nActivePage) {
        hPage = oTab.GetPageHWND(nActivePage);
        int nDlgId = GetWindowLong(hPage, GWL_ID);
        if(nId == GetDlgCtrlID(hPage)) {
            return TRUE;
        }
    }

    int i = 0, nPageCount = oTab.GetPageCount();
    for(i = 0; i < nPageCount; ++i) {
        hPage = oTab.GetPageHWND(i);
        if(nId == GetDlgCtrlID(hPage)) {
            oTab.SetActivePage(i);
            break;
        }
    }

    return TRUE;
}

BOOL
CUIControl::SwitchToMe(LPCTSTR pUIComponent)
{
    if(NULL == pUIComponent) {
        return FALSE;
    }

    CComPtr<IUIControl> pUIWindow = GetComponentInterface<IUIControl>(pUIComponent);
    if(NULL == pUIWindow) {
        return FALSE;
    }

    return pUIWindow->RequestDoOperation(UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_pWindow->m_hWnd);
}