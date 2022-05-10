#pragma once

// 下载机状态：ready -> running -> finish/failed
//                    \----------\----------------> stopping -> failed
#define DOWNLOAD_READY      1
#define DOWNLOAD_RUNNING    2
#define DOWNLOAD_STOPPING   3
#define DOWNLOAD_FAILED     4
#define DOWNLOAD_FINISH     5

__interface IDownload;

// 下载事件回调，在发起的线程回调
MIDL_INTERFACE("ECD29BAB-46CC-4ad4-9CAB-B7CBBAC609AD")
IDownloadEventHandler : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnStart(IDownload *pDownload) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnFinished(IDownload *pDownload) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnError(IDownload *pDownload, HRESULT nResult, DWORD nErrCode) = 0;
};

// 下载回调，会在任意可用的工作线程中回调
MIDL_INTERFACE("EE9D8DFB-B14B-490f-AE82-D2847A223A26")
IDownloadHandler : IUnknown
{
public:
    virtual DWORD STDMETHODCALLTYPE GetResponseSize() = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPreferredResponseSize(DWORD nResponseSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnWriteStart(IDownload *pDownload) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnWriteData(IDownload *pDownload, void *pBuffer, DWORD nDataSize, DWORD *pWriteSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnWriteEnd(IDownload *pDownload, BOOL bOK) = 0;
};

// Buffer类型的下载回调
MIDL_INTERFACE("8AB2783D-D128-4fc7-AF0D-C60337D1E404")
IDownloadHandlerBuffer : IDownloadHandler
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetResponsePointer(void **pBuffer) = 0;
};

// 文件类型的下载回调
MIDL_INTERFACE("8AB2783D-D128-4fc7-AF0D-C60337D1E404")
IDownloadHandlerFile : IDownloadHandler
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetResponseFile(BSTR *pFilePath) = 0;
};

MIDL_INTERFACE("80705FB1-95DF-4962-B4B8-765A46941794")
IDownload : IUnknown
{
public:
    // Control
    virtual HRESULT STDMETHODCALLTYPE Start() = 0;
    virtual HRESULT STDMETHODCALLTYPE Stop() = 0;
    virtual DWORD STDMETHODCALLTYPE GetState() = 0;

    // Id
    virtual DWORD STDMETHODCALLTYPE GetId() = 0;

    // URL
    virtual HRESULT STDMETHODCALLTYPE GetURL(BSTR *pURL) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetURL(BSTR strURL) = 0;

    // EventHandler
    virtual HRESULT STDMETHODCALLTYPE GetEventHandler(IDownloadEventHandler **ppHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetEventHandler(IDownloadEventHandler *pHandler) = 0;

    // DownloadHandler
    virtual HRESULT STDMETHODCALLTYPE GetDownloadHandler(IDownloadHandler **ppHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDownloadHandler(IDownloadHandler *pHandler) = 0;
};
