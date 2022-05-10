#include "stdafx.h"
#include "modules/builtin/hotkey/hotkey_manager.h"
#include "modules/builtin/hotkey/hotkey.h"
#include "modules/builtin/hotkey/hotkey_handler.h"
#include "modules/builtin/hotkey/global_hotkey_event_pump.h"
#include "modules/builtin/hotkey/local_hotkey_event_pump.h"

HotkeyManager::HotkeyManager()
{
    GlobalHotkeyEventPump::CreateObject(__uuidof(IHotkeyEventPump), (void **)&m_pGlobalPump);
    if(NULL != m_pGlobalPump) {
        m_pGlobalPump->Init();
    }

    LocalHotkeyEventPump::CreateObject(__uuidof(IHotkeyEventPump), (void **)&m_pLocalPump);
    if(NULL != m_pLocalPump) {
        m_pLocalPump->Init();
    }
}

HotkeyManager::~HotkeyManager()
{
    if(NULL != m_pGlobalPump) {
        m_pGlobalPump->Uninit();
    }

    if(NULL != m_pLocalPump) {
        m_pLocalPump->Uninit();
    }

    m_pGlobalPump = NULL;
    m_pLocalPump = NULL;
}

HRESULT
HotkeyManager::RegisterHotkey(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey **ppHotkey)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);

    CComPtr<IHotkey> pHotkey;
    UnregisterHotkey(strSection, strName, &pHotkey);

    if(NULL == pHotkey) {
        if(FAILED(Hotkey::CreateObject(__uuidof(IHotkey), (void **)&pHotkey)) || NULL == pHotkey) {
            return E_FAIL;
        }
    }

    Hotkey *pRawHotkey = static_cast<Hotkey *>(pHotkey.p);
    pRawHotkey->SetSection(strSection);
    pRawHotkey->SetName(strName);
    pRawHotkey->SetModifier(nModifiers);
    pRawHotkey->SetKey(nKey);
    pRawHotkey->SetGlobal(bGlobal);
    pRawHotkey->SetEventPumpManager(this);
    pRawHotkey->FixHotkeyByConfig();
    pRawHotkey->RegisterHotkey();

    m_vHotkeys.push_back(pHotkey);

    if(NULL != ppHotkey) {
        pHotkey.CopyTo(ppHotkey);
    }

    return S_OK;
}

HRESULT
HotkeyManager::UnregisterHotkey(BSTR strSection, BSTR strName, IHotkey **ppHotkey)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);

    HotkeyList::iterator oHotkeyIt = GetHotkeyIt(strSection, strName);
    if(oHotkeyIt == m_vHotkeys.end()) {
        return E_FAIL;
    }

    (*oHotkeyIt)->UnregisterHotkey();

    if(NULL != ppHotkey) {
        (*oHotkeyIt).CopyTo(ppHotkey);
    }

    return S_OK;
}

HRESULT
HotkeyManager::GetHotkey(BSTR strSection, BSTR strName, IHotkey **ppHotkey)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppHotkey, E_INVALIDARG);

    HotkeyList::iterator oHotkeyIt = GetHotkeyIt(strSection, strName);
    if(oHotkeyIt == m_vHotkeys.end()) {
        return E_FAIL;
    }

    if(NULL != ppHotkey) {
        (*oHotkeyIt).CopyTo(ppHotkey);
    }

    return S_OK;
}

HRESULT
HotkeyManager::GetHotkeyCount(DWORD *pCount)
{
    ASSERT_RETURN(NULL != pCount, E_INVALIDARG);

    *pCount = (DWORD)m_vHotkeys.size();

    return S_OK;
}

HRESULT
HotkeyManager::GetAllHotkey(IHotkey **ppHotkey, DWORD nMaxCount)
{
    ASSERT_RETURN(NULL != ppHotkey, E_INVALIDARG);

    DWORD nCount = 0;
    HotkeyList::iterator oHotkeyIt = m_vHotkeys.begin();
    while(oHotkeyIt != m_vHotkeys.end()) {
        (*oHotkeyIt).CopyTo(&(ppHotkey[nCount]));
        ++oHotkeyIt;
        ++nCount;
    }

    return S_OK;
}

HRESULT
HotkeyManager::SetHotkeyHandler(BSTR strSection, BSTR strName, HWND hNotifyWnd, DWORD nNotifyMessage, WPARAM nNotifyMessageWParam, LPARAM nNotifyMessageLParam)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);
    ASSERT_RETURN(NULL != hNotifyWnd, E_INVALIDARG);
    ASSERT_RETURN(NULL != nNotifyMessage, E_INVALIDARG);

    HotkeyList::iterator oHotkeyIt = GetHotkeyIt(strSection, strName);
    if(oHotkeyIt == m_vHotkeys.end()) {
        return E_FAIL;
    }

    CComPtr<IHotkeyHandler> pHotkeyHandler;
    if(FAILED(HotkeyHandler::CreateObject(__uuidof(IHotkeyHandler), (void **)&pHotkeyHandler)) || NULL == pHotkeyHandler) {
        return E_FAIL;
    }

    pHotkeyHandler->SetNotifyHwnd(hNotifyWnd);
    pHotkeyHandler->SetNotifyMessage(nNotifyMessage);
    pHotkeyHandler->SetNotifyMessageWParam(nNotifyMessageWParam);
    pHotkeyHandler->SetNotifyMessageLParam(nNotifyMessageLParam);

    (*oHotkeyIt)->SetHandler(pHotkeyHandler);

    return S_OK;
}

HRESULT
HotkeyManager::SetHotkeyHandler(BSTR strSection, BSTR strName, IHotkeyHandler *pHandler)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);

    HotkeyList::iterator oHotkeyIt = GetHotkeyIt(strSection, strName);
    if(oHotkeyIt == m_vHotkeys.end()) {
        return E_FAIL;
    }

    (*oHotkeyIt)->SetHandler(pHandler);

    return S_OK;
}

HRESULT
HotkeyManager::GetHotkeyHandler(BSTR strSection, BSTR strName, IHotkeyHandler **ppHandler)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppHandler, E_INVALIDARG);

    HotkeyList::iterator oHotkeyIt = GetHotkeyIt(strSection, strName);
    if(oHotkeyIt == m_vHotkeys.end()) {
        return E_FAIL;
    }

    (*oHotkeyIt)->GetHandler(ppHandler);

    return S_OK;
}

HRESULT
HotkeyManager::AddHotkeyNotifyHandler(IHotkeyNotifyHandler *pHandler)
{
    ASSERT_RETURN(NULL != pHandler, E_POINTER);
    m_vNotifyList.push_back(pHandler);
    return S_OK;
}

HRESULT
HotkeyManager::RemoveHotkeyNotifyHandler(IHotkeyNotifyHandler *pHandler)
{
    HotkeyNotifyList::iterator oNotifyIt = m_vNotifyList.begin();
    while(oNotifyIt != m_vNotifyList.end()) {
        if((*oNotifyIt) == pHandler) {
            m_vNotifyList.erase(oNotifyIt);
            return S_OK;
        }
        ++oNotifyIt;
    }

    return S_OK;
}

HRESULT
HotkeyManager::GetEventPump(DWORD nType, IHotkeyEventPump **pPump)
{
    ASSERT_RETURN(NULL != pPump, E_INVALIDARG);

    *pPump = NULL;

    switch(nType) {
    case HOTKEY_EVENT_PUMP_GLOBAL:
        m_pGlobalPump.CopyTo(pPump);
        return S_OK;
    case HOTKEY_EVENT_PUMP_LOCAL:
        m_pLocalPump.CopyTo(pPump);
        return S_OK;
    }

    return E_FAIL;
}

HRESULT
HotkeyManager::OnHotkeyRegistered(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey)
{
    HotkeyNotifyList::iterator oNotifyIt = m_vNotifyList.begin();
    while(oNotifyIt != m_vNotifyList.end()) {
        (*oNotifyIt)->OnRegister(strSection, strName, nModifiers, nKey, bGlobal, pHotkey);
        ++oNotifyIt;
    }

    return S_OK;
}

HRESULT
HotkeyManager::OnHotkeyUnregistered(BSTR strSection, BSTR strName, IHotkey *pHotkey)
{
    HotkeyNotifyList::iterator oNotifyIt = m_vNotifyList.begin();
    while(oNotifyIt != m_vNotifyList.end()) {
        (*oNotifyIt)->OnUnregister(strSection, strName, pHotkey);
        ++oNotifyIt;
    }

    return S_OK;
}

HotkeyManager::HotkeyList::iterator
HotkeyManager::GetHotkeyIt(BSTR strSection, BSTR strName)
{
    Hotkey *pHotkey = NULL;
    HotkeyList::iterator oHotkeyIt = m_vHotkeys.begin();
    while(oHotkeyIt != m_vHotkeys.end()) {
        pHotkey = static_cast<Hotkey *>((*oHotkeyIt).p);
        if(pHotkey->m_strSection == strSection && pHotkey->m_strName == strName) {
            break;
        }
        ++oHotkeyIt;
    }

    return oHotkeyIt;
}
