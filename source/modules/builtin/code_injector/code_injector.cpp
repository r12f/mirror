#include "stdafx.h"
#include "base/scoped_handle.h"
#include "base/system/process_control.h"
#include "base/system/wow_adapter.h"
#include "modules/builtin/code_injector/code_injector.h"
#include "modules/builtin/code_injector/injected_code.h"

CodeInjector::CodeInjector()
{
    
}

CodeInjector::~CodeInjector()
{
    
}

HRESULT
CodeInjector::InjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode)
{
    return DoInjectRawCode(FALSE, nPid, pFunction, nFunctionSize, pParam, nParamSize, nRetSize, ppCode);
}

HRESULT
CodeInjector::SafeInjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode)
{
    return DoInjectRawCode(TRUE, nPid, pFunction, nFunctionSize, pParam, nParamSize, nRetSize, ppCode);
}

HRESULT
CodeInjector::DoInjectRawCode(BOOL bCheckMachine, DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode)
{
    DWORD nErrCode = 0;
    if(0 == nPid || NULL == pFunction) {
        return E_INVALIDARG;
    }

    ProcessControl oInjectProc;
    if(!oInjectProc.Open(nPid)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "打开进程 %lu 失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);
        return E_ACCESSDENIED;
    }
    
    // 检查当前程序和目标程序是32位的还是64位的，如果不一样，则无法注入代码
    if(bCheckMachine) {
        BOOL bIsCurrentWow64 = FALSE, bIsTargetWow64 = FALSE;
        AdaIsWow64Process(::GetCurrentProcess(), &bIsCurrentWow64);
        AdaIsWow64Process(oInjectProc, &bIsTargetWow64);
        if(bIsTargetWow64 != bIsCurrentWow64) {
            nErrCode = ::GetLastError();
            MLOG_DEBUG("CodeInjector", "无法将代码注入进程 %lu，32位/64位不匹配：%lu (0x%08x)", nPid, nErrCode, nErrCode);
            return E_FAIL;
        }
    }

    // 获取需要注入的信息
    HMODULE hKernel32Dll = ::GetModuleHandle(_T("kernel32.dll"));
    HMODULE hKernelBaseDll = ::GetModuleHandle(_T("kernelbase.dll"));
    if(NULL == hKernel32Dll) {
        return E_FAIL;
    }

    InjectCodeParam oCodeParam = {0};
    if(NULL == hKernelBaseDll) {
        oCodeParam.m_pFuncGetProcAddress = (FuncGetProcAddress)::GetProcAddress(hKernel32Dll, "GetProcAddress");
        oCodeParam.m_pFuncLoadLibraryEx = (FuncLoadLibraryExW)::GetProcAddress(hKernel32Dll, "LoadLibraryExW");
    } else {
        oCodeParam.m_pFuncGetProcAddress = (FuncGetProcAddress)::GetProcAddress(hKernelBaseDll, "GetProcAddress");
        oCodeParam.m_pFuncLoadLibraryEx = (FuncLoadLibraryExW)::GetProcAddress(hKernelBaseDll, "LoadLibraryExW");
        if (NULL == oCodeParam.m_pFuncLoadLibraryEx) {
            oCodeParam.m_pFuncLoadLibraryEx = (FuncLoadLibraryExW)::GetProcAddress(hKernel32Dll, "LoadLibraryExW");
        }
    }

    oCodeParam.m_pParam = (BYTE *)(ULONG_PTR)sizeof(InjectCodeParam);
    oCodeParam.m_nParamSize = nParamSize;
    oCodeParam.m_pRetInfo = (BYTE *)(ULONG_PTR)(sizeof(InjectCodeParam) + nParamSize);
    oCodeParam.m_nRetSize = nRetSize;

    // 创建代码所需空间
    SIZE_T nCodeParamSize = sizeof(InjectCodeParam) + nParamSize + nRetSize;
    SIZE_T nAllocSize = (((nFunctionSize + nCodeParamSize) >> 12) + 1) << 12;    // 页面大小是4K
    BYTE *pAllocAddr = (BYTE *)::VirtualAllocEx(oInjectProc, NULL, nAllocSize,  MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if(NULL == pAllocAddr) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "准备将代码注入进程 %lu 失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);
        return E_ACCESSDENIED;
    }

    // 划分空间
    BYTE *pRemoteFunctionAddr = pAllocAddr + nCodeParamSize;
    BYTE *pRemoteDataAddr = pAllocAddr;

    // 写入代码
    SIZE_T nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, pRemoteFunctionAddr, pFunction, nFunctionSize, &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "将代码注入进程 %lu 失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    // 写入相关数据
    oCodeParam.m_pParam += (ULONG_PTR)pRemoteDataAddr;
    oCodeParam.m_pRetInfo += (ULONG_PTR)pRemoteDataAddr;

    nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, pRemoteDataAddr, &oCodeParam, sizeof(InjectCodeParam), &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "将代码注入进程 %lu 失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, oCodeParam.m_pParam, pParam, nParamSize, &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "将代码注入进程 %lu 失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    // 创建远程线程
    ScopedHandle hRemoteThread = ::CreateRemoteThread(oInjectProc, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteFunctionAddr, pRemoteDataAddr, 0, NULL);
    if(NULL == hRemoteThread) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "运行进程 %lu 中注入的代码失败：%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    if(NULL == ppCode) {
        return S_OK;
    }

    // 注入完成，创建代码对象
    InjectedCode *pCode = new InjectedCode(oInjectProc.Release(), hRemoteThread.Detach(), pAllocAddr, nAllocSize, oCodeParam, TRUE);
    if(NULL == pCode) {
        return E_OUTOFMEMORY;
    }

    *ppCode = pCode;

    return S_OK;
}