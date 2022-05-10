#include "stdafx.h"
#include "modules/builtin/code_injector/injected_code.h"

InjectedCode::InjectedCode(HANDLE hProcess, HANDLE hRemoteThread, LPVOID pAllocAddr, SIZE_T nAllocSize, InjectCodeParam oCodeParam, BOOL bAutoClean)
    : m_bAutoClean(bAutoClean)
    , m_hProcess(hProcess)
    , m_hRemoteThread(hRemoteThread)
    , m_pAllocAddr(pAllocAddr)
    , m_nAllocSize(nAllocSize)
    , m_oCodeParam(oCodeParam)
{

}

InjectedCode::~InjectedCode()
{
    if(NULL != m_hProcess) {
        if(NULL != m_hRemoteThread) {
            ::TerminateThread(m_hRemoteThread, 0);
            ::WaitForSingleObject(m_hRemoteThread, INFINITE);
            ::CloseHandle(m_hRemoteThread);
        }

        if(NULL != m_pAllocAddr) {
            ::VirtualFreeEx(m_hProcess, m_pAllocAddr, 0, MEM_RELEASE);
        }

        ::CloseHandle(m_hProcess);
    }
}

HRESULT
InjectedCode::GetAutoClean(BOOL *pAutoClean)
{
    ASSERT_RETURN(NULL != pAutoClean, E_POINTER);
    *pAutoClean = m_bAutoClean;
    return S_OK;
}

HRESULT
InjectedCode::SetAutoClean(BOOL bAutoClean)
{
    m_bAutoClean = bAutoClean;
    return S_OK;
}

HRESULT
InjectedCode::Wait(DWORD nWaitMs)
{
    if(WAIT_TIMEOUT == ::WaitForSingleObject(m_hRemoteThread, nWaitMs)) {
        return E_PENDING;
    }

    return S_OK;
}

HRESULT
InjectedCode::GetRetValue(void *pValue, DWORD nValueSize)
{
    SIZE_T nReadSize = 0;
    DWORD nNeedSize = nValueSize > m_oCodeParam.m_nRetSize ? m_oCodeParam.m_nRetSize : nValueSize;
    if(!ReadProcessMemory(m_hProcess, m_oCodeParam.m_pRetInfo, pValue, nNeedSize, &nReadSize)) {
        return E_ACCESSDENIED;
    }

    return S_OK;
}