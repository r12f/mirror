#include "stdafx.h"
#include "base/kv/kv_array.h"
#include "base/kv/kv_value.h"

KVArray::KVArray()
{

}

KVArray::~KVArray()
{

}

HRESULT
KVArray::GetCount(INT *pCount)
{
    if(NULL == pCount) {
        return E_INVALIDARG;
    }

    *pCount = (INT)m_vValues.size();

    return S_OK;
}

HRESULT
KVArray::AddValue(INT *pIndex)
{
    CComPtr<IKVValue> spValue;
    if(FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {
        return E_FAIL;
    }

    m_vValues.push_back(spValue);

    return S_OK;
}

HRESULT
KVArray::InsertValue(INT nIndex)
{
    CComPtr<IKVValue> spValue;
    if(FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {
        return E_FAIL;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    m_vValues.insert(m_vValues.begin() + nIndex, spValue);

    return S_OK;
}

HRESULT
KVArray::RemoveValue(INT nIndex)
{
    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    m_vValues.erase(m_vValues.begin() + nIndex);

    return S_OK;
}

HRESULT
KVArray::GetValue(INT nIndex, void **pValue)
{
    if(NULL == pValue) {
        return E_INVALIDARG;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue = m_vValues[nIndex];
    *pValue = spValue.Detach();

    return S_OK;
}

#define KV_ARRAY_GETTER_FUNC_TEMPLATE(func, vt)         \
    HRESULT                                             \
    KVArray::func(INT nIndex, vt *pV)               \
    {                                                   \
        if(NULL == pV) {                                \
            return E_INVALIDARG;                        \
        }                                               \
                                                        \
        INT nArraySize = (INT)m_vValues.size();         \
        if(nIndex >= nArraySize) {                      \
            return E_FAIL;                              \
        }                                               \
                                                        \
        CComPtr<IKVValue> spValue = m_vValues[nIndex];  \
        return spValue->func(pV);                       \
    }

KV_ARRAY_GETTER_FUNC_TEMPLATE(GetBool, BOOL)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetInt, INT)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetUInt, UINT)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetInt64, INT64)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetUInt64, UINT64)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetFloat, float)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetDouble, double)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetString, BSTR)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetBufferSize, DWORD)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetPointer, void *)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetIUnknown, IUnknown *)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetArray, IKVArray *)
KV_ARRAY_GETTER_FUNC_TEMPLATE(GetMap, IKVMap *)

#undef KV_ARRAY_GETTER_FUNC_TEMPLATE

HRESULT
KVArray::GetBuffer(INT nIndex, void *pBuffer, DWORD nBufferSize)
{
    if(NULL == pBuffer || 0 == nBufferSize) {
        return E_INVALIDARG;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue = m_vValues[nIndex];
    return spValue->GetBuffer(pBuffer, nBufferSize);
}

HRESULT
KVArray::GetComPtr(INT nIndex, REFIID riid, void **pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue = m_vValues[nIndex];
    return spValue->GetComPtr(riid, pV);
}

HRESULT
KVArray::SetValue(INT nIndex, IKVValue *pValue)
{
    if(NULL == pValue) {
        return E_INVALIDARG;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    m_vValues[nIndex] = pValue;

    return S_OK;
}

#define KV_ARRAY_SETTER_FUNC_TEMPLATE(func, vt)     \
    HRESULT                                         \
    KVArray::func(INT nIndex, vt v)             \
    {                                               \
        INT nArraySize = (INT)m_vValues.size();     \
        if(nIndex >= nArraySize) {                  \
            return E_FAIL;                          \
        }                                           \
                                                    \
        CComPtr<IKVValue> spValue;                  \
        if(FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {   \
            return E_FAIL;                          \
        }                                           \
                                                    \
        HRESULT hr = spValue->func(v);              \
        if(FAILED(hr)) {                            \
            return hr;                              \
        }                                           \
                                                    \
        m_vValues[nIndex] = spValue;                \
                                                    \
        return S_OK;                                \
    }

KV_ARRAY_SETTER_FUNC_TEMPLATE(SetBool, BOOL)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetInt, INT)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetUInt, UINT)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetInt64, INT64)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetUInt64, UINT64)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetFloat, float)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetDouble, double)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetString, BSTR)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetPointer, const void *)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetIUnknown, IUnknown *)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetArray, IKVArray *)
KV_ARRAY_SETTER_FUNC_TEMPLATE(SetMap, IKVMap *)

#undef KV_ARRAY_SETTER_FUNC_TEMPLATE

HRESULT
KVArray::SetBuffer(INT nIndex, const void *pBuffer, DWORD nBufferSize)
{
    if(NULL == pBuffer || 0 == nBufferSize) {
        return E_INVALIDARG;
    }

    INT nArraySize = (INT)m_vValues.size();
    if(nIndex >= nArraySize) {
        return E_FAIL;
    }

    CComPtr<IKVValue> spValue;
    if(FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {
        return E_FAIL;
    }

    HRESULT hr = spValue->SetBuffer(pBuffer, nBufferSize);
    if(FAILED(hr)) {
        return hr;
    }

    m_vValues[nIndex] = spValue;

    return S_OK;
}

