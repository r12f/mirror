#pragma once

#include "base/kv/kv.h"
#include "app/ui_base/layout.h"
#include "app/ui_base/ui_control.h"

class CDlgEtw :
    public CDialogImpl<CDlgEtw>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_ETW };

    CDlgEtw();
    virtual ~CDlgEtw();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgEtw)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

protected:
    virtual BOOL Layout();
};