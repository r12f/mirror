#pragma once

#include "modules/builtin/component_hotkey_id.h"

HWND GetWatchWindow();
BOOL GetWatchWindowWithOffset(HWND *pWnd, INT *pX, INT *pY);
BOOL SetWatchWindowWithOffset(HWND hWatchWindow, INT nX, INT nY);
BOOL RefreshAllWindowInfo();

#define ENSURE_WATCH_WINDOW(hwnd, ret)          \
    hwnd = GetWatchWindow();                    \
    if(NULL == hwnd || !::IsWindow(hwnd)) {     \
        return ret;                             \
    }

#define ENSURE_WATCH_WINDOW_RET_VOID(hwnd)      \
    hwnd = GetWatchWindow();                    \
    if(NULL == hwnd || !::IsWindow(hwnd)) {     \
        return;                                 \
    }

struct HotkeyHandlerData {
    LPCTSTR m_pSection;
    LPCTSTR m_pName;
    HWND m_hWindow;
    UINT m_nMsg;
    WPARAM m_wParam;
    LPARAM m_lParam;
};

BOOL ProvideHotkeyHandler(HotkeyHandlerData *pHandlers, DWORD nHandlerNum);

enum {
    REFRESH_PARAM_TRACK_MODE    = 1,
};