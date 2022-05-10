#include "stdafx.h"
#include "base/io/directory.h"
#include "modules/builtin/config/config.h"
#include "modules/builtin/component_config_id.h"

#define CONFIG_DIR  _T("%appdata%\\r12f\\mirror")
#define CONFIG_FILE _T("config.ini")

CConfig::CConfig()
    : m_pCfgMap(CreateKVMap())
{
    TCHAR szDataPath[MAX_PATH + 1] = { 0 };
    ::ExpandEnvironmentStrings(CONFIG_DIR, szDataPath, MAX_PATH);
    ::PathAppend(szDataPath, CONFIG_FILE);
    m_strConfigFile = szDataPath;
}

CConfig::~CConfig()
{
}

STDMETHODIMP
CConfig::Load()
{
    TCHAR szBuffer[4096] = {0};
    GetPrivateProfileSectionNames(szBuffer, 4095, m_strConfigFile);

    DWORD i = 0, nSectionLength = 0;
    CComBSTR strSection;
    while(i < 4095) {
        strSection = &(szBuffer[i]);
        nSectionLength = strSection.Length();
        i += nSectionLength + 1;
        if(0 == nSectionLength) {
            break;
        }

        LoadConfigSectionFromFile(strSection);
    }

    return S_OK;
}

STDMETHODIMP
CConfig::Save()
{
    TCHAR szConfigDir[MAX_PATH + 1] = { 0 };
    ::ExpandEnvironmentStrings(CONFIG_DIR, szConfigDir, MAX_PATH);
    if (!Directory::Exists(szConfigDir)) {
        if (!Directory::Create(szConfigDir)) {
            return E_ACCESSDENIED;
        }
    }

    CComPtr<IKVMapEnumerator> pCfgEnum;
    if (FAILED(m_pCfgMap->CreateEnumerator(&pCfgEnum)) || NULL == pCfgEnum) {
        return E_OUTOFMEMORY;
    }

    while (!pCfgEnum->IsEnd()) {
        CComBSTR strSection;
        pCfgEnum->GetKey(&strSection);

        CComPtr<IKVValue> pSectionValue;
        pCfgEnum->GetValue(&pSectionValue);

        CComPtr<IKVMap> pSectionMap;
        pSectionValue->GetMap(&pSectionMap);

        CComPtr<IKVMapEnumerator> pSectionEnum;
        pSectionMap->CreateEnumerator(&pSectionEnum);

        while (!pSectionEnum->IsEnd()) {
            CComBSTR strKey, strValue;
            pSectionEnum->GetKey(&strKey);

            CComPtr<IKVValue> pValueNode;
            pSectionEnum->GetValue(&pValueNode);
            pValueNode->GetString(&strValue);

            WritePrivateProfileString(strSection, strKey, strValue, m_strConfigFile);

            pSectionEnum->Next();
        }

        pCfgEnum->Next();
    }

    return S_OK;
}

STDMETHODIMP
CConfig::AddProvider(IConfigProvider *pProvider)
{
    ASSERT_RETURN(NULL != pProvider, E_INVALIDARG);

    ProviderList::iterator oProviderIt = GetConfigProviderIt(pProvider);
    if(oProviderIt != m_vConfigProviders.end()) {
        return E_FAIL;
    }

    m_vConfigProviders.push_back(pProvider);

    return S_OK;
}

STDMETHODIMP
CConfig::RemoveProvider(IConfigProvider *pProvider)
{
    ASSERT_RETURN(NULL != pProvider, E_INVALIDARG);
    
    ProviderList::iterator oProviderIt = GetConfigProviderIt(pProvider);
    if(oProviderIt == m_vConfigProviders.end()) {
        return E_FAIL;
    }

    m_vConfigProviders.erase(oProviderIt);

    return S_OK;
}

STDMETHODIMP
CConfig::GetBool(BSTR strSection, BSTR strKey, BOOL *pValue)
{
    ASSERT_RETURN(NULL != pValue, E_INVALIDARG);

    CComBSTR strRawValue;
    HRESULT hr = GetRawConfig(strSection, strKey, &strRawValue);
    if(FAILED(hr) || NULL == strRawValue) {
        return E_FAIL;
    }

    INT nValue = _ttoi(strRawValue);
    *pValue = (0 != nValue);

    return S_OK;
}

STDMETHODIMP
CConfig::SetBool(BSTR strSection, BSTR strKey, BOOL bValue)
{
    CString strRawValue;
    strRawValue.Format(_T("%lu"), bValue);
    return SetRawConfig(strSection, strKey, CComBSTR(strRawValue));
}

STDMETHODIMP
CConfig::GetInt(BSTR strSection, BSTR strKey, INT *pValue)
{
    ASSERT_RETURN(NULL != pValue, E_INVALIDARG);

    CComBSTR strRawValue;
    HRESULT hr = GetRawConfig(strSection, strKey, &strRawValue);
    if(FAILED(hr) || NULL == strRawValue) {
        return E_FAIL;
    }

    INT nValue = _ttoi(strRawValue);
    *pValue = nValue;

    return S_OK;
}

STDMETHODIMP
CConfig::SetInt(BSTR strSection, BSTR strKey, INT nValue)
{
    CString strRawValue;
    strRawValue.Format(_T("%ld"), nValue);
    return SetRawConfig(strSection, strKey, CComBSTR(strRawValue));
}

STDMETHODIMP
CConfig::GetUInt(BSTR strSection, BSTR strKey, UINT *pValue)
{
    ASSERT_RETURN(NULL != pValue, E_INVALIDARG);

    CComBSTR strRawValue;
    HRESULT hr = GetRawConfig(strSection, strKey, &strRawValue);
    if(FAILED(hr) || NULL == strRawValue) {
        return E_FAIL;
    }

    UINT nValue = _tcstoul(strRawValue, 0, 10);
    *pValue = nValue;

    return S_OK;
}

STDMETHODIMP
CConfig::SetUInt(BSTR strSection, BSTR strKey, UINT nValue)
{
    CString strRawValue;
    strRawValue.Format(_T("%lu"), nValue);
    return SetRawConfig(strSection, strKey, CComBSTR(strRawValue));
}

STDMETHODIMP
CConfig::GetString(BSTR strSection, BSTR strKey, BSTR *pValue)
{
    ASSERT_RETURN(NULL != pValue, E_INVALIDARG);

    CComBSTR strRawValue;
    HRESULT hr = GetRawConfig(strSection, strKey, &strRawValue);
    if(FAILED(hr) || NULL == strRawValue) {
        return E_FAIL;
    }

    *pValue = strRawValue.Detach();

    return S_OK;
}

STDMETHODIMP
CConfig::SetString(BSTR strSection, BSTR strKey, BSTR strValue)
{
    return SetRawConfig(strSection, strKey, strValue);
}

HRESULT
CConfig::GetRawConfig(BSTR strSection, BSTR strKey, BSTR *pRawValue)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != pRawValue, E_INVALIDARG);
    ASSERT_RETURN(NULL != m_pCfgMap, E_FAIL);

    if(GetRawConfigFromCache(strSection, strKey, pRawValue) && NULL != (*pRawValue)) {
        return S_OK;
    }

    if(!GetConfigFromProvider(strSection, strKey, pRawValue) || NULL == (*pRawValue)) {
        return E_FAIL;
    }

    SetRawConfig(strSection, strKey, *pRawValue);

    return S_OK;
}

HRESULT
CConfig::SetRawConfig(BSTR strSection, BSTR strKey, BSTR strRawValue)
{
    ASSERT_RETURN(NULL != strSection, E_INVALIDARG);
    ASSERT_RETURN(NULL != strKey, E_INVALIDARG);
    ASSERT_RETURN(NULL != strRawValue, E_INVALIDARG);
    ASSERT_RETURN(NULL != m_pCfgMap, E_FAIL);

    if(SetConfigToProvider(strSection, strKey, strRawValue)) {
        return S_OK;
    }

    if(!SetConfigToCache(strSection, strKey, strRawValue)) {
        return E_FAIL;
    }

    return S_OK;
}

BOOL
CConfig::GetRawConfigFromCache(BSTR strSection, BSTR strKey, BSTR *pRawValue)
{
    CComPtr<IKVMap> pSection;
    if(FAILED(m_pCfgMap->GetMap(strSection, &pSection)) || NULL == pSection) {
        return FALSE;
    }
    
    if(FAILED(pSection->GetString(strKey, pRawValue)) || NULL == pRawValue) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CConfig::SetConfigToCache(BSTR strSection, BSTR strKey, BSTR strRawValue)
{
    CComPtr<IKVMap> pSection;
    if(FAILED(m_pCfgMap->GetMap(strSection, &pSection)) || NULL == pSection) {
        pSection = CreateKVMap();
        if(NULL == pSection) {
            return FALSE;
        }
        
        m_pCfgMap->SetMap(strSection, pSection);
    }

    if(FAILED(pSection->SetString(strKey, strRawValue))) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CConfig::GetConfigFromProvider(BSTR strSection, BSTR strKey, BSTR *pRawValue)
{
    ProviderList::iterator oProviderIt = m_vConfigProviders.begin();
    while(oProviderIt != m_vConfigProviders.end()) {
        if(SUCCEEDED((*oProviderIt)->GetRawConfig(strSection, strKey, pRawValue)) && NULL != pRawValue) {
            return TRUE;
        }
        ++oProviderIt;
    }

    return FALSE;
}

BOOL
CConfig::SetConfigToProvider(BSTR strSection, BSTR strKey, BSTR strRawValue)
{
    ProviderList::iterator oProviderIt = m_vConfigProviders.begin();
    while(oProviderIt != m_vConfigProviders.end()) {
        if(SUCCEEDED((*oProviderIt)->SetRawConfig(strSection, strKey, strRawValue))) {
            return TRUE;
        }
        ++oProviderIt;
    }

    return FALSE;
}

CConfig::ProviderList::iterator
CConfig::GetConfigProviderIt(IConfigProvider *pProvider)
{
    ProviderList::iterator oProviderIt = m_vConfigProviders.begin();
    while(oProviderIt != m_vConfigProviders.end()) {
        if((*oProviderIt).p == pProvider) {
            break;
        }
        ++oProviderIt;
    }

    return oProviderIt;
}

void
CConfig::LoadConfigSectionFromFile(BSTR strSection)
{
    CComPtr<IKVMap> pConfig = CreateKVMap();
    m_pCfgMap->SetMap(strSection, pConfig);

    CString strConfigBuffer;
    strConfigBuffer.GetBufferSetLength(4096);

    DWORD nConfigReadLength = 0, nConfigBufferLength = 0;
    do {
        nConfigBufferLength = strConfigBuffer.GetLength();
        nConfigReadLength = GetPrivateProfileSection(strSection, strConfigBuffer.GetBuffer(), nConfigBufferLength, m_strConfigFile);
        if(nConfigReadLength < nConfigBufferLength - 2) {
            break;
        }
        strConfigBuffer.GetBufferSetLength(nConfigBufferLength * 2);
    } while(1);

    DWORD i = 0, nConfigLength = 0;
    int nEqualPos = 0;
    CString strConfig;
    CComBSTR strKey, strValue;
    while(i < nConfigBufferLength) {
        strConfig = strConfigBuffer.GetBuffer() + i;
        nConfigLength = strConfig.GetLength();
        i += nConfigLength + 1;
        if(0 == nConfigLength) {
            break;
        }

        nEqualPos = strConfig.Find(_T("="));
        if(nEqualPos < 0) {
            continue;
        }

        strKey = strConfig.Left(nEqualPos);
        strValue = strConfig.Mid(nEqualPos + 1);
        pConfig->SetString(strKey, strValue);
    }

    return;
}
