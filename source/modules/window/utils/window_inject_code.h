#pragma once

#include "modules/builtin/component_code_injector.h"

struct GetRemoteWndProcParam {
    WCHAR szUser32[20];
    WCHAR szApphelp[20];
    char szIsWindow[20];
    char szIsWindowUnicode[20];
    char szGetWindowLongW[20];
    char szGetWindowLongA[20];
    char szGetWindowLongPtrW[20];
    char szGetWindowLongPtrA[20];
    HWND m_hWindow;

    GetRemoteWndProcParam(HWND hWindow);
};

extern InjectCodeEntry g_pGetRemoteWndProc;