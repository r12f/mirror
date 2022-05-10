#pragma once

#include <vector>
#include <map>
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/tree_ctrl_ex.h"
#include "modules/builtin/component_hotkey.h"

class CDlgConfigHotkey :
    public CDialogImpl<CDlgConfigHotkey>,
    public CLayout,
    public CUIControl
{
    typedef std::vector<CComPtr<IHotkey>> HotkeyList;
    typedef std::map<CString, HTREEITEM> HotkeyTreeItemList;

public:
    enum { IDD = IDD_DIALOG_CONFIG_HOTKEY };

    CDlgConfigHotkey();
    virtual ~CDlgConfigHotkey();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgConfigHotkey)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_NOTIFY(OnNotify)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_HOTKEY_APPLY, OnApplyHotkeySettings)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    LRESULT OnNotify(INT nId, LPNMHDR pNotify);
    VOID OnApplyHotkeySettings(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual BOOL Refresh(BOOL bRecursive, BOOL bForceReload, DWORD nParam);

protected:
    BOOL AddHotkeyToTree(IHotkey *pHotkey);
    BOOL EnsureSectionExist(CString &strSection, HTREEITEM *pTreeItem);
    HTREEITEM AddSectionToTree(HTREEITEM hParentTreeItem, CString &strSection);
    VOID UpdateHotkeyInput();
    CComPtr<IHotkey> GetSelectedHotkey();

private:
    CTreeCtrlEx m_oHotkeyTree;
    CHotKeyCtrl m_oHotkeySelected;
    CButton m_oBtnIsHotkeyGlobal;
    HotkeyTreeItemList m_vTreeItemList;
    HotkeyList m_vHotkeys;
};