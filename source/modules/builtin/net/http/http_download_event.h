#pragma once

#include "base/com/simple_com_class.h"
#include "modules/builtin/component_http_downloader.h"
#include "modules/builtin/component_thread_pool.h"
#include "modules/builtin/net/http/http_download.h"

class HttpDownloadStartEvent :
    public SimpleComClassT<HttpDownloadStartEvent, ComThreadSafeRef, IThreadPoolWorkItem>
{
public:
    HttpDownloadStartEvent(HttpDownload *pDownload)
        : m_pDownload(pDownload)
    {}

    virtual ~HttpDownloadStartEvent() {}

    STDMETHODIMP Run(IThreadPool *pThreadPool) {
        if(NULL != m_pDownload) {
            m_pDownload->CallHandlerOnStart();
        }
        return S_OK;
    }

private:
    CComPtr<HttpDownload> m_pDownload;
};

class HttpDownloadFinishEvent :
    public SimpleComClassT<HttpDownloadFinishEvent, ComThreadSafeRef, IThreadPoolWorkItem>
{
public:
    HttpDownloadFinishEvent(HttpDownload *pDownload)
        : m_pDownload(pDownload)
    {}

    virtual ~HttpDownloadFinishEvent() {}

    STDMETHODIMP Run(IThreadPool *pThreadPool) {
        if(NULL != m_pDownload) {
            m_pDownload->CallHandlerOnFinished();
        }
        return S_OK;
    }

private:
    CComPtr<HttpDownload> m_pDownload;
};

class HttpDownloadErrorEvent :
    public SimpleComClassT<HttpDownloadErrorEvent, ComThreadSafeRef, IThreadPoolWorkItem>
{
public:
    HttpDownloadErrorEvent(HttpDownload *pDownload, HRESULT nResult, DWORD nErrCode)
        : m_pDownload(pDownload), m_nResult(nResult), m_nErrCode(nErrCode)
    {}

    virtual ~HttpDownloadErrorEvent() {}

    STDMETHODIMP Run(IThreadPool *pThreadPool) {
        if(NULL != m_pDownload) {
            m_pDownload->CallHandlerOnError(m_nResult, m_nErrCode);
        }
        return S_OK;
    }

private:
    CComPtr<HttpDownload> m_pDownload;
    HRESULT m_nResult;
    DWORD m_nErrCode;
};
