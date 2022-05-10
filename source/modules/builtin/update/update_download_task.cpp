#include "stdafx.h"
#include "modules/builtin/update/update_download_task.h"
#include "base/string_number_convertions.h"
#include "modules/builtin/app/dlg_update_notify.h"
#include "modules/builtin/component_config.h"

CheckUpdateTask::CheckUpdateTask(BOOL bAutoUpdate)
    : m_bAutoUpdate(bAutoUpdate)
{

}

HRESULT
CheckUpdateTask::OnStart(IDownload *pDownload)
{
    MLOG_INFO("UpdateService", "正在检查更新...");
    return S_OK;
}

HRESULT
CheckUpdateTask::OnFinished(IDownload *pDownload)
{
    UINT nVerMajor = 0, nVerMinor = 0, nVerBuild = 0, nVerSpecial = 0;
    HRESULT hr = GetNewVersion(pDownload, &nVerMajor, &nVerMinor, &nVerBuild, &nVerSpecial);
    if(FAILED(hr)) {
        MLOG_INFO("UpdateService", "检查更新失败...");
        return hr;
    }

    if(!NeedUpdate(nVerBuild)) {
        MLOG_INFO("UpdateService", "当前版本已经是最新版本。");
        return S_OK;
    }

    if(ShouldSkipThisVersion(nVerBuild)) {
        MLOG_INFO("UpdateService", "发现新版本：%lu.%lu.%lu.%lu，跳过。", nVerMajor, nVerMinor, nVerBuild, nVerSpecial);
        return S_OK;
    }

    hr = GetWhatsNewTxt(nVerMajor, nVerMinor, nVerBuild, nVerSpecial);
    if(FAILED(hr)) {
        MLOG_INFO("UpdateService", "下载新版本信息失败...");
    }

    return hr;
}

HRESULT
CheckUpdateTask::OnError(IDownload *pDownload, HRESULT nResult, DWORD nErrCode)
{
    MLOG_INFO("UpdateService", "检查更新失败...");
    return S_OK;
}

BOOL
CheckUpdateTask::NeedUpdate(int nNextVersion)
{
    return nNextVersion > MIRROR_VERSION_BUILD;
}

HRESULT
CheckUpdateTask::GetNewVersion(IDownload *pDownload, UINT *pVerMajor, UINT *pVerMinor, UINT *pVerBuild, UINT *pVerSpecial)
{
    // 获取下载的内容
    CComPtr<IDownloadHandler> pHandler;
    if(FAILED(pDownload->GetDownloadHandler(&pHandler)) || NULL == pHandler) {
        return E_FAIL;
    }

    CComQIPtr<IDownloadHandlerBuffer> pBufferHandler = pHandler;
    if(NULL == pBufferHandler) {
        return E_FAIL;
    }

    char *pBuffer = NULL;
    if(FAILED(pBufferHandler->GetResponsePointer((void **)&pBuffer)) || NULL == pBuffer) {
        return E_FAIL;
    }

    // 解析加载的内容，观察是否需要更新
    CString strVersionInfo = pBuffer;
    if(!VersionStringToNumber(strVersionInfo, pVerMajor, pVerMinor, pVerBuild, pVerSpecial)) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT
CheckUpdateTask::GetWhatsNewTxt(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial)
{   
    CString strUpdateURL;
    strUpdateURL.Format(WHATS_NEW_URL,
        nVerMajor,
        nVerMinor,
        nVerBuild,
        nVerSpecial);

    CComPtr<IHttpDownloader> pDownloader = GetComponentInterface<IHttpDownloader>(COMPONENT_HTTP_DOWNLOADER);
    if(NULL == pDownloader) {
        return E_FAIL;
    }

    GetWhatsNewTask *pTask = new GetWhatsNewTask(nVerMajor, nVerMinor, nVerBuild, nVerSpecial);
    if(NULL == pTask) {
        return E_OUTOFMEMORY;
    }

    CComPtr<IDownloadEventHandler> pGetWhatsNewHandler = (IDownloadEventHandler *)pTask;
    pTask->Release();

    CComPtr<IHttpDownload> pGetWhatsNew;
    HRESULT hResult = pDownloader->DownloadToBuffer(&pGetWhatsNew, CComBSTR(strUpdateURL), NULL, pGetWhatsNewHandler);
    if(FAILED(hResult) || NULL == pGetWhatsNew) {
        return E_FAIL;
    }

    return pGetWhatsNew->Start();
}

BOOL
CheckUpdateTask::ShouldSkipThisVersion(UINT nVerBuild)
{
    if(!m_bAutoUpdate) {
        return FALSE;
    }

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    UINT nSkipBuild = 0;
    if(FAILED(pConfig->GetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_SKIP_VERSION), &nSkipBuild))) {
        return FALSE;
    }

    return nVerBuild <= nSkipBuild;
}

GetWhatsNewTask::GetWhatsNewTask(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial)
    : m_nVerMajor(nVerMajor)
    , m_nVerMinor(nVerMinor)
    , m_nVerBuild(nVerBuild)
    , m_nVerSpecial(nVerSpecial)
{

}

HRESULT
GetWhatsNewTask::OnStart(IDownload *pDownload)
{
    MLOG_INFO("UpdateService", "发现新版本：%lu.%lu.%lu.%lu，正在获取新版本信息...",
        m_nVerMajor, m_nVerMinor, m_nVerBuild, m_nVerSpecial);

    return S_OK;
}

HRESULT
GetWhatsNewTask::OnFinished(IDownload *pDownload)
{
    // 获取下载的内容
    CComPtr<IDownloadHandler> pHandler;
    if(FAILED(pDownload->GetDownloadHandler(&pHandler)) || NULL == pHandler) {
        return S_OK;
    }

    CComQIPtr<IDownloadHandlerBuffer> pBufferHandler = pHandler;
    if(NULL == pBufferHandler) {
        return S_OK;
    }

    char *pBuffer = NULL;
    if(FAILED(pBufferHandler->GetResponsePointer((void **)&pBuffer)) || NULL == pBuffer) {
        return S_OK;
    }

    MLOG_INFO("UpdateService", "检查更新完毕");

    CComBSTR strWhatsNew = pBuffer;

    // 弹出是否更新的窗口
    CDlgUpdateNotify oDlg;
    oDlg.SetVersion(m_nVerMajor, m_nVerMinor, m_nVerBuild, m_nVerSpecial);
    oDlg.SetWhatsNew(strWhatsNew);

    switch(oDlg.DoModal()) {
    case IDOK:
        {
            CString strDownloadUrl;
            strDownloadUrl.Format(DOWNLOAD_URL, m_nVerMajor, m_nVerMinor, m_nVerBuild, m_nVerSpecial);
            ShellExecute(NULL, _T("open"), strDownloadUrl, NULL, NULL, SW_SHOW);
        }
        break;
    case IDCANCEL:
        break;
    case IDC_BUTTON_SKIP_CURRENT_VERSION:
        SetSkipVersionToCurrent();
        break;
    }

    return S_OK;
}

HRESULT
GetWhatsNewTask::OnError(IDownload *pDownload, HRESULT nResult, DWORD nErrCode)
{
    MLOG_INFO("UpdateService", "下载新版本信息失败...");
    return S_OK;
}

void
GetWhatsNewTask::SetSkipVersionToCurrent()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL != pConfig) {
        pConfig->SetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_SKIP_VERSION), m_nVerBuild);
    }
}
