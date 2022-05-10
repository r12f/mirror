#pragma once

#include <list>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"

class CDlgWindowProp :
    public CDialogImpl<CDlgWindowProp>,
    public CLayout,
    public CUIControl
{
    struct PropInfo {
        BOOL m_bIsAtom;
        CString m_strKey;
        LPCTSTR m_pAtomKey;
        HANDLE m_hValue;
    };

    typedef std::list<PropInfo> PropInfoList;

public:
    enum { IDD = IDD_DIALOG_WINDOW_PROP };

    CDlgWindowProp();
    virtual ~CDlgWindowProp();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowProp)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(ID_WP_PROP_OP_CREATE_PROP, OnMenuCreateProp)
        COMMAND_ID_HANDLER_EX(ID_WP_PROP_OP_EDIT_PROP, OnMenuEditProp)
        COMMAND_ID_HANDLER_EX(ID_WP_PROP_OP_DELETE_PROP, OnMenuDeleteProp)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnMenuCreateProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuEditProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuDeleteProp(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

    static BOOL IsWindowPropAtom(HANDLE hProp);
    static BOOL CALLBACK PropEnumProcEx(HWND hwnd, LPCTSTR lpszString, HANDLE hData, ULONG_PTR dwData);

    VOID ProvideHotkey();

private:
    CEditableListCtrl m_oListProps;
    PropInfoList m_vProps;
};