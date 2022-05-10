#pragma once

#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/builtin/app/dlg_config_hotkey.h"
#include "modules/builtin/app/dlg_log.h"

class CDlgConfig :
    public CDialogImpl<CDlgConfig>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_CONFIG };

    CDlgConfig();
    virtual ~CDlgConfig();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgConfig)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

    virtual void OnFinalMessage(HWND);

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);

private:
    CTabPanel m_oTabConfig;
    CDlgConfigHotkey m_oDlgHotkey;
    CDlgLog m_oDlgLog;
};