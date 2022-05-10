#pragma once

#include "base/com/com_class.h"
#include "modules/builtin/component_http_downloader.h"

class HttpDownloader :
    public IHttpDownloader
{
public:
    HttpDownloader();
    virtual ~HttpDownloader();

    COM_CLASS_FACTORY(HttpDownloader)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HttpDownloader)
            QI_MAPPING(IHttpDownloader)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP DownloadToFile(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, BSTR strFilePath, IDownloadEventHandler *pEventHandler);
    STDMETHODIMP DownloadToBuffer(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, IDownloadEventHandler *pEventHandler);
};