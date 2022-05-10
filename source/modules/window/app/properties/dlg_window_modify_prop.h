#pragma once

#include <list>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"

class CDlgWindowModifyProp :
    public CDialogImpl<CDlgWindowModifyProp>,
    public CLayout,
    public CUIControl
{
    struct PropInfo {
        BOOL m_bIsAtom;
        CString m_strKey;
        HANDLE m_hValue;
    };

    typedef std::list<PropInfo> PropInfoList;

public:
    enum { IDD = IDD_DIALOG_WINDOW_MODIFY_PROP };

    CDlgWindowModifyProp();
    virtual ~CDlgWindowModifyProp();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    VOID SetDialogMethod(BOOL bUseForAdd);
    VOID SetInitProp(BOOL bIsAtomProp, CString &strPropKey, HANDLE hPropValue);
    VOID GetInputProp(CString &strPropKey, HANDLE &hPropValue);

    BEGIN_MSG_MAP_EX(CDlgWindowModifyProp)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

private:
    BOOL m_bIsAddPropDialog;
    BOOL m_bIsAtomProp;
    CString m_strPropKey;
    HANDLE m_hPropValue;
    CEdit m_oPropKey;
    CEdit m_oPropValue;
};