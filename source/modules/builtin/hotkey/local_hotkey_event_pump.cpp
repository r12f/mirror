#include "stdafx.h"
#include "modules/builtin/hotkey/local_hotkey_event_pump.h"

HHOOK LocalHotkeyEventPump::s_hHook = NULL;
LocalHotkeyEventPump *LocalHotkeyEventPump::s_pThis = NULL;

LocalHotkeyEventPump::LocalHotkeyEventPump()
{
    s_pThis = this;
}

LocalHotkeyEventPump::~LocalHotkeyEventPump()
{
    s_pThis = NULL;
}

HRESULT
LocalHotkeyEventPump::GetType(DWORD *pType)
{
    ASSERT_RETURN(NULL != pType, E_INVALIDARG);

    *pType = HOTKEY_EVENT_PUMP_LOCAL;

    return S_OK;
}

HRESULT
LocalHotkeyEventPump::Init()
{
    s_hHook = ::SetWindowsHookEx(WH_KEYBOARD, KeyboardProc, ::GetModuleHandle(NULL), ::GetCurrentThreadId());
    if(NULL == s_hHook) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT
LocalHotkeyEventPump::Uninit()
{
    if(NULL != s_hHook) {
        ::UnhookWindowsHookEx(s_hHook);
        s_hHook = NULL;
    }

    return S_OK;
}

HRESULT
LocalHotkeyEventPump::RegisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey)
{
    ASSERT_RETURN(0 != nModifier || 0 != nKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != pHotkey, E_INVALIDARG);

    UINT64 nKeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    LocalHotkeyList::iterator oHotkeyIt = m_vLocalHotkeys.find(nKeyIndex);
    if(oHotkeyIt != m_vLocalHotkeys.end()) {
        return E_FAIL;
    }

    m_vLocalHotkeys[nKeyIndex] = pHotkey;


    return S_OK;
}

HRESULT
LocalHotkeyEventPump::UnregisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey)
{
    ASSERT_RETURN(0 != nModifier || 0 != nKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != pHotkey, E_INVALIDARG);

    UINT64 nKeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    LocalHotkeyList::iterator oHotkeyIt = m_vLocalHotkeys.find(nKeyIndex);
    if(oHotkeyIt == m_vLocalHotkeys.end()) {
        return E_FAIL;
    }

    m_vLocalHotkeys.erase(oHotkeyIt);

    return S_OK;
}

UINT64
LocalHotkeyEventPump::MakeSystemHotkeyIndex(UINT nModifier, UINT nKey)
{
    UINT64 n = nModifier;
    n <<= 32;
    n |= nKey;
    return n;
}

UINT
LocalHotkeyEventPump::GetKeyModifier(LPARAM lParam)
{
    UINT nModifier = 0;
    if(HIWORD(::GetKeyState(VK_CONTROL)) != 0) {
        nModifier |= MOD_CONTROL;
    }

    if(HIWORD(::GetKeyState(VK_MENU)) != 0) {
        nModifier |= MOD_ALT;
    }

    if(HIWORD(::GetKeyState(VK_SHIFT)) != 0) {
        nModifier |= MOD_SHIFT;
    }

    return nModifier;
}

BOOL
LocalHotkeyEventPump::OnKeyStroke(WPARAM wParam, LPARAM lParam)
{
    UINT nModifier = GetKeyModifier(lParam);
    UINT nKey = (UINT)wParam;

    UINT64 nKeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    LocalHotkeyList::iterator oHotkeyIt = m_vLocalHotkeys.find(nKeyIndex);
    if(oHotkeyIt == m_vLocalHotkeys.end()) {
        return FALSE;
    }

    if(NULL == oHotkeyIt->second) {
        return FALSE;
    }

    // 只响应按下的事件，吃掉释放的事件
    if(0 != (lParam & 0x80000000)) {
        return TRUE;
    }

    oHotkeyIt->second->OnHotkey(nModifier, nKey);

    return TRUE;
}

LRESULT
LocalHotkeyEventPump::KeyboardProc(int code, WPARAM wParam, LPARAM lParam)
{
    if(code < 0 || NULL == s_pThis) {
        return CallNextHookEx(s_hHook, code, wParam, lParam);
    }

    if(!s_pThis->OnKeyStroke(wParam, lParam)) {
        return CallNextHookEx(s_hHook, code, wParam, lParam);
    }

    return 1;
}