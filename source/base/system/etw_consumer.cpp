#include "stdafx.h"
#include "base/system/etw_consumer.h"
#include "base/scoped_handle.h"

// http://msdn.microsoft.com/en-us/library/aa364114(v=VS.85).aspx
struct ScopedHandleTraitTraceHandle {
    static BOOL Destroy(TRACEHANDLE h) { return ::CloseTrace(h); }
    static TRACEHANDLE NullHandle() { return (TRACEHANDLE)INVALID_HANDLE_VALUE; }
};

typedef ScopedHandleBase<TRACEHANDLE, ScopedHandleTraitTraceHandle> ScopedTraceHandle;

EtwConsumer::EtwConsumer()
    : m_nPointerSize(0)
    , m_bUserMode(FALSE)
    , m_nTimerResolution(0)
    , m_bTerminateTrace(FALSE)
{
    
}

EtwConsumer::~EtwConsumer()
{
    StopTrace();
}

BOOL
EtwConsumer::StartTrace(LPTSTR pLoggerName)
{
    EVENT_TRACE_LOGFILE trace;
    TRACE_LOGFILE_HEADER* pHeader = &trace.LogfileHeader;

    // Identify the log file from which you want to consume events
    // and the callbacks used to process the events and buffers.
    ZeroMemory(&trace, sizeof(EVENT_TRACE_LOGFILE));
    trace.LogFileName = pLoggerName;
    trace.BufferCallback = (PEVENT_TRACE_BUFFER_CALLBACK)(EtwConsumer::ProcessBuffer);
    trace.EventCallback = (PEVENT_CALLBACK)(EtwConsumer::ProcessEvent);

    ScopedTraceHandle hTrace(::OpenTrace(&trace));
    if ((TRACEHANDLE)INVALID_HANDLE_VALUE == hTrace.Get()) {
        return FALSE;
    }

    m_nPointerSize = (USHORT)pHeader->PointerSize;
    m_bUserMode = pHeader->LogFileMode & EVENT_TRACE_PRIVATE_LOGGER_MODE;
    if (pHeader->TimerResolution > 0) {
        m_nTimerResolution = pHeader->TimerResolution / 10000;
    }

    //wprintf(L"Number of events lost:  %lu\n", pHeader->EventsLost);

    // Use pHeader to access all fields prior to LoggerName.
    // Adjust pHeader based on the pointer size to access
    // all fields after LogFileName. This is required only if
    // you are consuming events on an architecture that is 
    // different from architecture used to write the events.
    if (pHeader->PointerSize != sizeof(PVOID)) {
        pHeader = (PTRACE_LOGFILE_HEADER)((PUCHAR)pHeader + 2 * (pHeader->PointerSize - sizeof(PVOID)));
    }

    //wprintf(L"Number of buffers lost: %lu\n\n", pHeader->BuffersLost);

    HRESULT hr = ConnectToETWNamespace(CComBSTR(L"root\\wmi"));
    if (FAILED(hr)) {
        //wprintf(L"ConnectToETWNamespace failed with 0x%x\n", hr);
        return FALSE;
    }

    ULONG status = ::ProcessTrace(&hTrace, 1, 0, 0);
    if (status != ERROR_SUCCESS && status != ERROR_CANCELLED) {
        //wprintf(L"ProcessTrace failed with %lu\n", status);
    }

    hTrace.Reset();

    m_pService = NULL;

    CoUninitialize();

    return TRUE;
}

BOOL
EtwConsumer::StopTrace()
{
    m_bTerminateTrace = TRUE;
    return TRUE;
}

VOID
EtwConsumer::OnEvent(PEVENT_TRACE pEvent)
{
    if (IsEqualGUID(pEvent->Header.Guid, EventTraceGuid) &&
        pEvent->Header.Class.Type == EVENT_TRACE_TYPE_INFO)
    {
        return;
    }

    return;
}

HRESULT
EtwConsumer::ConnectToETWNamespace(BSTR bstrNamespace)
{
    HRESULT hr = S_OK;
    CComPtr<IWbemLocator> pLocator;
    hr = CoInitialize(0);
    hr = CoCreateInstance(__uuidof(WbemLocator), 0, CLSCTX_INPROC_SERVER, __uuidof(IWbemLocator), (LPVOID*)&pLocator);

    if (FAILED(hr)) {
        //wprintf(L"CoCreateInstance failed with 0x%x\n", hr);
        return hr;
    }

    hr = pLocator->ConnectServer(bstrNamespace, NULL, NULL, NULL, 0L, NULL, NULL, &m_pService);

    if (FAILED(hr)) {
        //wprintf(L"pLocator->ConnectServer failed with 0x%x\n", hr);
        return hr;
    }

    hr = CoSetProxyBlanket(m_pService,
        RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_PKT, RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL, EOAC_NONE);

    if (FAILED(hr)) {
        //wprintf(L"CoSetProxyBlanket failed with 0x%x\n", hr);
        m_pService = NULL;
    }

    return hr;
}

VOID WINAPI
EtwConsumer::ProcessEvent(PEVENT_TRACE pEvent)
{
    EtwConsumer *pThis = (EtwConsumer *)pEvent->ClientContext;
    if(NULL == pThis) {
        return;
    }

    pThis->OnEvent(pEvent);

    return;
}

ULONG WINAPI
EtwConsumer::ProcessBuffer(PEVENT_TRACE_LOGFILE pBuffer)
{
    EtwConsumer *pThis = (EtwConsumer *)pBuffer->Context;
    if(NULL == pThis) {
        return 0;
    }

    return pThis->m_bTerminateTrace;
}