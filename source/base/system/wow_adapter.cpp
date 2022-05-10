#include "stdafx.h"
#include "base/system/wow_adapter.h"

typedef BOOL (WINAPI * FuncIsWow64Process)(
    __in  HANDLE hProcess,
    __out PBOOL Wow64Process
    );

static HMODULE s_hKernel32dll = NULL;
static FuncIsWow64Process s_pFuncIsWow64Process = NULL;

class WowAutoLoader
{
public:
    WowAutoLoader() {
        s_hKernel32dll = ::LoadLibrary(_T("Kernel32.dll"));
        if(NULL != s_hKernel32dll) {
            s_pFuncIsWow64Process = (FuncIsWow64Process)::GetProcAddress(s_hKernel32dll, "IsWow64Process");
        }
    }

    ~WowAutoLoader() {}
};

static WowAutoLoader s_oWowAutoLoader;

BOOL AdaIsWow64Process(HANDLE hProcess, PBOOL Wow64Process)
{
    if(NULL == hProcess || NULL == Wow64Process) {
        return FALSE;
    }

    // 如果此函数都不存在，那么肯定是32位的系统
    if(NULL == s_pFuncIsWow64Process) {
        *Wow64Process = FALSE;
        return TRUE;
    }

    return s_pFuncIsWow64Process(hProcess, Wow64Process);
}
