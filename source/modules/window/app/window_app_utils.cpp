#include "stdafx.h"
#include "modules/window/app/window_app_utils.h"
#include "base/kv/kv.h"
#include "modules/framework.h"
#include "modules/component_id.h"
#include "modules/builtin/component_config.h"
#include "modules/ui_interface.h"
#include "modules/builtin/component_hotkey.h"

HWND
GetWatchWindow()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return NULL;
    }

    HWND hWatchWindow = NULL;
    pConfig->GetInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_WATCH_HWND), (INT *)&hWatchWindow);
    return hWatchWindow;
}

BOOL
GetWatchWindowWithOffset(HWND *pWnd, INT *pX, INT *pY)
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return NULL;
    }

    HWND hWatchWindow = NULL;
    INT nX = 0, nY = 0;
    pConfig->GetUInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_WATCH_HWND), (UINT *)&hWatchWindow);
    pConfig->GetInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_MOUSE_OFFSET_X), &nX);
    pConfig->GetInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_MOUSE_OFFSET_Y), &nY);

    if(NULL != pWnd) {
        *pWnd = hWatchWindow;
    }

    if(NULL != pX) {
        *pX = nX;
    }

    if(NULL != pY) {
        *pY = nY;
    }

    return TRUE;
}

BOOL
SetWatchWindowWithOffset(HWND hWatchWindow, INT nX, INT nY)
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    pConfig->SetUInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_WATCH_HWND), (UINT)hWatchWindow);
    pConfig->SetInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_MOUSE_OFFSET_X), nX);
    pConfig->SetInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_MOUSE_OFFSET_Y), nY);

    return TRUE;
}

BOOL
RefreshAllWindowInfo()
{
    CComPtr<IUIControl> pUIWindow = GetComponentInterface<IUIControl>(COMPONENT_UI_DLG_WINDOW);
    if(NULL == pUIWindow) {
        return FALSE;
    }

    pUIWindow->RequestRefresh(TRUE, TRUE);

    return TRUE;
}

BOOL
ProvideHotkeyHandler(HotkeyHandlerData *pHandlers, DWORD nHandlerNum)
{
    CComPtr<IHotkeyManager> pHotkeyMgr = GetComponentInterface<IHotkeyManager>(COMPONENT_HOTKEY);
    if(NULL == pHotkeyMgr) {
        return FALSE;
    }

    DWORD i = 0;
    for(i = 0; i < nHandlerNum; ++i) {
        pHotkeyMgr->SetHotkeyHandler(CComBSTR(pHandlers[i].m_pSection), CComBSTR(pHandlers[i].m_pName), pHandlers[i].m_hWindow, pHandlers[i].m_nMsg, pHandlers[i].m_wParam, pHandlers[i].m_lParam);
    }

    return TRUE;
}