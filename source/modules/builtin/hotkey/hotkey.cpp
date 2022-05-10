#include "stdafx.h"
#include "modules/builtin/hotkey/hotkey.h"

Hotkey::Hotkey()
    : m_strSection(NULL)
    , m_strName(NULL)
    , m_nModifier(0)
    , m_nKey(0)
    , m_bGlobal(FALSE)
    , m_bHasChanged(FALSE)
{

}

Hotkey::~Hotkey()
{

}

HRESULT
Hotkey::SetSection(BSTR strSection)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    
    m_strSection = strSection;

    return S_OK;
}

HRESULT
Hotkey::SetName(BSTR strName)
{
    ASSERT_RETURN(NULL != strName, E_INVALIDARG);

    m_strName = strName;

    return S_OK;
}

HRESULT
Hotkey::SetModifier(UINT nModifier)
{
    m_nModifier = nModifier;
    m_bHasChanged = TRUE;

    return S_OK;
}

HRESULT
Hotkey::SetKey(UINT nKey)
{
    m_nKey = nKey;
    m_bHasChanged = TRUE;

    return S_OK;
}

HRESULT
Hotkey::SetHandler(IHotkeyHandler *pHandler)
{
    m_pHandler = pHandler;

    return S_OK;
}

HRESULT
Hotkey::SetGlobal(BOOL bGlobal)
{
    m_bGlobal = bGlobal;
    m_bHasChanged = TRUE;

    return S_OK;
}

HRESULT
Hotkey::SetEventPumpManager(IHotkeyEventPumpManager *pPumpMgr)
{
    m_pEventPumpMgr = pPumpMgr;

    return S_OK;
}

HRESULT
Hotkey::GetSection(BSTR *pSection)
{
    ASSERT_RETURN(NULL != pSection, E_INVALIDARG);

    *pSection = CComBSTR(m_strSection).Detach();

    return S_OK;
}

HRESULT
Hotkey::GetName(BSTR *pName)
{
    ASSERT_RETURN(NULL != pName, E_INVALIDARG);
    
    *pName = CComBSTR(m_strName).Detach();

    return S_OK;
}

HRESULT
Hotkey::GetModifier(UINT *pModifier)
{
    ASSERT_RETURN(NULL != pModifier, E_INVALIDARG);

    *pModifier = m_nModifier;

    return S_OK;
}

HRESULT
Hotkey::GetKey(UINT *pKey)
{
    ASSERT_RETURN(NULL != pKey, E_INVALIDARG);

    *pKey = m_nKey;

    return S_OK;
}

HRESULT
Hotkey::GetHandler(IHotkeyHandler **ppHandler)
{
    ASSERT_RETURN(NULL != ppHandler, E_INVALIDARG);

    *ppHandler = m_pHandler;

    return S_OK;
}

HRESULT
Hotkey::GetGlobal(BOOL *pGlobal)
{
    ASSERT_RETURN(NULL != pGlobal, E_INVALIDARG);

    *pGlobal = m_bGlobal;

    return S_OK;
}

HRESULT
Hotkey::GetEventPumpManager(IHotkeyEventPumpManager **ppPumpMgr)
{
    ASSERT_RETURN(NULL != ppPumpMgr, E_INVALIDARG);

    m_pEventPumpMgr.CopyTo(ppPumpMgr);

    return S_OK;
}

HRESULT
Hotkey::RegisterHotkey()
{
    SaveHotkeyToConfig();

    ASSERT_RETURN(NULL != m_pEventPumpMgr, E_FAIL);

    DWORD nPumpType = m_bGlobal ? HOTKEY_EVENT_PUMP_GLOBAL : HOTKEY_EVENT_PUMP_LOCAL;
    if(FAILED(m_pEventPumpMgr->GetEventPump(nPumpType, &m_pEventPump)) || NULL == m_pEventPump) {
        ATLASSERT(FALSE);
        return E_FAIL;
    }

    if(0 == m_nModifier && 0 == m_nKey) {
        return E_FAIL;
    }

    m_pEventPump->RegisterHotkey(m_nModifier, m_nKey, this);
    m_pEventPumpMgr->OnHotkeyRegistered(m_strSection, m_strName, m_nModifier, m_nKey, m_bGlobal, this);

    return S_OK;
}

HRESULT
Hotkey::UnregisterHotkey()
{
    ASSERT_RETURN(NULL != m_pEventPump, E_FAIL);

    if(0 != m_nModifier || 0 != m_nKey) {
        m_pEventPump->UnregisterHotkey(m_nModifier, m_nKey, this);
        m_pEventPumpMgr->OnHotkeyUnregistered(m_strSection, m_strName, this);
    }

    m_pEventPump = NULL;

    return S_OK;
}

HRESULT
Hotkey::OnHotkey(UINT nModifier, UINT nKey)
{
    if(m_nModifier != nModifier || m_nKey != nKey) {
        return E_FAIL;
    }

    if(NULL == m_pHandler) {
        return E_FAIL;
    }

    return m_pHandler->OnHotkey(nModifier, nKey);
}

BOOL
Hotkey::FixHotkeyByConfig()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    ASSERT_RETURN(NULL != pConfig, FALSE);

    CComBSTR strHotkeyName = m_strSection;
    strHotkeyName.Append(_T("\\"));
    strHotkeyName.AppendBSTR(m_strName);

    UINT nModifier = 0, nKey = 0;
    BOOL bGlobal = FALSE;
    if(!ParseHotkeyFromConfig(pConfig, strHotkeyName, nModifier, nKey, bGlobal)) {
        return FALSE;
    }

    m_nModifier = nModifier;
    m_nKey = nKey;
    m_bGlobal = bGlobal;
    m_bHasChanged = FALSE;

    return TRUE;
}

BOOL
Hotkey::ParseHotkeyFromConfig(IConfig *pConfig, BSTR strHotkeyName, UINT &nModifier, UINT &nKey, BOOL &bGlobal)
{
    CComBSTR strHotkeyConfig;
    if(FAILED(pConfig->GetString(CComBSTR(CONFIG_SECTION_HOTKEY), strHotkeyName, &strHotkeyConfig)) || NULL == strHotkeyConfig) {
        return FALSE;
    }

    UINT nGlobal = 0;
    _sntscanf_s(strHotkeyConfig, strHotkeyConfig.Length(), _T("%lu+%lu+%lu"), &nModifier, &nKey, &nGlobal);
    bGlobal = (nGlobal != 0);

    return TRUE;
}

BOOL
Hotkey::SaveHotkeyToConfig()
{
    if(!m_bHasChanged) {
        return TRUE;
    }

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    ASSERT_RETURN(NULL != pConfig, FALSE);

    CComBSTR strHotkeyName = m_strSection;
    strHotkeyName.Append(_T("\\"));
    strHotkeyName.AppendBSTR(m_strName);

    CString strHotkeyConfig;
    strHotkeyConfig.Format(_T("%lu+%lu+%lu"), m_nModifier, m_nKey, (m_bGlobal ? 1 : 0));
    if(FAILED(pConfig->SetString(CComBSTR(CONFIG_SECTION_HOTKEY), strHotkeyName, CComBSTR(strHotkeyConfig)))) {
        return FALSE;
    }

    return TRUE;
}