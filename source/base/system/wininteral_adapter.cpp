#include "stdafx.h"
#include "base/system/wininteral_adapter.h"

typedef NTSTATUS (WINAPI *FuncNtQueryInformationProcess)(
    HANDLE ProcessHandle,
    WIN_PROCESS_INFORMATION_CLASS ProcessInformationClass,
    PVOID ProcessInformation,
    ULONG ProcessInformationLength,
    PULONG ReturnLength);

typedef NTSTATUS (WINAPI* FuncNtQueryInformationThread)(
    HANDLE ThreadHandle,
    WIN_THREAD_INFORMATION_CLASS ThreadInformationClass,
    PVOID ThreadInformation,
    ULONG ThreadInformationLength,
    PULONG ReturnLength
    );

static HMODULE s_hNtDll = NULL;
static FuncNtQueryInformationProcess s_pfnNtQueryInformationProcess = NULL;
static FuncNtQueryInformationThread s_pfnNtQueryInformationThread = NULL;

struct WinInteralAutoLoad
{
    WinInteralAutoLoad()
    {
        s_hNtDll = ::LoadLibrary(_T("ntdll.dll"));
        if(NULL != s_hNtDll) {
            s_pfnNtQueryInformationProcess = (FuncNtQueryInformationProcess)::GetProcAddress(s_hNtDll, "NtQueryInformationProcess");
            s_pfnNtQueryInformationThread = (FuncNtQueryInformationThread)::GetProcAddress(s_hNtDll, "NtQueryInformationThread");
        }
    }

    ~WinInteralAutoLoad()
    {
        s_pfnNtQueryInformationProcess = NULL;
        s_pfnNtQueryInformationThread = NULL;
    }
};

static WinInteralAutoLoad s_oWinInteralAutoLoad;

NTSTATUS
AdaNtQueryInformationProcess(HANDLE ProcessHandle,
                             WIN_PROCESS_INFORMATION_CLASS ProcessInformationClass,
                             PVOID ProcessInformation,
                             ULONG ProcessInformationLength,
                             PULONG ReturnLength)
{
    if(NULL == s_pfnNtQueryInformationProcess) {
        return -1;
    }

    return s_pfnNtQueryInformationProcess(ProcessHandle, ProcessInformationClass, ProcessInformation, ProcessInformationLength, ReturnLength);
}

NTSTATUS
AdaNtQueryInformationThread(HANDLE ThreadHandle,
                            WIN_THREAD_INFORMATION_CLASS ThreadInformationClass,
                            PVOID ThreadInformation,
                            ULONG ThreadInformationLength,
                            PULONG ReturnLength)
{
    if(NULL == s_pfnNtQueryInformationThread) {
        return -1;
    }

    return s_pfnNtQueryInformationThread(ThreadHandle, ThreadInformationClass, ThreadInformation, ThreadInformationLength, ReturnLength);
}