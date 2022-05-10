#include "stdafx.h"
#include "base/system/thread_control.h"
#include "base/system/system_snapshot_walker.h"

ThreadControl::ThreadControl()
    : m_hThread(NULL)
    , m_pTeb(NULL)
{

}

ThreadControl::ThreadControl(DWORD nTid, DWORD nAccess)
    : m_hThread(NULL)
    , m_pTeb(NULL)
{
    Open(nTid, nAccess);
}

ThreadControl::~ThreadControl()
{
    Close();
}

ThreadControl::operator HANDLE()
{
    return m_hThread;
}

BOOL
ThreadControl::Open(DWORD nTid, DWORD nAccess)
{
    Close();

    HANDLE hThread = ::OpenThread(nAccess, FALSE, nTid);
    if(NULL == hThread) {
        return FALSE;
    }

    m_hThread = hThread;

    return TRUE;
}

BOOL
ThreadControl::Close()
{
    if(NULL != m_hThread) {
        ::CloseHandle(m_hThread);
    }
    m_hThread = NULL;

    if(NULL != m_pTeb) {
        delete m_pTeb;
    }
    m_pTeb = NULL;

    return TRUE;
}

BOOL
ThreadControl::Terminate(UINT nExitCode)
{
    return ::TerminateThread(m_hThread, nExitCode);
}

DWORD
ThreadControl::GetProcessId()
{
    if(NULL == m_hThread) {
        return 0;
    }

    ULONG nRetLen = 0;
    WIN_THREAD_BASIC_INFORMATION oTBI = {0};
    AdaNtQueryInformationThread(m_hThread, ThreadBasicInformation, &oTBI, sizeof(WIN_THREAD_BASIC_INFORMATION), &nRetLen);
    return (DWORD)(oTBI.ClientId.UniqueProcess);
}

BOOL
ThreadControl::GetThreadModule(CString &strModule)
{
    strModule = _T("");

    if(NULL == m_hThread) {
        return FALSE;
    }

    ULONG nRetLen = 0;
    PVOID pThreadStartAddr = NULL;
    AdaNtQueryInformationThread(m_hThread, ThreadQuerySetWin32StartAddress, &pThreadStartAddr, sizeof(pThreadStartAddr), &nRetLen);
    if(NULL == pThreadStartAddr) {
        return FALSE;
    }

    DWORD nPid = GetProcessId();
    Module32Walker oWalker;
    if(!oWalker.Start(nPid)) {
        return FALSE;
    }

    MODULEENTRY32 *pModuleEntry = oWalker.Get();
    do {
        if(pModuleEntry->modBaseAddr <= pThreadStartAddr
            && pModuleEntry->modBaseAddr + pModuleEntry->modBaseSize >= pThreadStartAddr)
        {
            strModule = pModuleEntry->szModule;
            return TRUE;
        }
    } while(oWalker.WalkNext());

    return FALSE;
}

BOOL
ThreadControl::FetchTEB()
{
    if(NULL != m_pTeb) {
        return TRUE;
    }

    return TRUE;
}