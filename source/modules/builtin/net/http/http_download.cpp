#include "stdafx.h"
#include "base/scoped_internet_handle.h"
#include "base/log/log.h"
#include "modules/framework.h"
#include "modules/component_id.h"
#include "modules/builtin/net/http/http_download.h"
#include "modules/builtin/net/http/http_download_event.h"

#pragma comment(lib, "WinInet.lib")

static DWORD s_nDownloadId = 0;

#define DEFAULT_USER_AGENT      _T("Mozilla/4.0 (compatible; MSIE 8.0; Windows NT 6.1; Trident/4.0;)")

HttpDownload::HttpDownload()
    : m_nId(++s_nDownloadId)
    , m_nBirthTid(::GetCurrentThreadId())
    , m_nTaskState(DOWNLOAD_READY)
    , m_nMethod(HTTP_METHOD_GET)
    , m_nReqScheme(INTERNET_SCHEME_HTTP)
    , m_nReqPort(0)
{
    
}

HttpDownload::~HttpDownload()
{

}

void
HttpDownload::CallHandlerOnStart()
{
    MLOG_DEBUG("HttpDownload", "�������أ�%s", (LPCTSTR)m_strURL);

    if(NULL == m_pEventHandler) {
        return;
    }

    m_pEventHandler->OnStart(this);
}

void
HttpDownload::CallHandlerOnFinished()
{
    MLOG_DEBUG("HttpDownload", "������ɣ�%s", (LPCTSTR)m_strURL);

    if(NULL == m_pEventHandler) {
        return;
    }

    m_pEventHandler->OnFinished(this);
}

void
HttpDownload::CallHandlerOnError(HRESULT nResult, DWORD nErrCode)
{
    MLOG_DEBUG("HttpDownload", "���ش���%s", (LPCTSTR)m_strURL);

    if(NULL == m_pEventHandler) {
        return;
    }

    m_pEventHandler->OnError(this, nResult, nErrCode);
}


HRESULT
HttpDownload::Start()
{
    // ���URL�ĺϷ��ԣ�����������Ҫ����ĸ�������
    TCHAR strUserName[INTERNET_MAX_USER_NAME_LENGTH] = {0};
    TCHAR strPassword[INTERNET_MAX_PASSWORD_LENGTH] = {0};
    TCHAR strHost[INTERNET_MAX_HOST_NAME_LENGTH] = {0};
    TCHAR strPath[INTERNET_MAX_PATH_LENGTH] = {0};
    TCHAR strExtraInfo[INTERNET_MAX_PATH_LENGTH] = {0};

    URL_COMPONENTS oURLComp = {0};
    oURLComp.dwStructSize = sizeof(URL_COMPONENTS);
    oURLComp.lpszHostName = strHost;
    oURLComp.dwHostNameLength = INTERNET_MAX_HOST_NAME_LENGTH;
    oURLComp.lpszUserName = strUserName;
    oURLComp.dwUserNameLength = INTERNET_MAX_USER_NAME_LENGTH;
    oURLComp.lpszPassword = strPassword;
    oURLComp.dwPasswordLength = INTERNET_MAX_PASSWORD_LENGTH;
    oURLComp.lpszUrlPath = strPath;
    oURLComp.dwUrlPathLength = INTERNET_MAX_PATH_LENGTH;
    oURLComp.lpszExtraInfo = strExtraInfo;
    oURLComp.dwExtraInfoLength = INTERNET_MAX_PATH_LENGTH;
    if(!::InternetCrackUrl(m_strURL, m_strURL.Length(), 0, &oURLComp)) {
        return E_FAIL;
    }

    m_nReqScheme = oURLComp.nScheme;
    m_strReqUserName = oURLComp.lpszUserName;
    m_strReqPassword = oURLComp.lpszPassword;
    m_strReqHost = oURLComp.lpszHostName;
    m_nReqPort = oURLComp.nPort;
    m_strReqPath.Format(_T("%s%s"), oURLComp.lpszUrlPath, oURLComp.lpszExtraInfo);

    CComPtr<IThreadPool> pThreadPool = GetComponentInterface<IThreadPool>(COMPONENT_THREAD_POOL);
    if(NULL == pThreadPool) {
        return E_FAIL;
    }

    return pThreadPool->DispatchToWorker(this);
}

HRESULT
HttpDownload::Stop()
{
    SetState(DOWNLOAD_STOPPING);
    return S_OK;
}

DWORD
HttpDownload::SetState(DWORD nNewState, IThreadPool *pThreadPool, HRESULT nResult, DWORD nErrCode)
{
    DWORD nState = (DWORD)::InterlockedExchange(&m_nTaskState, nNewState);

    IThreadPoolWorkItem *pWorkItem = NULL;
    if(NULL != pThreadPool) {
        switch(nNewState) {
        case DOWNLOAD_RUNNING:
            pWorkItem = new HttpDownloadStartEvent(this);
            break;
        case DOWNLOAD_FINISH:
            pWorkItem = new HttpDownloadFinishEvent(this);
            break;
        case DOWNLOAD_FAILED:
            pWorkItem = new HttpDownloadErrorEvent(this, nResult, nErrCode);
            break;
        }
    }

    pThreadPool->DispatchToSpecifiedWorker(m_nBirthTid, pWorkItem);
    pWorkItem->Release();
    pWorkItem = NULL;

    return nState;
}

DWORD
HttpDownload::GetState()
{
    volatile DWORD *pState = &m_nTaskState;
    return *pState;
}

DWORD
HttpDownload::GetId()
{
    return m_nId;
}

HRESULT
HttpDownload::GetURL(BSTR *pURL)
{
    return m_strURL.CopyTo(pURL);
}

HRESULT
HttpDownload::SetURL(BSTR strURL)
{
    m_strURL = strURL;
    return S_OK;
}

HRESULT
HttpDownload::GetEventHandler(IDownloadEventHandler **ppHandler)
{
    if(NULL == ppHandler) {
        return E_INVALIDARG;
    }

    return m_pEventHandler->QueryInterface(__uuidof(IDownloadEventHandler), (void **)ppHandler);
}

HRESULT
HttpDownload::SetEventHandler(IDownloadEventHandler *pHandler)
{
    m_pEventHandler = pHandler;
    return S_OK;
}

HRESULT
HttpDownload::GetDownloadHandler(IDownloadHandler **ppHandler)
{
    if(NULL == ppHandler) {
        return E_INVALIDARG;
    }

    return m_pDownloadHandler->QueryInterface(__uuidof(IDownloadHandler), (void **)ppHandler);
}

HRESULT
HttpDownload::SetDownloadHandler(IDownloadHandler *pHandler)
{
    if(NULL == pHandler) {
        // TODO: ����Ĭ�ϵĻص�
        return E_INVALIDARG;
    }

    m_pDownloadHandler = pHandler;

    return S_OK;
}

HRESULT
HttpDownload::GetMethod(DWORD *pMethod)
{
    if(NULL == pMethod) {
        return E_INVALIDARG;
    }
    
    *pMethod = m_nMethod;

    return S_OK;
}

HRESULT
HttpDownload::SetMethod(DWORD nMethod)
{
    if(HTTP_METHOD_GET != nMethod && HTTP_METHOD_POST != nMethod) {
        return E_FAIL;
    }

    m_nMethod = nMethod;

    return S_OK;
}

HRESULT
HttpDownload::GetRef(BSTR *pRef)
{
    return m_strRef.CopyTo(pRef);
}

HRESULT
HttpDownload::SetRef(BSTR strRef)
{
    m_strRef = strRef;
    return S_OK;
}

HRESULT
HttpDownload::GetUserAgent(BSTR *pUA)
{
    return m_strUA.CopyTo(pUA);
}

HRESULT
HttpDownload::SetUserAgent(BSTR strUA)
{
    m_strUA = strUA;
    return S_OK;
}

HRESULT
HttpDownload::GetHeaderMap(IKVMap **ppHeaderMap)
{
    if(NULL == ppHeaderMap) {
        return E_INVALIDARG;
    }

    return m_pHeaderMap->QueryInterface(__uuidof(IKVMap), (void **)ppHeaderMap);
}

HRESULT
HttpDownload::SetHeaderMap(IKVMap *pHeaderMap)
{
    m_pHeaderMap = pHeaderMap;
    return S_OK;
}

HRESULT
HttpDownload::Run(IThreadPool *pThreadPool)
{
    // ��ʼ����
    SetState(DOWNLOAD_RUNNING, pThreadPool);

    HRESULT hr = RunDownload();
    if(FAILED(hr)) {
        // ����ʧ��
        SetState(DOWNLOAD_FAILED, pThreadPool, hr, ::GetLastError());
    } else {
        // �������
        SetState(DOWNLOAD_FINISH, pThreadPool);
    }

    return S_OK;
}

HRESULT
HttpDownload::RunDownload() 
{
    // ���ӷ�����
    LPCTSTR pUA = (NULL == m_strUA) ? DEFAULT_USER_AGENT : m_strUA;
    ScopedINetHandle hSession = ::InternetOpen(pUA,
        INTERNET_OPEN_TYPE_PRECONFIG,
        NULL,
        NULL,
        0);

    if(NULL == hSession) {
        return E_FAIL;
    }

    ScopedINetHandle hConnect = ::InternetConnect(hSession,
        m_strReqHost,
        m_nReqPort,
        m_strReqUserName,
        m_strReqPassword,
        INTERNET_SERVICE_HTTP,
        0,
        0);

    if(NULL == hConnect) {
        return E_FAIL;
    }

    if(ShouldQuitDownload()) {
        return E_FAIL;
    }

    // ��������������
    ScopedINetHandle hRequest = CreateHttpRequest(hConnect);
    if(NULL == hRequest) {
        return E_FAIL;
    }

    if(ShouldQuitDownload()) {
        return E_FAIL;
    }

    HRESULT hr = SendHttpRequest(hRequest);
    if(FAILED(hr)) {
        return E_FAIL;
    }

    if(ShouldQuitDownload()) {
        return E_FAIL;
    }

    // ��ȡ��Ӧ
    return ReceiveHttpResponse(hRequest);
}

HINTERNET
HttpDownload::CreateHttpRequest(HINTERNET hConnect)
{
    // ��������
    BOOL bIsSecureReq = (m_nReqScheme == INTERNET_SCHEME_HTTPS);
    DWORD dwRequestFlag = INTERNET_FLAG_RELOAD | INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_KEEP_CONNECTION;
    if(bIsSecureReq) {
        dwRequestFlag |= INTERNET_FLAG_SECURE | INTERNET_FLAG_IGNORE_CERT_CN_INVALID | INTERNET_FLAG_IGNORE_CERT_DATE_INVALID;
    }

    ScopedINetHandle hRequest = ::HttpOpenRequest(hConnect,
        (m_nMethod == HTTP_METHOD_POST) ? _T("POST") : _T("GET"),
        m_strReqPath,
        _T("HTTP/1.1"),
        NULL,
        0,
        dwRequestFlag,
        0);

    if(NULL == hRequest) {
        return NULL;
    }

    // TODO: ����Header

    return hRequest.Detach();
}

HRESULT
HttpDownload::SendHttpRequest(HINTERNET hRequest)
{
    // ��������
    BOOL bIsSecureReq = (m_nReqScheme == INTERNET_SCHEME_HTTPS);
    DWORD dwErrCode = 0;
    if(::HttpSendRequest(hRequest, NULL, 0, NULL, 0)) {
        return S_OK;
    }

    dwErrCode = GetLastError();

    if(!bIsSecureReq || dwErrCode != ERROR_INTERNET_INVALID_CA) {
        ::InternetCloseHandle(hRequest);
        return E_FAIL;
    }

    if(ShouldQuitDownload()) {
        return E_FAIL;
    }

    DWORD dwFlags = 0;
    DWORD dwBuffLen = sizeof(dwFlags);
    ::InternetQueryOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS,
        (LPVOID)&dwFlags, &dwBuffLen);

    dwFlags |= SECURITY_FLAG_IGNORE_UNKNOWN_CA;
    ::InternetSetOption(hRequest, INTERNET_OPTION_SECURITY_FLAGS,
        &dwFlags, sizeof(dwFlags));

    if(!::HttpSendRequest(hRequest, NULL, 0, NULL, 0)) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT
HttpDownload::ReceiveHttpResponse(HINTERNET hRequest)
{
    DWORD dwFileSize = 0;
    DWORD dwLengthBufQuery = sizeof(dwFileSize);
    BOOL bHasContentLength = ::HttpQueryInfo(hRequest,
        HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
        &dwFileSize, 
        &dwLengthBufQuery,
        NULL);

    if(!bHasContentLength) {
        dwFileSize = 10240;
    }

    // QueryInfo�����һ�������ݣ�������Ҫ���һ��
    if(ShouldQuitDownload()) {
        return E_FAIL;
    }

    if(NULL != m_pDownloadHandler) {
        m_pDownloadHandler->SetPreferredResponseSize(dwFileSize);
        m_pDownloadHandler->OnWriteStart(this);
    }

    DWORD dwTotalRead = 0, dwBytesRead = 0;
    char szRecvBuf[DEFAULT_RECV_BUF_SIZE] = {0};

    BOOL bOK = FALSE;
    for(;;) {
        // ÿ�ζ�ȡ����֮ǰ�����һ�Σ��Ƿ���Ҫֹͣ
        if(ShouldQuitDownload()) {
            break;
        }

        // ��������
        BOOL bRead = ::InternetReadFile(hRequest,
            szRecvBuf,
            DEFAULT_RECV_BUF_SIZE, 
            &dwBytesRead);

        if(!bRead) {
            break;
        }

        if(0 == dwBytesRead) {
            bOK = TRUE;
            break;
        }

        // ������ɣ����һ��
        if(ShouldQuitDownload()) {
            break;
        }

        // д��Handler
        if(NULL != m_pDownloadHandler) {
            DWORD nHandlerWriteSize = 0, nHandlerTotalWriteSize = 0;
            while(nHandlerTotalWriteSize < dwBytesRead) {
                if(FAILED(m_pDownloadHandler->OnWriteData(this, szRecvBuf, dwBytesRead, &nHandlerWriteSize))) {
                    SetState(DOWNLOAD_STOPPING);
                    break;
                }
                nHandlerTotalWriteSize += nHandlerWriteSize;
            }
        }

        dwTotalRead += dwBytesRead;

        if(bHasContentLength) {
            if(dwTotalRead >= dwFileSize) {
                bOK = TRUE;
                break;
            }
        }
    }

    // �������������ˣ�����Ҳ������
    if(NULL != m_pDownloadHandler) {
        m_pDownloadHandler->OnWriteEnd(this, bOK);
    }

    return S_OK;
}

BOOL
HttpDownload::ShouldQuitDownload()
{
    DWORD nState = GetState();
    return (nState == DOWNLOAD_STOPPING || nState == DOWNLOAD_FAILED || nState == DOWNLOAD_FINISH);
}