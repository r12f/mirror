#pragma once

#include <WinInet.h>
#include "base/com/com_class.h"
#include "modules/builtin/component_http_downloader.h"
#include "modules/builtin/component_thread_pool.h"

class HttpDownload :
    public IHttpDownload,
    public IThreadPoolWorkItem
{
    enum {
        DEFAULT_RECV_BUF_SIZE   = 1024,
    };

public:
    HttpDownload();
    virtual ~HttpDownload();

    void CallHandlerOnStart();
    void CallHandlerOnFinished();
    void CallHandlerOnError(HRESULT nResult, DWORD nErrCode);

    COM_CLASS_FACTORY(HttpDownload)
    MTA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HttpDownload)
            QI_MAPPING(IHttpDownload)
            QI_MAPPING(IDownload)
            QI_MAPPING(IThreadPoolWorkItem)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Overload IDownload
    STDMETHODIMP Start();
    STDMETHODIMP Stop();
    STDMETHODIMP_(DWORD) GetState();
    STDMETHODIMP_(DWORD) GetId();
    STDMETHODIMP GetURL(BSTR *pURL);
    STDMETHODIMP SetURL(BSTR strURL);
    STDMETHODIMP GetEventHandler(IDownloadEventHandler **ppHandler);
    STDMETHODIMP SetEventHandler(IDownloadEventHandler *pHandler);
    STDMETHODIMP GetDownloadHandler(IDownloadHandler **ppHandler);
    STDMETHODIMP SetDownloadHandler(IDownloadHandler *pHandler);

    // Overload IHttpDownload
    STDMETHODIMP GetMethod(DWORD *pMethod);
    STDMETHODIMP SetMethod(DWORD nMethod);
    STDMETHODIMP GetRef(BSTR *pRef);
    STDMETHODIMP SetRef(BSTR strRef);
    STDMETHODIMP GetUserAgent(BSTR *pUA);
    STDMETHODIMP SetUserAgent(BSTR strUA);
    STDMETHODIMP GetHeaderMap(IKVMap **ppHeaderMap);
    STDMETHODIMP SetHeaderMap(IKVMap *pHeaderMap);

    // Overload IThreadPoolWorkItem
    STDMETHODIMP Run(IThreadPool *pThreadPool);

protected:
    DWORD SetState(DWORD nNewState, IThreadPool *pThreadPool = NULL, HRESULT nResult = S_OK, DWORD nErrCode = 0);

    HRESULT RunDownload();
    HINTERNET CreateHttpRequest(HINTERNET hConnect);
    HRESULT SendHttpRequest(HINTERNET hRequest);
    HRESULT ReceiveHttpResponse(HINTERNET hRequest);
    BOOL ShouldQuitDownload();

private:
    DWORD m_nId;
    DWORD m_nBirthTid;
    DWORD m_nTaskState;
    DWORD m_nMethod;
    CComBSTR m_strURL;
    CComBSTR m_strRef;
    CComBSTR m_strUA;
    CComPtr<IKVMap> m_pHeaderMap;
    CComPtr<IDownloadEventHandler> m_pEventHandler;
    CComPtr<IDownloadHandler> m_pDownloadHandler;

    INTERNET_SCHEME m_nReqScheme;
    CString m_strReqUserName;
    CString m_strReqPassword;
    CString m_strReqHost;
    INTERNET_PORT m_nReqPort;
    CString m_strReqPath;
};