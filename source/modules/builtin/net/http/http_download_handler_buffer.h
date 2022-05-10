#pragma once

#include <vector>
#include "base/com/com_class.h"
#include "modules/builtin/component_http_downloader.h"

class HttpDownloadHandlerBuffer :
    public IDownloadHandlerBuffer
{
public:
    HttpDownloadHandlerBuffer();
    virtual ~HttpDownloadHandlerBuffer();

    COM_CLASS_FACTORY(HttpDownloadHandlerBuffer)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HttpDownloadHandlerBuffer)
            QI_MAPPING(IDownloadHandlerBuffer)
            QI_MAPPING(IDownloadHandler)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // IDownloadHandler
    STDMETHODIMP_(DWORD) GetResponseSize();
    STDMETHODIMP SetPreferredResponseSize(DWORD nResponseSize);
    STDMETHODIMP OnWriteStart(IDownload *pDownload);
    STDMETHODIMP OnWriteData(IDownload *pDownload, void *pBuffer, DWORD nDataSize, DWORD *pWriteSize);
    STDMETHODIMP OnWriteEnd(IDownload *pDownload, BOOL bOK);

    // IHttpDownloaderHandlerBuffer
    STDMETHODIMP GetResponsePointer(void **pBuffer);

private:
    std::vector<BYTE> m_vBuffer;
    DWORD m_nResponseSize;
};
