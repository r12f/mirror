#pragma once

#include <vector>
#include "base/com/com_class.h"
#include "base/kv/kv.h"
#include "modules/builtin/component_config.h"

class CConfig :
    public IConfig
{
    typedef std::vector<CComPtr<IConfigProvider>> ProviderList;

public:
    CConfig();
    virtual ~CConfig();

    COM_CLASS_FACTORY(CConfig)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(CConfig)
            QI_MAPPING(IConfig)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP Load();
    STDMETHODIMP Save();

    STDMETHODIMP AddProvider(IConfigProvider *pProvider);
    STDMETHODIMP RemoveProvider(IConfigProvider *pProvider);
    STDMETHODIMP GetBool(BSTR strSection, BSTR strKey, BOOL *pValue);
    STDMETHODIMP SetBool(BSTR strSection, BSTR strKey, BOOL bValue);
    STDMETHODIMP GetInt(BSTR strSection, BSTR strKey, INT *pValue);
    STDMETHODIMP SetInt(BSTR strSection, BSTR strKey, INT nValue);
    STDMETHODIMP GetUInt(BSTR strSection, BSTR strKey, UINT *pValue);
    STDMETHODIMP SetUInt(BSTR strSection, BSTR strKey, UINT nValue);
    STDMETHODIMP GetString(BSTR strSection, BSTR strKey, BSTR *pValue);
    STDMETHODIMP SetString(BSTR strSection, BSTR strKey, BSTR strValue);

protected:
    HRESULT GetRawConfig(BSTR strSection, BSTR strKey, BSTR *pRawValue);
    HRESULT SetRawConfig(BSTR strSection, BSTR strKey, BSTR strRawValue);

    BOOL GetRawConfigFromCache(BSTR strSection, BSTR strKey, BSTR *pRawValue);
    BOOL SetConfigToCache(BSTR strSection, BSTR strKey, BSTR strRawValue);

    BOOL GetConfigFromProvider(BSTR strSection, BSTR strKey, BSTR *pRawValue);
    BOOL SetConfigToProvider(BSTR strSection, BSTR strKey, BSTR strRawValue);
    ProviderList::iterator GetConfigProviderIt(IConfigProvider *pProvider);

    BOOL SetRawConfigToFile(BSTR strSection, BSTR strKey, BSTR strRawValue);

    void LoadConfigSectionFromFile(BSTR strSection);

private:
    CComPtr<IKVMap> m_pCfgMap;
    ProviderList m_vConfigProviders;
    CString m_strConfigFile;
};