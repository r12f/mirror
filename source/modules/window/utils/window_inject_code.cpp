#include "stdafx.h"
#include "modules/window/utils/window_inject_code.h"

GetRemoteWndProcParam::GetRemoteWndProcParam(HWND hWindow)
{
    wcscpy_s(szUser32, sizeof(szUser32), L"user32.dll");
    wcscpy_s(szApphelp, sizeof(szApphelp), L"apphelp.dll");
    strcpy_s(szIsWindow, sizeof(szIsWindow), "IsWindow");
    strcpy_s(szIsWindowUnicode, sizeof(szIsWindowUnicode), "IsWindowUnicode");
    strcpy_s(szGetWindowLongW, sizeof(szGetWindowLongW), "GetWindowLongW");
    strcpy_s(szGetWindowLongA, sizeof(szGetWindowLongA), "GetWindowLongA");
    strcpy_s(szGetWindowLongPtrW, sizeof(szGetWindowLongPtrW), "GetWindowLongPtrW");
    strcpy_s(szGetWindowLongPtrA, sizeof(szGetWindowLongPtrA), "GetWindowLongPtrA");
    m_hWindow = hWindow;
}

typedef BOOL (WINAPI * FuncIsWindow)(HWND hWnd);
typedef BOOL (WINAPI * FuncIsWindowUnicode)(HWND hWnd);
typedef LONG (WINAPI * FuncGetWindowLongW)(HWND hWnd, int nIndex);
typedef LONG (WINAPI * FuncGetWindowLongA)(HWND hWnd, int nIndex);
typedef LONG_PTR (WINAPI * FuncGetWindowLongPtrW)(HWND hWnd, int nIndex);
typedef LONG_PTR (WINAPI * FuncGetWindowLongPtrA)(HWND hWnd, int nIndex);

#pragma check_stack(push)
#pragma check_stack(off)
#pragma runtime_checks("", off)
static DWORD WINAPI GetRemoteWndProc(InjectCodeParam *pParam)
{
    if(NULL == pParam || NULL == pParam->m_pFuncLoadLibraryEx || NULL == pParam->m_pFuncGetProcAddress
        || NULL == pParam->m_pParam || sizeof(GetRemoteWndProcParam) != pParam->m_nParamSize
        || NULL == pParam->m_pRetInfo || sizeof(LPVOID) != pParam->m_nRetSize)
    {
        return 0;
    }

    GetRemoteWndProcParam *pFunctionParam = (GetRemoteWndProcParam *)(pParam->m_pParam);
    HWND hWindow = pFunctionParam->m_hWindow;
    if (NULL == hWindow) {
        return 0;
    }

    LPVOID *ppWndProc = (LPVOID *)(pParam->m_pRetInfo);
    pParam->m_pFuncLoadLibraryEx(pFunctionParam->szApphelp, NULL, 0);

    HMODULE hUser32 = pParam->m_pFuncLoadLibraryEx(pFunctionParam->szUser32, NULL, 0);
    if(NULL == hUser32) {
        return 0;
    }

    FuncIsWindow pFuncIsWindow = (FuncIsWindow) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szIsWindow));
    FuncIsWindowUnicode pFuncIsWindowUnicode = (FuncIsWindowUnicode) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szIsWindowUnicode));
    FuncGetWindowLongW pFuncGetWindowLongW = (FuncGetWindowLongW) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szGetWindowLongW));
    FuncGetWindowLongA pFuncGetWindowLongA = (FuncGetWindowLongA) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szGetWindowLongA));
    FuncGetWindowLongPtrW pFuncGetWindowLongPtrW = (FuncGetWindowLongPtrW) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szGetWindowLongPtrW));
    FuncGetWindowLongPtrA pFuncGetWindowLongPtrA = (FuncGetWindowLongPtrA) (pParam->m_pFuncGetProcAddress(hUser32, pFunctionParam->szGetWindowLongPtrA));

    if(NULL == pFuncIsWindow || !pFuncIsWindow(hWindow)) {
        return 0;
    }

    LPVOID pWndProc = NULL;
    if(pFuncIsWindowUnicode(hWindow)) {
        if(NULL != pFuncGetWindowLongPtrW) {
            pWndProc = (LPVOID)pFuncGetWindowLongPtrW(hWindow, GWLP_WNDPROC);
        } else if(NULL != pFuncGetWindowLongW) {
            pWndProc = (LPVOID)pFuncGetWindowLongW(hWindow, GWLP_WNDPROC);  // GWLP_WNDPROC的值和GWL_WNDPROC一样，但是GWL_WNDPROC在64位下不存在
        }
    } else {
        if(NULL != pFuncGetWindowLongPtrA) {
            pWndProc = (LPVOID)pFuncGetWindowLongPtrA(hWindow, GWLP_WNDPROC);
        } else if(NULL != pFuncGetWindowLongA) {
            pWndProc = (LPVOID)pFuncGetWindowLongA(hWindow, GWLP_WNDPROC);
        }
    }

    *ppWndProc = pWndProc;

    return 0;
} 
#pragma runtime_checks("", restore)
#pragma check_stack(pop)

InjectCodeEntry g_pGetRemoteWndProc = GetRemoteWndProc;