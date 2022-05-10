#pragma once

#include "base/system/wininteral_adapter.h"

class ProcessControl
{
public:
    ProcessControl();
    ProcessControl(DWORD nPid, DWORD nAccess = PROCESS_ALL_ACCESS);
    ProcessControl(HANDLE hProcess);
    ~ProcessControl();

    operator HANDLE();

    BOOL Open(DWORD nPid, DWORD nAccess = PROCESS_ALL_ACCESS);
    BOOL Close();
    HANDLE Release();
    BOOL Terminate(UINT nExitCode);
    BOOL GetExePath(CString &strFilePath);
    BOOL GetExeFileName(CString &strFileName);
    BOOL GetCommandLine(CString &strCmdLine);
    BOOL GetWorkDir(CString &strWorkDir);

protected:
    BOOL FetchPEB();
    void Reset();

private:
    HANDLE m_hProcess;
    PEB *m_pPeb;
    RTL_USER_PROCESS_PARAMETERS *m_pProcUserParams;
};