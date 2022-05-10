#include "stdafx.h"
#include "modules/builtin/component_hotkey_id.h"
#include "modules/window/app/properties/dlg_window_prop.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/app/properties/dlg_window_modify_prop.h"

CDlgWindowProp::CDlgWindowProp()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_PROP, this);
}

CDlgWindowProp::~CDlgWindowProp()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_PROP);
}

LRESULT
CDlgWindowProp::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oListProps.SubclassWindow(GetDlgItem(IDC_LIST_WINDOW_PROP));
    m_oListProps.SetExtendedListViewStyle(m_oListProps.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_oListProps.ModifyStyle(0, LVS_SINGLESEL, 0);
    m_oListProps.SetContextMenu(IDR_MENU_WP_PROP_OP);

    LVCOLUMN oColumn = {0};
    oColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    oColumn.fmt = LVCFMT_LEFT;

    oColumn.pszText = _T("Atom?");
    oColumn.iSubItem = 0;
    oColumn.cx = 50;
    m_oListProps.InsertColumn(0, &oColumn);

    oColumn.pszText = _T("Prop Key");
    oColumn.iSubItem = 0;
    oColumn.cx = 270;
    m_oListProps.InsertColumn(1, &oColumn);

    oColumn.pszText = _T("Prop Value");
    oColumn.iSubItem = 0;
    oColumn.cx = 100;
    m_oListProps.InsertColumn(2, &oColumn);

    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowProp::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowProp::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowProp::OnDestroy()
{
    SetMsgHandled(FALSE);
    
    m_oListProps.UnsubclassWindow();

    return;
}

VOID
CDlgWindowProp::OnMenuCreateProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return;
    }

    CDlgWindowModifyProp oDlg;
    oDlg.SetDialogMethod(TRUE);
    if(IDOK != oDlg.DoModal()) {
        return;
    }

    CString strNewPropKey;
    HANDLE hNewPropValue = NULL;
    oDlg.GetInputProp(strNewPropKey, hNewPropValue);
    ::SetProp(oWindow.m_hWnd, strNewPropKey, hNewPropValue);

    Refresh();
}

VOID
CDlgWindowProp::OnMenuEditProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return;
    }

    int nIndex = m_oListProps.GetSelectedIndex();
    if(0 > nIndex) {
        return;
    }

    PropInfo *pPropInfo = (PropInfo *)m_oListProps.GetItemData(nIndex);
    if(NULL == pPropInfo) {
        return;
    }

    CDlgWindowModifyProp oDlg;
    oDlg.SetDialogMethod(FALSE);
    oDlg.SetInitProp(pPropInfo->m_bIsAtom, pPropInfo->m_strKey, pPropInfo->m_hValue);
    if(IDOK != oDlg.DoModal()) {
        return;
    }

    // 编辑的窗口Prop有可能本来是Atom，所以Key是不可更改的
    CString strNewPropKey;
    HANDLE hNewPropValue = NULL;
    oDlg.GetInputProp(strNewPropKey, hNewPropValue);

    if(pPropInfo->m_bIsAtom) {
        ::SetProp(oWindow.m_hWnd, pPropInfo->m_pAtomKey, hNewPropValue);
    } else {
        if(strNewPropKey != pPropInfo->m_strKey) {
            ::RemoveProp(oWindow.m_hWnd, pPropInfo->m_strKey);
        }
        ::SetProp(oWindow.m_hWnd, strNewPropKey, hNewPropValue);
    }

    Refresh();

    return;
}

VOID
CDlgWindowProp::OnMenuDeleteProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return;
    }

    int nIndex = m_oListProps.GetSelectedIndex();
    if(0 > nIndex) {
        return;
    }

    PropInfo *pPropInfo = (PropInfo *)m_oListProps.GetItemData(nIndex);
    if(NULL == pPropInfo) {
        return;
    }

    if(pPropInfo->m_bIsAtom) {
        ::RemoveProp(oWindow.m_hWnd, pPropInfo->m_pAtomKey);
    } else {
        ::RemoveProp(oWindow.m_hWnd, pPropInfo->m_strKey);
    }

    Refresh();
}

BOOL
CDlgWindowProp::ResetData(DWORD nParam)
{
    m_vProps.clear();
    return TRUE;
}

BOOL
CDlgWindowProp::FetchData(DWORD nParam)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return FALSE;
    }

    EnumPropsEx(oWindow.m_hWnd, (PROPENUMPROCEXW)PropEnumProcEx, (LPARAM)&m_vProps);

    return TRUE;
}

BOOL
CDlgWindowProp::UpdateUI(DWORD nParam)
{
    m_oListProps.SetRedraw(FALSE);

    m_oListProps.DeleteAllItems();

    int i = 0;
    PropInfoList::iterator oPropIt = m_vProps.begin();
    while(oPropIt != m_vProps.end()) {
        CString strValue;
        strValue.Format(_T("%08x"), oPropIt->m_hValue);

        m_oListProps.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM, i, _T(""), 0, 0, -1, (LPARAM)&(*oPropIt));
        m_oListProps.SetItemText(i, 0, oPropIt->m_bIsAtom ? _T("YES") : _T("NO"));
        m_oListProps.SetItemText(i, 1, oPropIt->m_strKey);
        m_oListProps.SetItemText(i, 2, strValue);

        ++oPropIt;
    }

    m_oListProps.SetRedraw(TRUE);

    return TRUE;
}

BOOL
CDlgWindowProp::IsWindowPropAtom(HANDLE hProp)
{
    if(hProp > (HANDLE)0xBFFF) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CDlgWindowProp::PropEnumProcEx(HWND hwnd, LPCTSTR lpszString, HANDLE hData, ULONG_PTR dwData)
{
    PropInfoList *pPropList = (PropInfoList *)dwData;
    
    PropInfo oInfo;

    if((LONG_PTR)lpszString <= 0xBFFF) {
        TCHAR szBuf[1024] = {0};
        GetAtomName((ATOM)lpszString, szBuf, sizeof(szBuf));
        oInfo.m_strKey = szBuf;
        oInfo.m_pAtomKey = lpszString;
        oInfo.m_bIsAtom = true;
    } else {
        oInfo.m_strKey = lpszString;
        oInfo.m_pAtomKey = NULL;
        oInfo.m_bIsAtom = false;
    }

    oInfo.m_hValue = hData;

    pPropList->push_back(oInfo);

    return TRUE;
}

VOID
CDlgWindowProp::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_WINDOW_PROP, HOTKEY_NAME_WINDOW_PROP_CREATE_PROP, m_hWnd, WM_COMMAND, (WPARAM)ID_WP_PROP_OP_CREATE_PROP, 0 },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}