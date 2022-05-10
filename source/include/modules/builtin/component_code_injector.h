#pragma once

typedef HMODULE(WINAPI * FuncLoadLibraryExW)(LPCWSTR lpLibFileName, HANDLE hFile, DWORD dwFlags);
typedef LPVOID (WINAPI * FuncGetProcAddress)(HMODULE hModule, LPCSTR lpProcName);

typedef struct _InjectCodeParam {
    FuncLoadLibraryExW m_pFuncLoadLibraryEx;
    FuncGetProcAddress m_pFuncGetProcAddress;
    BYTE *m_pParam;
    DWORD m_nParamSize;
    BYTE *m_pRetInfo;
    DWORD m_nRetSize;
} InjectCodeParam;

typedef DWORD (WINAPI * InjectCodeEntry)(InjectCodeParam *pParam);

// {EFF2496C-2851-4450-BC43-97091517A27F}
MIDL_INTERFACE("EFF2496C-2851-4450-BC43-97091517A27F")
IInjectedCode : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetAutoClean(BOOL *pAutoClean) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetAutoClean(BOOL bAutoClean) = 0;
    virtual HRESULT STDMETHODCALLTYPE Wait(DWORD nWaitMs) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRetValue(void *pValue, DWORD nValueSize) = 0;
};

// {19953701-115A-474e-8177-F72B40F9866B}
MIDL_INTERFACE("19953701-115A-474e-8177-F72B40F9866B")
ICodeInjector : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE InjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode) = 0;
    virtual HRESULT STDMETHODCALLTYPE SafeInjectRawCode(DWORD nPid, InjectCodeEntry pFunction, DWORD nFunctionSize, void *pParam, DWORD nParamSize, DWORD nRetSize, IInjectedCode **ppCode) = 0;
};
