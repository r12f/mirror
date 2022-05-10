#pragma once

#include "base/system/wininteral_adapter.h"

class ThreadControl
{
public:
    ThreadControl();
    ThreadControl(DWORD nTid, DWORD nAccess = THREAD_ALL_ACCESS);
    ~ThreadControl();

    operator HANDLE();

    BOOL Open(DWORD nTid, DWORD nAccess = THREAD_ALL_ACCESS);
    BOOL Close();
    BOOL Terminate(UINT nExitCode);
    DWORD GetProcessId();
    BOOL GetThreadModule(CString &strModule);

protected:
    BOOL FetchTEB();

private:
    HANDLE m_hThread;
    TEB *m_pTeb;
};