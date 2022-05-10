#include "stdafx.h"
#include "base/kv/kv_map.h"
#include "base/kv/kv_value.h"

HRESULT
KVMapEnumerator::Init(IKVMap *pMap)
{
    if (NULL == pMap) {
        return E_INVALIDARG;
    }

    m_pMap = pMap;

    KVMap *pRawMap = static_cast<KVMap *>(pMap);
    m_pValues = &(pRawMap->m_vValues);
    m_oValueIt = m_pValues->begin();

    return S_OK;
}

HRESULT
KVMapEnumerator::Start()
{
    if (NULL == m_pMap || NULL == m_pValues) {
        return E_FAIL;
    }

    m_oValueIt = m_pValues->begin();

    return S_OK;
}

BOOL
KVMapEnumerator::IsBegin()
{
    if (NULL == m_pMap || NULL == m_pValues) {
        return FALSE;
    }

    return (m_oValueIt == m_pValues->begin());
}

BOOL
KVMapEnumerator::IsEnd()
{
    if (NULL == m_pMap || NULL == m_pValues) {
        return FALSE;
    }

    return (m_oValueIt == m_pValues->end());
}

void
KVMapEnumerator::Prev()
{
    if (NULL == m_pMap || NULL == m_pValues) {
        return;
    }

    --m_oValueIt;

    return;
}

void
KVMapEnumerator::Next()
{
    if (NULL == m_pMap || NULL == m_pValues) {
        return;
    }

    ++m_oValueIt;

    return;
}

HRESULT
KVMapEnumerator::GetKey(BSTR *pKey)
{
    if (NULL == pKey) {
        return E_POINTER;
    }

    if (NULL == m_pMap || NULL == m_pValues) {
        return E_FAIL;
    }

    return m_oValueIt->first.CopyTo(pKey);
}

HRESULT
KVMapEnumerator::GetValue(IKVValue **ppValue)
{
    if (NULL == ppValue) {
        return E_POINTER;
    }

    if (NULL == m_pMap || NULL == m_pValues) {
        return E_FAIL;
    }

    return m_oValueIt->second.CopyTo(ppValue);
}

KVMap::KVMap()
{

}

KVMap::~KVMap()
{

}

HRESULT
KVMap::CreateEnumerator(IKVMapEnumerator **ppEnumerator)
{
    if (NULL == ppEnumerator) {
        return E_POINTER;
    }

    CComPtr<IKVMapEnumerator> pEnumerator;
    HRESULT hr = KVMapEnumerator::CreateObject(__uuidof(IKVMapEnumerator), (void **)&pEnumerator);

    KVMapEnumerator *pRawEnumerator = static_cast<KVMapEnumerator *>(pEnumerator.p);
    hr = pRawEnumerator->Init(this);
    if (FAILED(hr)) {
        return hr;
    }

    *ppEnumerator = pEnumerator.Detach();

    return S_OK;
}

UINT
KVMap::GetCount()
{
    return (UINT) m_vValues.size();
}

HRESULT
KVMap::RemoveValue(BSTR strKey)
{
    if (NULL == strKey) {
        return E_INVALIDARG;
    }

    CComBSTR spKey;
    spKey.Attach(strKey);
    KVMapValueList::iterator oValueIt = m_vValues.find(spKey);
    spKey.Detach();

    if (oValueIt == m_vValues.end()) {
        return E_FAIL;
    }

    m_vValues.erase(oValueIt);

    return S_OK;
}

HRESULT
KVMap::GetValue(BSTR strKey, IKVValue **ppValue)
{
    if (NULL == strKey || NULL == ppValue) {
        return E_INVALIDARG;
    }

    CComBSTR spKey;
    spKey.Attach(strKey);
    KVMapValueList::iterator oValueIt = m_vValues.find(spKey);
    spKey.Detach();

    if (oValueIt == m_vValues.end()) {
        return E_FAIL;
    }

    CComPtr<IKVValue> sppValue = oValueIt->second;
    *ppValue = sppValue.Detach();

    return S_OK;
}

#define KV_MAP_GETTER_FUNC_TEMPLATE(func, vt)                   \
    HRESULT                                                     \
    KVMap::func(BSTR strKey, vt *pV)                            \
    {                                                           \
        if (NULL == strKey || NULL == pV) {                     \
        return E_INVALIDARG;                                    \
    }                                                           \
                                                                \
    CComBSTR spKey;                                             \
    spKey.Attach(strKey);                                       \
    KVMapValueList::iterator oValueIt = m_vValues.find(spKey);  \
    spKey.Detach();                                             \
                                                                \
    if (oValueIt == m_vValues.end()) {                          \
        return E_FAIL;                                          \
    }                                                           \
                                                                \
    CComPtr<IKVValue> spValue = oValueIt->second;               \
    return spValue->func(pV);                                   \
}


KV_MAP_GETTER_FUNC_TEMPLATE(GetBool, BOOL)
KV_MAP_GETTER_FUNC_TEMPLATE(GetInt, INT)
KV_MAP_GETTER_FUNC_TEMPLATE(GetUInt, UINT)
KV_MAP_GETTER_FUNC_TEMPLATE(GetInt64, INT64)
KV_MAP_GETTER_FUNC_TEMPLATE(GetUInt64, UINT64)
KV_MAP_GETTER_FUNC_TEMPLATE(GetFloat, float)
KV_MAP_GETTER_FUNC_TEMPLATE(GetDouble, double)
KV_MAP_GETTER_FUNC_TEMPLATE(GetString, BSTR)
KV_MAP_GETTER_FUNC_TEMPLATE(GetBufferSize, DWORD)
KV_MAP_GETTER_FUNC_TEMPLATE(GetPointer, void *)
KV_MAP_GETTER_FUNC_TEMPLATE(GetIUnknown, IUnknown *)
KV_MAP_GETTER_FUNC_TEMPLATE(GetArray, IKVArray *)
KV_MAP_GETTER_FUNC_TEMPLATE(GetMap, IKVMap *)

#undef KV_MAP_GETTER_FUNC_TEMPLATE

HRESULT
KVMap::GetBuffer(BSTR strKey, void *pBuffer, DWORD nBufferSize)
{
    if (NULL == strKey || NULL == pBuffer || 0 == nBufferSize) {
        return E_INVALIDARG;
    }

    CComBSTR spKey;
    spKey.Attach(strKey);
    KVMapValueList::iterator oValueIt = m_vValues.find(spKey);
    spKey.Detach();

    if (oValueIt == m_vValues.end()) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue = oValueIt->second;
    return spValue->GetBuffer(pBuffer, nBufferSize);
}

HRESULT
KVMap::GetComPtr(BSTR strKey, REFIID riid, void **pV)
{
    if (NULL == strKey || NULL == pV) {
        return E_INVALIDARG;
    }

    CComBSTR spKey;
    spKey.Attach(strKey);
    KVMapValueList::iterator oValueIt = m_vValues.find(spKey);
    spKey.Detach();

    if (oValueIt == m_vValues.end()) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue = oValueIt->second;
    return spValue->GetComPtr(riid, pV);
}

HRESULT
KVMap::SetValue(BSTR strKey, IKVValue *pValue)
{
    if (NULL == strKey || NULL == pValue) {
        return E_INVALIDARG;
    }

    m_vValues[CComBSTR(strKey)] = pValue;

    return S_OK;
}

#define KV_MAP_SETTER_FUNC_TEMPLATE(func, vt)       \
    HRESULT                                         \
    KVMap::func(BSTR strKey, vt v)                  \
    {                                               \
    if (NULL == strKey) {                           \
        return E_INVALIDARG;                        \
    }                                               \
                                                    \
    CComPtr<IKVValue> spValue;                      \
    if (FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) { \
        return E_FAIL;                              \
    }                                               \
                                                    \
    HRESULT hr = spValue->func(v);                  \
    if (FAILED(hr)) {                               \
        return hr;                                  \
    }                                               \
                                                    \
    m_vValues[CComBSTR(strKey)] = spValue;          \
                                                    \
    return S_OK;                                    \
}

KV_MAP_SETTER_FUNC_TEMPLATE(SetBool, BOOL)
KV_MAP_SETTER_FUNC_TEMPLATE(SetInt, INT)
KV_MAP_SETTER_FUNC_TEMPLATE(SetUInt, UINT)
KV_MAP_SETTER_FUNC_TEMPLATE(SetInt64, INT64)
KV_MAP_SETTER_FUNC_TEMPLATE(SetUInt64, UINT64)
KV_MAP_SETTER_FUNC_TEMPLATE(SetFloat, float)
KV_MAP_SETTER_FUNC_TEMPLATE(SetDouble, double)
KV_MAP_SETTER_FUNC_TEMPLATE(SetString, BSTR)
KV_MAP_SETTER_FUNC_TEMPLATE(SetPointer, const void *)
KV_MAP_SETTER_FUNC_TEMPLATE(SetIUnknown, IUnknown *)
KV_MAP_SETTER_FUNC_TEMPLATE(SetArray, IKVArray *)
KV_MAP_SETTER_FUNC_TEMPLATE(SetMap, IKVMap *)

#undef KV_MAP_SETTER_FUNC_TEMPLATE

HRESULT
KVMap::SetBuffer(BSTR strKey, const void *pBuffer, DWORD nBufferSize)
{
    if (NULL == strKey || NULL == pBuffer || 0 == nBufferSize) {
        return E_INVALIDARG;
    }

    CComPtr<IKVValue> spValue;
    if (FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {
        return E_FAIL;
    }

    HRESULT hr = spValue->SetBuffer(pBuffer, nBufferSize);
    if (FAILED(hr)) {
        return hr;
    }

    m_vValues[CComBSTR(strKey)] = spValue;

    return S_OK;
}
