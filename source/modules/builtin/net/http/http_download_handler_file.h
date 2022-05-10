#pragma once

#include "base/com/com_class.h"
#include "modules/builtin/component_http_downloader.h"

class HttpDownloadHandlerFile :
    public IDownloadHandlerFile
{
public:
    HttpDownloadHandlerFile();
    virtual ~HttpDownloadHandlerFile();

    BOOL CreateDownloadFile(BSTR strFileName);

    COM_CLASS_FACTORY(HttpDownloadHandlerFile)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HttpDownloadHandlerFile)
            QI_MAPPING(IDownloadHandlerFile)
            QI_MAPPING(IDownloadHandler)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // IDownloadHandler
    STDMETHODIMP_(DWORD) GetResponseSize();
    STDMETHODIMP SetPreferredResponseSize(DWORD nResponseSize);
    STDMETHODIMP OnWriteStart(IDownload *pDownload);
    STDMETHODIMP OnWriteData(IDownload *pDownload, void *pBuffer, DWORD nDataSize, DWORD *pWriteSize);
    STDMETHODIMP OnWriteEnd(IDownload *pDownload, BOOL bOK);

    // IHttpDownloadHandler
    STDMETHODIMP GetResponseFile(BSTR *pFilePath);

private:
    CComBSTR m_strFileName;
};
