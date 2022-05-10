#pragma once

#include <wbemidl.h>
#include <wmistr.h>

#ifndef INITGUID
#define INITGUID
#endif

#include <evntrace.h>

class EtwConsumer
{
public:
    EtwConsumer();
    virtual ~EtwConsumer();

    BOOL StartTrace(LPTSTR pLoggerName);
    BOOL StopTrace();

protected:
    virtual VOID OnEvent(PEVENT_TRACE pEvent);

    HRESULT ConnectToETWNamespace(BSTR bstrNamespace);
    static ULONG WINAPI ProcessBuffer(PEVENT_TRACE_LOGFILE Logfile);
    static VOID WINAPI ProcessEvent(PEVENT_TRACE pEvent);

protected:
    ULONG m_nPointerSize;
    BOOL m_bUserMode;
    ULONG m_nTimerResolution;
    BOOL m_bTerminateTrace;
    CComPtr<IWbemServices> m_pService;
};