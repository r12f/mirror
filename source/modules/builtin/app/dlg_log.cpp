#include "stdafx.h"
#include "modules/builtin/app/dlg_log.h"

static TCHAR *s_pLogLevelDesc[LOG_LEVEL_MAX] = {
    _T("DEBUG"),
    _T("INFO"),
    _T("WARN"),
    _T("ERROR"),
    _T("FATAL"),
};

CDlgLog::CDlgLog()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgLog::~CDlgLog()
{

}

HRESULT
CDlgLog::OnLog(ILogRecord *pLogRecord)
{
    CComBSTR strModule;
    CComBSTR strLogContent;
    DWORD nLogLevel = 0;
    pLogRecord->GetLevel(&nLogLevel);
    pLogRecord->GetModule(&strModule);
    pLogRecord->GetContent(&strLogContent);

    if(nLogLevel >= LOG_LEVEL_MAX) {
        return S_OK;
    }

    CString strLog;
    strLog.Format(_T("[%s][%s] %s\r\n"), s_pLogLevelDesc[nLogLevel], (LPCTSTR)strModule, (LPCTSTR)strLogContent);

    m_oEditLog.AppendText(strLog);

    return S_OK;
}

LRESULT
CDlgLog::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CComPtr<ILogCore> pCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    if(NULL != pCore) {
        pCore->RegisterConsumer(this);
    }

    m_oEditLog = GetDlgItem(IDC_EDIT_LOG_CONTENT);

    CComboBox oComboLogLevel = GetDlgItem(IDC_COMBO_SET_LOG_LEVEL);
    for(int i = LOG_LOWER_BOUND; i < LOG_LEVEL_MAX; ++i) {
        oComboLogLevel.AddString(s_pLogLevelDesc[i]);
    }

    DWORD nLogLevel = 0;
    CComPtr<ILogCore> pLogCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    pLogCore->GetLogLevel(&nLogLevel);

    oComboLogLevel.SetCurSel((int)nLogLevel - LOG_LOWER_BOUND);

    return TRUE;
}

VOID
CDlgLog::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgLog::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgLog::OnClose()
{
    SetMsgHandled(FALSE);

    CComPtr<ILogCore> pCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    if(NULL != pCore) {
        pCore->UnregisterConsumer(this);
    }

    DestroyWindow();

    return;
}

VOID
CDlgLog::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgLog::OnSetLogSettings(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CComboBox oComboLogLevel = GetDlgItem(IDC_COMBO_SET_LOG_LEVEL);
    int nLogLevel = oComboLogLevel.GetCurSel() + LOG_LOWER_BOUND;
    if(nLogLevel >= LOG_LEVEL_MAX) {
        MLOG_ERROR("Log", "错误的日志级别。");
        return;
    }

    CComPtr<ILogCore> pLogCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    pLogCore->SetLogLevel(nLogLevel);

    MLOG_INFO("Log", "设置日志级别成功。");

    return;
}
