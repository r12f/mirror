#include "stdafx.h"
#include "mirror_version.h"
#include "base/log/log.h"
#include "modules/builtin/component_config.h"
#include "modules/builtin/update/update_service.h"
#include "modules/builtin/update/update_download_task.h"

UpdateService::UpdateService()
{

}

UpdateService::~UpdateService()
{

}

HRESULT
UpdateService::CheckUpdate(BOOL bAutoCheck)
{
    if(bAutoCheck && !NeedCheckUpdate()) {
        return S_OK;
    }

    CString strUpdateURL;
    strUpdateURL.Format(UPDATE_URL,
        MIRROR_VERSION_MAJOR,
        MIRROR_VERSION_MINOR,
        MIRROR_VERSION_BUILD,
        MIRROR_VERSION_SPECIAL);

    CComPtr<IHttpDownloader> pDownloader = GetComponentInterface<IHttpDownloader>(COMPONENT_HTTP_DOWNLOADER);
    if(NULL == pDownloader) {
        return E_FAIL;
    }

    CheckUpdateTask *pTask = new CheckUpdateTask(bAutoCheck);
    if(NULL == pTask) {
        return E_OUTOFMEMORY;
    }

    CComPtr<IDownloadEventHandler> pHandler = pTask;
    pTask->Release();

    CComPtr<IHttpDownload> pDownload;
    HRESULT hResult = pDownloader->DownloadToBuffer(&pDownload, CComBSTR(strUpdateURL), NULL, pHandler);
    if(FAILED(hResult) || NULL == pDownload) {
        return E_FAIL;
    }

    return pDownload->Start();
}

BOOL
UpdateService::NeedCheckUpdate()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    FILETIME nLastCheckFileTime = {0}, nNowTimeFileTime = {0}, nDayStartFileTime = {0};
    pConfig->GetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_LAST_CHECK_TIME_HIGH), (UINT *)&(nLastCheckFileTime.dwHighDateTime));
    pConfig->GetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_LAST_CHECK_TIME_LOW), (UINT *)&(nLastCheckFileTime.dwLowDateTime));
    
    SYSTEMTIME nNowTimeSysTime = {0};
    GetSystemTime(&nNowTimeSysTime);
    SystemTimeToFileTime(&nNowTimeSysTime, &nNowTimeFileTime);

    nNowTimeSysTime.wHour = 0;
    nNowTimeSysTime.wMinute = 0;
    nNowTimeSysTime.wSecond = 0;
    nNowTimeSysTime.wMilliseconds = 0;
    SystemTimeToFileTime(&nNowTimeSysTime, &nDayStartFileTime);

    unsigned __int64 nLastCheck = *((unsigned __int64 *)&nLastCheckFileTime);
    unsigned __int64 nDayStart = *((unsigned __int64 *)&nDayStartFileTime);
    if(nLastCheck > nDayStart) {
        return FALSE;
    }

    pConfig->SetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_LAST_CHECK_TIME_HIGH), nNowTimeFileTime.dwHighDateTime);
    pConfig->SetUInt(CComBSTR(CONFIG_SECTION_UPDATE_SERVICE), CComBSTR(CONFIG_KEY_LAST_CHECK_TIME_LOW), nNowTimeFileTime.dwLowDateTime);

    return TRUE;
}