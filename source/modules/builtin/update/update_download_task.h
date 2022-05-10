#pragma once

#include "base/com/simple_com_class.h"
#include "modules/builtin/component_http_downloader.h"

#define UPDATE_URL      _T("http://bigasp.com/projects/check_update.php?product=mirror&version=%lu.%lu.%lu.%lu")
#define WHATS_NEW_URL   _T("http://bigasp.com/projects/mirror/%lu.%lu.%lu.%lu/whats_new.txt")
#define DOWNLOAD_URL    _T("https://bitbucket.org/bigasp/mirror")

class CheckUpdateTask :
    public SimpleComClassT<CheckUpdateTask, ComRef, IDownloadEventHandler>
{
public:
    CheckUpdateTask(BOOL bAutoUpdate);

    // Override IDownloadEventHandler
    STDMETHODIMP OnStart(IDownload *pDownload);
    STDMETHODIMP OnFinished(IDownload *pDownload);
    STDMETHODIMP OnError(IDownload *pDownload, HRESULT nResult, DWORD nErrCode);

protected:
    BOOL NeedUpdate(int nNextVersion);
    HRESULT GetNewVersion(IDownload * pDownload, UINT *pVerMajor, UINT *pVerMinor, UINT *pVerBuild, UINT *pVerSpecial);
    HRESULT GetWhatsNewTxt(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial);
    BOOL ShouldSkipThisVersion(UINT nVerBuild);

private:
    BOOL m_bAutoUpdate;
};

class GetWhatsNewTask :
    public SimpleComClassT<GetWhatsNewTask, ComRef, IDownloadEventHandler>
{
public:
    GetWhatsNewTask(UINT nVerMajor, UINT nVerMinor, UINT nVerBuild, UINT nVerSpecial);

    // Override IDownloadEventHandler
    STDMETHODIMP OnStart(IDownload *pDownload);
    STDMETHODIMP OnFinished(IDownload *pDownload);
    STDMETHODIMP OnError(IDownload *pDownload, HRESULT nResult, DWORD nErrCode);

protected:
    void SetSkipVersionToCurrent();

private:
    UINT m_nVerMajor;
    UINT m_nVerMinor;
    UINT m_nVerBuild;
    UINT m_nVerSpecial;
};