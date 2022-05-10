#include "stdafx.h"
#include "modules/window/utils/window_utils.h"
#include "modules/framework.h"
#include "modules/builtin/component_code_injector.h"
#include "modules/window/utils/window_inject_code.h"

UINT
ConvertModToHotkeyF(UINT nMod)
{
    UINT nHotkeyF = ((nMod & (~MOD_ALT)) & (~MOD_SHIFT));
    nHotkeyF |= (0 == (nMod & MOD_ALT)) ? 0 : HOTKEYF_ALT;
    nHotkeyF |= (0 == (nMod & MOD_SHIFT)) ? 0 : HOTKEYF_SHIFT;
    return nHotkeyF;
}

UINT
ConvertHotkeyFToMod(UINT nHotkeyF)
{
    UINT nMod = ((nHotkeyF & (~HOTKEYF_ALT)) & (~HOTKEYF_ALT));
    nMod |= (0 == (nHotkeyF & HOTKEYF_ALT)) ? 0 : MOD_ALT;
    nMod |= (0 == (nHotkeyF & HOTKEYF_SHIFT)) ? 0 : MOD_SHIFT;
    return nMod;
}

LONG_PTR GetWindowProc(HWND hWindow)
{
    if(!::IsWindow(hWindow)) {
        return NULL;
    }

    DWORD nPid = 0;
    GetWindowThreadProcessId(hWindow, &nPid);
    if(nPid == 0) {
        return NULL;
    }

    // 如果是当前进程内
    if(nPid == ::GetCurrentProcessId()) {
        if(::IsWindowUnicode(hWindow)) {
            return ::GetWindowLongPtrW(hWindow, GWLP_WNDPROC);
        }

        return ::GetWindowLongPtrA(hWindow, GWLP_WNDPROC);
    }

    // 否则，注入代码去获取
    CComPtr<ICodeInjector> pCodeInjector = GetComponentInterface<ICodeInjector>(COMPONENT_CODE_INJECTOR);
    if(NULL == pCodeInjector) {
        return NULL;
    }

    CComPtr<IInjectedCode> pCode;
    GetRemoteWndProcParam oParam(hWindow);
    if(FAILED(pCodeInjector->SafeInjectRawCode(nPid, g_pGetRemoteWndProc, 3000, &oParam, sizeof(oParam), sizeof(LPVOID), &pCode)) || NULL == pCode) {
        return NULL;
    }

    pCode->Wait(500);

    LPVOID pRetValue = NULL;
    if(FAILED(pCode->GetRetValue(&pRetValue, sizeof(LPVOID)))) {
        return NULL;
    }

    return (LONG_PTR)pRetValue;
}