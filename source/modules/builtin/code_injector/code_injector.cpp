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
        MLOG_DEBUG("CodeInjector", "�򿪽��� %lu ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);
        return E_ACCESSDENIED;
    }
    
    // ��鵱ǰ�����Ŀ�������32λ�Ļ���64λ�ģ������һ�������޷�ע�����
    if(bCheckMachine) {
        BOOL bIsCurrentWow64 = FALSE, bIsTargetWow64 = FALSE;
        AdaIsWow64Process(::GetCurrentProcess(), &bIsCurrentWow64);
        AdaIsWow64Process(oInjectProc, &bIsTargetWow64);
        if(bIsTargetWow64 != bIsCurrentWow64) {
            nErrCode = ::GetLastError();
            MLOG_DEBUG("CodeInjector", "�޷�������ע����� %lu��32λ/64λ��ƥ�䣺%lu (0x%08x)", nPid, nErrCode, nErrCode);
            return E_FAIL;
        }
    }

    // ��ȡ��Ҫע�����Ϣ
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

    // ������������ռ�
    SIZE_T nCodeParamSize = sizeof(InjectCodeParam) + nParamSize + nRetSize;
    SIZE_T nAllocSize = (((nFunctionSize + nCodeParamSize) >> 12) + 1) << 12;    // ҳ���С��4K
    BYTE *pAllocAddr = (BYTE *)::VirtualAllocEx(oInjectProc, NULL, nAllocSize,  MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
    if(NULL == pAllocAddr) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "׼��������ע����� %lu ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);
        return E_ACCESSDENIED;
    }

    // ���ֿռ�
    BYTE *pRemoteFunctionAddr = pAllocAddr + nCodeParamSize;
    BYTE *pRemoteDataAddr = pAllocAddr;

    // д�����
    SIZE_T nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, pRemoteFunctionAddr, pFunction, nFunctionSize, &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "������ע����� %lu ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    // д���������
    oCodeParam.m_pParam += (ULONG_PTR)pRemoteDataAddr;
    oCodeParam.m_pRetInfo += (ULONG_PTR)pRemoteDataAddr;

    nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, pRemoteDataAddr, &oCodeParam, sizeof(InjectCodeParam), &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "������ע����� %lu ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    nWriteNum = 0;
    if(!::WriteProcessMemory(oInjectProc, oCodeParam.m_pParam, pParam, nParamSize, &nWriteNum)) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "������ע����� %lu ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    // ����Զ���߳�
    ScopedHandle hRemoteThread = ::CreateRemoteThread(oInjectProc, NULL, 0, (LPTHREAD_START_ROUTINE)pRemoteFunctionAddr, pRemoteDataAddr, 0, NULL);
    if(NULL == hRemoteThread) {
        nErrCode = ::GetLastError();
        MLOG_DEBUG("CodeInjector", "���н��� %lu ��ע��Ĵ���ʧ�ܣ�%lu (0x%08x)", nPid, nErrCode, nErrCode);

        ::VirtualFreeEx(oInjectProc, pAllocAddr, 0, MEM_RELEASE);
        return E_ACCESSDENIED;
    }

    if(NULL == ppCode) {
        return S_OK;
    }

    // ע����ɣ������������
    InjectedCode *pCode = new InjectedCode(oInjectProc.Release(), hRemoteThread.Detach(), pAllocAddr, nAllocSize, oCodeParam, TRUE);
    if(NULL == pCode) {
        return E_OUTOFMEMORY;
    }

    *ppCode = pCode;

    return S_OK;
}