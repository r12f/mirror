#include "stdafx.h"
#include "modules/builtin/net/http/http_downloader.h"
#include "modules/builtin/net/http/http_download.h"
#include "modules/builtin/net/http/http_download_handler_buffer.h"

HttpDownloader::HttpDownloader()
{

}

HttpDownloader::~HttpDownloader()
{

}

HRESULT
HttpDownloader::DownloadToFile(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, BSTR strFilePath, IDownloadEventHandler *pEventHandler)
{
    if(NULL == ppDownload || NULL == strURL || NULL == strFilePath) {
        return E_INVALIDARG;
    }

    CComPtr<IHttpDownload> pDownload;
    if(FAILED(HttpDownload::CreateObject(__uuidof(IHttpDownload), (void **)&pDownload)) || NULL == pDownload) {
        return E_OUTOFMEMORY;
    }

    pDownload->SetURL(strURL);
    pDownload->SetRef(strRef);
    pDownload->SetEventHandler(pEventHandler);
    pDownload->SetDownloadHandler(NULL);

    *ppDownload = pDownload.Detach();

    return S_OK;
}

HRESULT
HttpDownloader::DownloadToBuffer(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, IDownloadEventHandler *pEventHandler)
{
    if(NULL == ppDownload || NULL == strURL) {
        return E_INVALIDARG;
    }

    CComPtr<IHttpDownload> pDownload;
    if(FAILED(HttpDownload::CreateObject(__uuidof(IHttpDownload), (void **)&pDownload)) || NULL == pDownload) {
        return E_OUTOFMEMORY;
    }

    HttpDownloadHandlerBuffer *pHandler = new HttpDownloadHandlerBuffer;
    if(NULL == pHandler) {
        return E_OUTOFMEMORY;
    }

    pDownload->SetURL(strURL);
    pDownload->SetRef(strRef);
    pDownload->SetEventHandler(pEventHandler);
    pDownload->SetDownloadHandler(pHandler);

    *ppDownload = pDownload.Detach();

    return S_OK;
}