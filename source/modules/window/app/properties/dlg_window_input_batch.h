#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/menu_button.h"

class CDlgWindowInputBatch :
    public CDialogImpl<CDlgWindowInputBatch>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_INPUT_BATCH };

    CDlgWindowInputBatch();
    virtual ~CDlgWindowInputBatch();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowInputBatch)
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
};