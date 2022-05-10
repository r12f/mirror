#include "stdafx.h"
#include "modules/builtin/app/dlg_config_hotkey.h"
#include "modules/window/utils/window_utils.h"

#define HOTKEY_SECTION_SEPERATOR    _T('\\')

CDlgConfigHotkey::CDlgConfigHotkey()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgConfigHotkey::~CDlgConfigHotkey()
{

}

LRESULT
CDlgConfigHotkey::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oHotkeyTree.SubclassWindow(GetDlgItem(IDC_TREE_CONFIG_HOTKEY));
    m_oHotkeySelected = GetDlgItem(IDC_HOTKEY_CONFIG_HOTKEY_PICKER);
    m_oBtnIsHotkeyGlobal = GetDlgItem(IDC_CHECK_CONFIG_HOTKEY_GLOBAL);

    return TRUE;
}

VOID
CDlgConfigHotkey::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgConfigHotkey::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgConfigHotkey::OnClose()
{
    SetMsgHandled(FALSE);
    
    m_oHotkeyTree.UnsubclassWindow();

    DestroyWindow();

    return;
}

VOID
CDlgConfigHotkey::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

LRESULT
CDlgConfigHotkey::OnNotify(INT nId, LPNMHDR pNotify)
{
    SetMsgHandled(FALSE);

    if(TVN_SELCHANGED != pNotify->code || IDC_TREE_CONFIG_HOTKEY != nId) {
        return 0;
    }

    UpdateHotkeyInput();

    return 0;
}

VOID
CDlgConfigHotkey::OnApplyHotkeySettings(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CComPtr<IHotkey> pHotkey = GetSelectedHotkey();
    if(NULL == pHotkey) {
        return;
    }

    WORD nModifier = 0, nKey = 0;
    BOOL bGlobal = FALSE;
    m_oHotkeySelected.GetHotKey(nKey, nModifier);
    bGlobal = m_oBtnIsHotkeyGlobal.GetCheck();

    pHotkey->UnregisterHotkey();
    pHotkey->SetModifier(ConvertHotkeyFToMod(nModifier));
    pHotkey->SetKey(nKey);
    pHotkey->SetGlobal(bGlobal);
    pHotkey->RegisterHotkey();

    return;
}

BOOL
CDlgConfigHotkey::Refresh(BOOL bRecursive, BOOL bForceReload, DWORD nParam)
{
    m_oHotkeyTree.DeleteAllItems();

    CComPtr<IHotkeyManager> pHotMgr = GetComponentInterface<IHotkeyManager>(COMPONENT_HOTKEY);
    if(NULL == pHotMgr) {
        return FALSE;
    }

    DWORD nCount = 0;
    pHotMgr->GetHotkeyCount(&nCount);
    if(0 == nCount) {
        return FALSE;
    }

    m_vHotkeys.clear();
    m_vHotkeys.resize(nCount);
    pHotMgr->GetAllHotkey(&(m_vHotkeys[0].p), nCount);

    DWORD i = 0;
    for(i = 0; i < nCount; ++i) {
        AddHotkeyToTree(m_vHotkeys[i]);
    }

    HotkeyTreeItemList::iterator oTreeItemIt = m_vTreeItemList.begin();
    while(oTreeItemIt != m_vTreeItemList.end()) {
        m_oHotkeyTree.Expand(oTreeItemIt->second);
        ++oTreeItemIt;
    }

    oTreeItemIt = m_vTreeItemList.find(HOTKEY_SECTION_BASIC);
    if(oTreeItemIt != m_vTreeItemList.end()) {
        m_oHotkeyTree.SelectItem(oTreeItemIt->second);
    }

    return TRUE;
}

BOOL
CDlgConfigHotkey::AddHotkeyToTree(IHotkey *pHotkey)
{
    if(NULL == pHotkey) {
        return FALSE;
    }

    CComBSTR bstrSection;
    CComBSTR bstrName;
    UINT nModifier = 0;
    UINT nKey = 0;
    pHotkey->GetSection(&bstrSection);
    pHotkey->GetName(&bstrName);
    pHotkey->GetModifier(&nModifier);
    pHotkey->GetKey(&nKey);

    HTREEITEM hParentTreeItem = NULL;
    CString strSection = bstrSection;
    if(!EnsureSectionExist(strSection, &hParentTreeItem)) {
        return FALSE;
    }

    TVINSERTSTRUCT oItem = {0};
    oItem.hParent = hParentTreeItem;
    oItem.hInsertAfter = TVI_LAST;
    oItem.item.mask = TVIF_TEXT | TVIF_PARAM;
    oItem.item.pszText = bstrName;
    oItem.item.lParam = (LPARAM)pHotkey;
    if(!m_oHotkeyTree.InsertItem(&oItem)) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CDlgConfigHotkey::EnsureSectionExist(CString &strSection, HTREEITEM *pTreeItem)
{
    if(NULL != pTreeItem) {
        *pTreeItem = NULL;
    }

    HTREEITEM hTreeItem = NULL;
    HTREEITEM hParentTreeItem = TVI_ROOT;
    int nSectionSeperator = strSection.ReverseFind(HOTKEY_SECTION_SEPERATOR);
    if(nSectionSeperator != -1) {
        CString strParentSection = strSection.Left(nSectionSeperator);
        if(!EnsureSectionExist(strParentSection, &hParentTreeItem)) {
            return FALSE;
        }
    }

    CString strSubSection = strSection.Mid(nSectionSeperator + 1);
    hTreeItem = AddSectionToTree(hParentTreeItem, strSubSection);
    if(hTreeItem == NULL) {
        return FALSE;
    }

    if(NULL != pTreeItem) {
        *pTreeItem = hTreeItem;
    }

    return TRUE;
}

HTREEITEM
CDlgConfigHotkey::AddSectionToTree(HTREEITEM hParentTreeItem, CString &strSection)
{
    HotkeyTreeItemList::iterator oIt = m_vTreeItemList.find(strSection);
    if(oIt != m_vTreeItemList.end()) {
        return oIt->second;
    }

    HTREEITEM hTreeItem = NULL;
    TVINSERTSTRUCT oItem = {0};
    oItem.hParent = hParentTreeItem;
    oItem.hInsertAfter = TVI_LAST;
    oItem.item.mask = TVIF_TEXT | TVIF_PARAM;
    oItem.item.pszText = strSection.GetBuffer();
    oItem.item.lParam = (LPARAM)NULL;
    hTreeItem = m_oHotkeyTree.InsertItem(&oItem);
    if(NULL == hTreeItem) {
        return NULL;
    }

    m_vTreeItemList[strSection] = hTreeItem;

    return hTreeItem;
}

VOID
CDlgConfigHotkey::UpdateHotkeyInput()
{
    UINT nModifier = 0, nKey = 0;
    BOOL bGlobal = FALSE;

    CComPtr<IHotkey> pHotkey = GetSelectedHotkey();
    if(NULL != pHotkey) {
        pHotkey->GetModifier(&nModifier);
        pHotkey->GetKey(&nKey);
        pHotkey->GetGlobal(&bGlobal);
    }

    m_oHotkeySelected.SetHotKey(nKey, ConvertModToHotkeyF(nModifier));
    m_oBtnIsHotkeyGlobal.SetCheck(bGlobal);

    return;
}

CComPtr<IHotkey>
CDlgConfigHotkey::GetSelectedHotkey()
{
    CTreeItem oSelectItem = m_oHotkeyTree.GetSelectedItem();
    if(oSelectItem.IsNull()) {
        return NULL;
    }

    IHotkey *pHotkey = (IHotkey *)oSelectItem.GetData();
    if(NULL == pHotkey) {
        return NULL;
    }

    DWORD i = 0, nCount = (DWORD)m_vHotkeys.size();
    for(i = 0; i < nCount; ++i) {
        if(m_vHotkeys[i].p == pHotkey) {
            break;
        }
    }

    if(i == nCount) {
        return NULL;
    }

    return m_vHotkeys[i];
}
