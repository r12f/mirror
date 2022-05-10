#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"

class CDlgUpdateNotify:
    public CDialogImpl<CDlgUpdateNotify>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_UPDATE_NOTIFY };

    CDlgUpdateNotify();
    virtual ~CDlgUpdateNotify();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    VOID SetVersion(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial);
    VOID SetWhatsNew(LPCTSTR pWhatsNew);

    BEGIN_MSG_MAP_EX(CDlgUpdateNotify)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_SKIP_CURRENT_VERSION, OnSkipCurrentVersion)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnSkipCurrentVersion(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    
private:
    CStatic m_oStaNotify;
    CEdit m_oEditWhatsNew;

    UINT m_nVerMajor;
    UINT m_nVerMinor;
    UINT m_nVerBuild;
    UINT m_nVerSpecial;
    CString m_strWhatsNew;
};