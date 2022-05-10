#pragma once

#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/process/app/dlg_process_list.h"

class CDlgProcess :
    public CDialogImpl<CDlgProcess>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PROCESS };

    CDlgProcess();
    virtual ~CDlgProcess();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgProcess)
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
    virtual BOOL DoCustomLayout();

private:
    CTabPanel m_oTabProcess;
    CDlgProcessList m_oDlgProcessList;
};