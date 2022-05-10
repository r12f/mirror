#pragma once

#include "base/kv/kv.h"
#include "modules/builtin/component_config_id.h"

MIDL_INTERFACE("A9294CF2-64CA-4840-A64E-9433CDB47610")
IConfigProvider : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetRawConfig(BSTR strSection, BSTR strKey, BSTR *pRawValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRawConfig(BSTR strSection, BSTR strKey, BSTR strRawValue) = 0;
};

MIDL_INTERFACE("8204C291-17F8-4254-9662-AC642DEE4EFC")
IConfig : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Load();
    virtual HRESULT STDMETHODCALLTYPE Save();

    virtual HRESULT STDMETHODCALLTYPE AddProvider(IConfigProvider *pProvider) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveProvider(IConfigProvider *pProvider) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBool(BSTR strSection, BSTR strKey, BOOL *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBool(BSTR strSection, BSTR strKey, BOOL bValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt(BSTR strSection, BSTR strKey, INT *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt(BSTR strSection, BSTR strKey, INT nValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt(BSTR strSection, BSTR strKey, UINT *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt(BSTR strSection, BSTR strKey, UINT nValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetString(BSTR strSection, BSTR strKey, BSTR *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetString(BSTR strSection, BSTR strKey, BSTR strValue) = 0;
};