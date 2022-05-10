#include "stdafx.h"
#include "base/system/process_control.h"

ProcessControl::ProcessControl()
    : m_hProcess(NULL)
    , m_pPeb(NULL)
    , m_pProcUserParams(NULL)
{

}

ProcessControl::ProcessControl(DWORD nPid, DWORD nAccess)
    : m_hProcess(NULL)
    , m_pPeb(NULL)
    , m_pProcUserParams(NULL)
{
    Open(nPid, nAccess);
}

ProcessControl::ProcessControl(HANDLE hProcess)
    : m_hProcess(hProcess)
    , m_pPeb(NULL)
    , m_pProcUserParams(NULL)
{

}

ProcessControl::~ProcessControl()
{
    Reset();
}

ProcessControl::operator HANDLE()
{
    return m_hProcess;
}

BOOL
ProcessControl::Open(DWORD nPid, DWORD nAccess)
{
    ASSERT_RETURN(0 != nPid, FALSE);

    HANDLE hProcess = ::OpenProcess(nAccess, FALSE, nPid);
    if(NULL == hProcess) {
        return FALSE;
    }

    m_hProcess = hProcess;

    return TRUE;
}

BOOL
ProcessControl::Close()
{
    ASSERT_RETURN(NULL != m_hProcess, FALSE);

    Reset();

    return TRUE;
}

HANDLE
ProcessControl::Release()
{
    HANDLE h = m_hProcess;
    m_hProcess = NULL;
    return h;
}

BOOL
ProcessControl::Terminate(UINT nExitCode)
{
    ASSERT_RETURN(NULL != m_hProcess, FALSE);

    ::TerminateProcess(m_hProcess, nExitCode);

    return TRUE;
}

BOOL
ProcessControl::GetExePath(CString &strFilePath)
{
    ASSERT_RETURN(NULL != m_hProcess, FALSE);
    ASSERT_RETURN(FetchPEB(), FALSE);

    strFilePath = _T("");

    if(NULL == m_pProcUserParams->ImagePathName.Buffer) {
        return FALSE;
    }

    DWORD nBufLen = MAX_PATH - 1;
    TCHAR szBuf[MAX_PATH] = {0};
    if(nBufLen > m_pProcUserParams->ImagePathName.Length) {
        nBufLen = m_pProcUserParams->ImagePathName.Length;
    }

    ::ReadProcessMemory(m_hProcess, m_pProcUserParams->ImagePathName.Buffer, szBuf, nBufLen, NULL);

    strFilePath = szBuf;

    return TRUE;
}

BOOL
ProcessControl::GetExeFileName(CString &strFileName)
{
    CString strExeFilePath;
    if(!GetExePath(strExeFilePath)) {
        return FALSE;
    }

    int nOSFileSep = strExeFilePath.ReverseFind(_T('\\'));
    if(nOSFileSep < 0) {
        strFileName = strExeFilePath;
        return TRUE;
    }

    strFileName = strExeFilePath.Mid(nOSFileSep + 1);

    return TRUE;
}

BOOL
ProcessControl::GetCommandLine(CString &strCmdLine)
{
    ASSERT_RETURN(NULL != m_hProcess, FALSE);
    ASSERT_RETURN(!FetchPEB(), FALSE);

    strCmdLine = _T("");

    if(NULL == m_pProcUserParams->CommandLine.Buffer) {
        return FALSE;
    }

    DWORD nBufLen = MAX_PATH - 1;
    TCHAR szBuf[MAX_PATH] = {0};
    if(nBufLen > m_pProcUserParams->CommandLine.Length) {
        nBufLen = m_pProcUserParams->CommandLine.Length;
    }

    ::ReadProcessMemory(m_hProcess, m_pProcUserParams->CommandLine.Buffer, szBuf, nBufLen, NULL);

    strCmdLine = szBuf;

    return TRUE;
}

BOOL
ProcessControl::GetWorkDir(CString &strWorkDir)
{
    ASSERT_RETURN(NULL != m_hProcess, FALSE);
    ASSERT_RETURN(!FetchPEB(), FALSE);

    strWorkDir = _T("");

    if(NULL == m_pProcUserParams->CurrentDirectory.Buffer) {
        return FALSE;
    }

    DWORD nBufLen = MAX_PATH - 1;
    TCHAR szBuf[MAX_PATH] = {0};
    if(nBufLen > m_pProcUserParams->CurrentDirectory.Length) {
        nBufLen = m_pProcUserParams->CurrentDirectory.Length;
    }

    ::ReadProcessMemory(m_hProcess, m_pProcUserParams->CurrentDirectory.Buffer, szBuf, nBufLen, NULL);

    strWorkDir = szBuf;

    return TRUE;
}

BOOL
ProcessControl::FetchPEB()
{
    if(NULL != m_pPeb) {
        return TRUE;
    }

    WIN_PROCESS_BASIC_INFORMATION oProcBasicInfo = {0};
    AdaNtQueryInformationProcess(m_hProcess, ProcessBasicInformation, &oProcBasicInfo, sizeof(WIN_PROCESS_BASIC_INFORMATION), NULL);
    if(NULL == oProcBasicInfo.PebBaseAddress) {
        return FALSE;
    }

    PEB *pPeb = new PEB;
    RTL_USER_PROCESS_PARAMETERS *pProcUserParams = new RTL_USER_PROCESS_PARAMETERS;
    if(NULL == pPeb || NULL == pProcUserParams) {
        if(NULL != pPeb) {
            delete pPeb;
        }

        if(NULL != pProcUserParams) {
            delete pProcUserParams;
        }
        return FALSE;
    }

    memset(pPeb, 0, sizeof(RTL_USER_PROCESS_PARAMETERS));
    memset(pProcUserParams, 0, sizeof(RTL_USER_PROCESS_PARAMETERS));

    if(!::ReadProcessMemory(m_hProcess, oProcBasicInfo.PebBaseAddress, pPeb, sizeof(PEB), NULL) || 0 == pPeb->ProcessParameters) {
        delete pPeb;
        delete pProcUserParams;
        return FALSE;
    }
    
    if(!::ReadProcessMemory(m_hProcess, pPeb->ProcessParameters, pProcUserParams, sizeof(RTL_USER_PROCESS_PARAMETERS), NULL)) {
        delete pPeb;
        delete pProcUserParams;
        return FALSE;
    }

    m_pPeb = pPeb;
    m_pProcUserParams = pProcUserParams;

    return TRUE;
}

void
ProcessControl::Reset()
{
    ::CloseHandle(m_hProcess);

    m_hProcess = NULL;

    if(NULL != m_pPeb) {
        delete m_pPeb;
    }

    m_pPeb = NULL;

    if(NULL == m_pProcUserParams) {
        delete m_pProcUserParams;
    }

    m_pProcUserParams = NULL;

    return;
}