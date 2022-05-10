#include "stdafx.h"
#include "modules/builtin/hotkey/global_hotkey_event_pump.h"

GlobalHotkeyEventPump::SystemHotkeyInfo::SystemHotkeyInfo()
    : m_nHotkeyId(0)
{

}

GlobalHotkeyEventPump::SystemHotkeyInfo::SystemHotkeyInfo(INT nHotkeyId)
    : m_nHotkeyId(nHotkeyId)
{

}

GlobalHotkeyEventPump::SystemHotkeyInfo::SystemHotkeyInfo(const SystemHotkeyInfo &rhs)
    : m_nHotkeyId(rhs.m_nHotkeyId)
    , m_vHotkeys(rhs.m_vHotkeys)
{

}

GlobalHotkeyEventPump::HotkeyList::iterator
GlobalHotkeyEventPump::SystemHotkeyInfo::GetHotkey(IHotkey *pHotkey)
{
    HotkeyList::iterator oHotkeyIt = m_vHotkeys.begin();
    while(oHotkeyIt != m_vHotkeys.end()) {
        if((*oHotkeyIt).p == pHotkey) {
            break;
        }
        ++oHotkeyIt;
    }

    return oHotkeyIt;
}

GlobalHotkeyEventPump::GlobalHotkeyEventPump()
    : m_nNextHotkeyId(1)
{
    
}

GlobalHotkeyEventPump::~GlobalHotkeyEventPump()
{

}

HRESULT
GlobalHotkeyEventPump::Init()
{
    Create(HWND_MESSAGE);

    return S_OK;
}

HRESULT
GlobalHotkeyEventPump::Uninit()
{
    DestroyWindow();

    return S_OK;
}

HRESULT
GlobalHotkeyEventPump::GetType(DWORD *pType)
{
    ASSERT_RETURN(NULL != pType, E_INVALIDARG);

    *pType = HOTKEY_EVENT_PUMP_GLOBAL;

    return S_OK;
}

HRESULT
GlobalHotkeyEventPump::RegisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey)
{
    ASSERT_RETURN(0 != nModifier || 0 != nKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != pHotkey, E_INVALIDARG);
    ASSERT_RETURN(NULL != m_hWnd, E_FAIL);

    UINT64 nHotkeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    SystemHotkeyList::iterator oHotkeyIt = m_vSysHotkeys.find(nHotkeyIndex);
    if(oHotkeyIt != m_vSysHotkeys.end()) {
        if(oHotkeyIt->second.GetHotkey(pHotkey) == oHotkeyIt->second.m_vHotkeys.end()) {
            oHotkeyIt->second.m_vHotkeys.push_back(pHotkey);
        }
        return S_OK;
    }

    INT nHotkeyId = ++m_nNextHotkeyId;
    if(!::RegisterHotKey(m_hWnd, nHotkeyId, nModifier, nKey)) {
        return E_FAIL;
    }

    SystemHotkeyInfo oHotkeyInfo(nHotkeyId);
    oHotkeyInfo.m_vHotkeys.push_back(pHotkey);
    m_vSysHotkeys[nHotkeyIndex] = oHotkeyInfo;

    return S_OK;
}

HRESULT
GlobalHotkeyEventPump::UnregisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey)
{
    ASSERT_RETURN(0 != nModifier || 0 != nKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != pHotkey, E_INVALIDARG);
    ASSERT_RETURN(NULL != m_hWnd, E_FAIL);

    UINT64 nHotkeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    SystemHotkeyList::iterator oSysHotkeyIt = m_vSysHotkeys.find(nHotkeyIndex);
    if(oSysHotkeyIt == m_vSysHotkeys.end()) {
        return E_FAIL;
    }

    HotkeyList::iterator oHotkeyIt = oSysHotkeyIt->second.GetHotkey(pHotkey);
    if(oHotkeyIt != oSysHotkeyIt->second.m_vHotkeys.end()) {
        oSysHotkeyIt->second.m_vHotkeys.erase(oHotkeyIt);
    }

    if(0 != oSysHotkeyIt->second.m_vHotkeys.size()) {
        return TRUE;
    }

    INT nHotkeyId = oSysHotkeyIt->second.m_nHotkeyId;

    m_vSysHotkeys.erase(oSysHotkeyIt);

    if(!::UnregisterHotKey(m_hWnd, nHotkeyId)) {
        return FALSE;
    }

    return S_OK;
}

VOID
GlobalHotkeyEventPump::OnHotkey(INT nId, UINT nModifier, UINT nKey)
{
    UINT64 nHotkeyIndex = MakeSystemHotkeyIndex(nModifier, nKey);
    SystemHotkeyList::iterator oSysHotkeyIt = m_vSysHotkeys.find(nHotkeyIndex);
    if(oSysHotkeyIt == m_vSysHotkeys.end()) {
        return;
    }

    HotkeyList::iterator oHotkeyIt = oSysHotkeyIt->second.m_vHotkeys.begin();
    while(oHotkeyIt != oSysHotkeyIt->second.m_vHotkeys.end()) {
        (*oHotkeyIt)->OnHotkey(nModifier, nKey);
        ++oHotkeyIt;
    }

    return;
}

UINT64
GlobalHotkeyEventPump::MakeSystemHotkeyIndex(UINT nModifier, UINT nKey)
{
    UINT64 n = nModifier;
    n <<= 32;
    n |= nKey;
    return n;
}