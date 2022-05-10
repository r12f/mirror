#pragma once

#include <vector>
#include <map>
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/builtin/component_log.h"

class CDlgLog :
    public CDialogImpl<CDlgLog>,
    public CLayout,
    public CUIControl,
    public ILogConsumer
{
public:
    enum { IDD = IDD_DIALOG_LOG };

#ifdef DEBUG
    enum { LOG_LOWER_BOUND = LOG_LEVEL_DEBUG };
#else
    enum { LOG_LOWER_BOUND = LOG_LEVEL_INFO };
#endif

    CDlgLog();
    virtual ~CDlgLog();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
        BEGIN_QI_MAP(CDlgLog)
            QI_MAPPING(ILogConsumer)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP OnLog(ILogRecord *pLogRecord);

    BEGIN_MSG_MAP_EX(CDlgLog)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_SET_LOG_SETTING, OnSetLogSettings)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnSetLogSettings(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

private:
    CEdit m_oEditLog;
};