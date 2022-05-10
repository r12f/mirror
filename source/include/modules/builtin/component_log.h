#pragma once

// ʹ�ü򵥵�Channelʵ��Log��Log��ҪConsumer�ſ������
// ����ʵ�ֵĺô��ǣ������еĺ���ʵ���в���Ҫ����һ��������IErrorLog�Ľӿ���

enum {
    LOG_LEVEL_DEBUG     = 0,        // ����ʹ�õ���Ϣ
    LOG_LEVEL_INFO,                 // ��Ҫ��֪�û�����Ϣ
    LOG_LEVEL_WARN,                 // ��Ҫ��֪�û��ľ���
    LOG_LEVEL_ERROR,                // ��Ҫ��֪�û��Ĵ���
    LOG_LEVEL_FATAL,                // ��Ҫ��֪�û������ش��󣨵��³����˳���
    LOG_LEVEL_MAX
};

enum {
    LOG_RECORD_TYPE_BASIC   = 0,
};

MIDL_INTERFACE("391B5F16-9DB2-44d0-BAC6-70CAD9D2AF90")
ILogRecord : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetType(DWORD *pType) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLevel(DWORD *pLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLevel(DWORD nLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModule(BSTR *pModule) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetModule(BSTR strModule) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFile(BSTR *pFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFile(BSTR strFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFunction(BSTR *pFunction) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFunction(BSTR strFunction) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLine(DWORD *pLine) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetLine(DWORD nLine) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetContent(BSTR *pContent) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetContent(BSTR strContent) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetParam(PVOID *ppParam) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetParam(PVOID pParam) = 0;
};

MIDL_INTERFACE("6153DF96-6F80-47da-AA33-E39A27C6E45D")
ILogConsumer : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnLog(ILogRecord *pLogRecord) = 0;
};

MIDL_INTERFACE("63475D79-3ACA-4caa-BA85-65B60A16F6D0")
ILogCore : IUnknown
{
public:
    // Consumer
    virtual HRESULT STDMETHODCALLTYPE RegisterConsumer(ILogConsumer *pConsumer) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterConsumer(ILogConsumer *pConsumer) = 0;

    // Global Log Level
    virtual HRESULT STDMETHODCALLTYPE SetLogLevel(DWORD nLogLevel) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetLogLevel(DWORD *pLogLevel) = 0;

    // Log Record
    virtual HRESULT STDMETHODCALLTYPE CreateLogRecord(DWORD nType, ILogRecord **pLogRecord) = 0;
    virtual HRESULT STDMETHODCALLTYPE Log(ILogRecord *pLogRecord) = 0;
};
