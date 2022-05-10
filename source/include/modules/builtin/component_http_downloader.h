#pragma once

#include "base/kv/kv.h"
#include "modules/builtin/component_downloader.h"

enum {
    HTTP_METHOD_GET    = 1,
    HTTP_METHOD_POST,
};

// ��������
MIDL_INTERFACE("364743ED-8893-48a1-A08C-B8072FBC6B2A")
IHttpDownload : IDownload
{
public:
    // Method
    virtual HRESULT STDMETHODCALLTYPE GetMethod(DWORD *pMethod) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMethod(DWORD nMethod) = 0;

    // Ref
    virtual HRESULT STDMETHODCALLTYPE GetRef(BSTR *pRef) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRef(BSTR strRef) = 0;

    // UA
    virtual HRESULT STDMETHODCALLTYPE GetUserAgent(BSTR *pUA) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUserAgent(BSTR strUA) = 0;

    // HeaderMap
    virtual HRESULT STDMETHODCALLTYPE GetHeaderMap(IKVMap **ppHeaderMap) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetHeaderMap(IKVMap *pHeaderMap) = 0;
};

// ����ģ�飬�������첽�ģ����Ա���Ҫ����EventHandler��������ɵ��¼�
MIDL_INTERFACE("D1C4D048-58DE-4acb-AD00-4543294618EA")
IHttpDownloader : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE DownloadToFile(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, BSTR strFilePath, IDownloadEventHandler *pEventHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE DownloadToBuffer(IHttpDownload **ppDownload, BSTR strURL, BSTR strRef, IDownloadEventHandler *pEventHandler) = 0;
};
