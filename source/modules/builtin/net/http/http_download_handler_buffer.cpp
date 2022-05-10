#include "stdafx.h"
#include "modules/builtin/net/http/http_download_handler_buffer.h"

HttpDownloadHandlerBuffer::HttpDownloadHandlerBuffer()
{

}

HttpDownloadHandlerBuffer::~HttpDownloadHandlerBuffer()
{

}

DWORD
HttpDownloadHandlerBuffer::GetResponseSize()
{
    return (DWORD)m_vBuffer.size();
}

HRESULT
HttpDownloadHandlerBuffer::SetPreferredResponseSize(DWORD nResponseSize)
{
    m_vBuffer.reserve(nResponseSize);
    return S_OK;
}

HRESULT
HttpDownloadHandlerBuffer::OnWriteStart(IDownload *pDownload)
{
    return S_OK;
}

HRESULT
HttpDownloadHandlerBuffer::OnWriteData(IDownload *pDownload, void *pBuffer, DWORD nDataSize, DWORD *pWriteSize)
{
    DWORD nSize = (DWORD)m_vBuffer.size();
    m_vBuffer.resize(nSize + nDataSize);
    memcpy_s(&(m_vBuffer[nSize]), nDataSize, pBuffer, nDataSize);
    *pWriteSize = nDataSize;
    return S_OK;
}

HRESULT
HttpDownloadHandlerBuffer::OnWriteEnd(IDownload *pDownload, BOOL bOK)
{
    // ±‹√‚ª∫¥Ê‘ΩΩÁ
    if(bOK) {
        DWORD nSize = (DWORD)m_vBuffer.size();
        m_vBuffer.resize(nSize + 1);
        m_vBuffer[nSize] = 0;
    }

    return S_OK;
}

HRESULT
HttpDownloadHandlerBuffer::GetResponsePointer(void **pBuffer)
{
    if(NULL == pBuffer) {
        return E_INVALIDARG;
    }

    if(m_vBuffer.size() == 0) {
        *pBuffer = NULL;
        return S_OK;
    }

    *pBuffer = &(m_vBuffer[0]);

    return S_OK;
}