#pragma once

#include "base/com/simple_com_class.h"
#include "modules/builtin/component_code_injector.h"

class InjectedCode :
    public SimpleComClassT<InjectedCode, ComRef, IInjectedCode>
{
public:
    InjectedCode(HANDLE hProcess, HANDLE hRemoteThread, LPVOID pAllocAddr, SIZE_T nAllocSize, InjectCodeParam oCodeParam, BOOL bAutoClean);
    virtual ~InjectedCode();

    STDMETHODIMP GetAutoClean(BOOL *pAutoClean);
    STDMETHODIMP SetAutoClean(BOOL bAutoClean);
    STDMETHODIMP Wait(DWORD nWaitMs);
    STDMETHODIMP GetRetValue(void *pValue, DWORD nValueSize);

private:
    BOOL m_bAutoClean;
    HANDLE m_hProcess;
    HANDLE m_hRemoteThread;
    LPVOID m_pAllocAddr;
    SIZE_T m_nAllocSize;
    InjectCodeParam m_oCodeParam;
};