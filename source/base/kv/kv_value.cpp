#include "stdafx.h"
#include "base/kv/kv_value.h"

KVValue::KVValue()
    : m_nType(KV_VALUE_TYPE_UNDEFINED)
    , m_pDataPtr(m_vDataBuffer)
    , m_nDataBufferSize(sizeof(m_vDataBuffer))
    , m_nDataSize(0)
{
    memset(m_vDataBuffer, 0, sizeof(m_vDataBuffer));
}

KVValue::~KVValue()
{
    Reset();

    if(NULL != m_pDataPtr && m_pDataPtr != m_vDataBuffer) {
        delete [] m_pDataPtr;
        m_pDataPtr = NULL;
    }
}

HRESULT
KVValue::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = m_nType;

    return S_OK;
}

#define KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(func, t)      \
    HRESULT                                                 \
    KVValue::func(BOOL *pB)                             \
    {                                                       \
        if(NULL == pB) {                                    \
            return E_INVALIDARG;                            \
        }                                                   \
                                                            \
        *pB = (m_nType == t);                               \
                                                            \
        return S_OK;                                        \
    }

KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsUndefined, KV_VALUE_TYPE_UNDEFINED)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsBool, KV_VALUE_TYPE_BOOL)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsInt, KV_VALUE_TYPE_INT)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsUInt, KV_VALUE_TYPE_UINT)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsFloat, KV_VALUE_TYPE_FLOAT)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsString, KV_VALUE_TYPE_STRING)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsBuffer, KV_VALUE_TYPE_BUFFER)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsPointer, KV_VALUE_TYPE_POINTER)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsIUnknown, KV_VALUE_TYPE_IUNKNOWN)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsArray, KV_VALUE_TYPE_ARRAY)
KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE(IsMap, KV_VALUE_TYPE_MAP)

#undef KV_VALUE_IMPL_TYPE_TEST_FUNC_TEMPLATE

HRESULT
KVValue::GetRawBuffer(void **ppBuffer, DWORD *pBufferSize, DWORD *pDataSize)
{
    if(NULL == ppBuffer || NULL == pDataSize || NULL == pBufferSize) {
        return E_INVALIDARG;
    }

    *ppBuffer = m_pDataPtr;
    *pBufferSize = m_nDataBufferSize;
    *pDataSize = m_nDataSize;

    return S_OK;
}

#define KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(func, vt, dt, t)                     \
    HRESULT                                                                     \
    KVValue::func(vt *pV)                                                   \
    {                                                                           \
        if(NULL == pV) {                                                        \
            return E_INVALIDARG;                                                \
        }                                                                       \
                                                                                \
        if(NULL == m_pDataPtr || t != m_nType || m_nDataSize < sizeof(vt)) {    \
            return E_FAIL;                                                      \
        }                                                                       \
                                                                                \
        dt *p = (dt *)m_pDataPtr;                                               \
        *pV = (vt)*p;                                                           \
                                                                                \
        return S_OK;                                                            \
    }

KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetBool, BOOL, BOOL, KV_VALUE_TYPE_BOOL)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetInt, INT, INT64, KV_VALUE_TYPE_INT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetUInt, UINT, UINT64, KV_VALUE_TYPE_UINT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetInt64, INT64, INT64, KV_VALUE_TYPE_INT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetUInt64, UINT64, UINT64, KV_VALUE_TYPE_UINT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetFloat, float, float, KV_VALUE_TYPE_FLOAT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetDouble, double, double, KV_VALUE_TYPE_FLOAT)
KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE(GetPointer, void *, void *, KV_VALUE_TYPE_POINTER)

#undef KV_VALUE_IMPL_GETTER_FUNC_TEMPLATE

HRESULT
KVValue::GetString(BSTR *pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_STRING != m_nType || m_nDataSize < sizeof(BSTR)) {
        return E_FAIL;
    }

    CComBSTR str = (*(BSTR *)(m_pDataPtr));
    *pV = str.Detach();

    return S_OK;
}

HRESULT
KVValue::GetBufferSize(DWORD *pBufferSize)
{
    if(NULL == pBufferSize) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_BUFFER != m_nType) {
        return E_FAIL;
    }

    *pBufferSize = m_nDataSize;

    return S_OK;
}

HRESULT
KVValue::GetBuffer(void *pBuffer, DWORD nBufferSize)
{
    if(NULL == pBuffer || 0 == nBufferSize) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_BUFFER != m_nType) {
        return E_FAIL;
    }

    DWORD nCopySize = m_nDataSize < nBufferSize ? m_nDataSize : nBufferSize;
    if(0 < nCopySize) {
        memcpy_s(pBuffer, nBufferSize, m_pDataPtr, nCopySize);
    }

    return S_OK;
}

HRESULT
KVValue::GetIUnknown(IUnknown **pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_IUNKNOWN != m_nType || m_nDataSize < sizeof(void *)) {
        return E_FAIL;
    }

    IUnknown **pDataPtr = (IUnknown **)(m_pDataPtr);
    if(NULL != *pDataPtr) {
        (*pDataPtr)->AddRef();
    }

    *pV = *pDataPtr;

    return S_OK;
}

HRESULT
KVValue::GetComPtr(REFIID riid, void **pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_IUNKNOWN != m_nType || m_nDataSize < sizeof(void *)) {
        return E_FAIL;
    }

    *pV = NULL;

    IUnknown **pDataPtr = (IUnknown **)(m_pDataPtr);
    if(NULL == *pDataPtr) {
        return S_OK;
    }

    return (*pDataPtr)->QueryInterface(riid, pV);
}

HRESULT
KVValue::GetArray(IKVArray **pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_ARRAY != m_nType || m_nDataSize < sizeof(IKVArray *)) {
        return E_FAIL;
    }

    IKVArray **pDataPtr = (IKVArray **)(m_pDataPtr);
    if(NULL != *pDataPtr) {
        (*pDataPtr)->AddRef();
    }

    *pV = *pDataPtr;

    return S_OK;
}

HRESULT
KVValue::GetMap(IKVMap **pV)
{
    if(NULL == pV) {
        return E_INVALIDARG;
    }

    if(NULL == m_pDataPtr || KV_VALUE_TYPE_MAP != m_nType || m_nDataSize < sizeof(IKVMap *)) {
        return E_FAIL;
    }

    IKVMap **pDataPtr = (IKVMap **)(m_pDataPtr);
    if(NULL != *pDataPtr) {
        (*pDataPtr)->AddRef();
    }

    *pV = *pDataPtr;

    return S_OK;
}

#define KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(func, vt, dt, t) \
    HRESULT                                                 \
    KVValue::func(vt v)                                 \
    {                                                       \
        if(NULL == m_pDataPtr) {                            \
            return E_FAIL;                                  \
        }                                                   \
                                                            \
        Reset();                                            \
                                                            \
        if(!EnsureBufferSizeNoCopy(sizeof(vt))) {           \
            return E_FAIL;                                  \
        }                                                   \
                                                            \
        dt *p = (dt *)m_pDataPtr;                           \
        *p = (dt)v;                                         \
        m_nType = t;                                        \
        m_nDataSize = sizeof(vt);                           \
                                                            \
        return S_OK;                                        \
    }

KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetBool, BOOL, BOOL, KV_VALUE_TYPE_BOOL)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetInt, INT, INT64, KV_VALUE_TYPE_INT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetUInt, UINT, UINT64, KV_VALUE_TYPE_UINT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetInt64, INT64, INT64, KV_VALUE_TYPE_INT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetUInt64, UINT64, UINT64, KV_VALUE_TYPE_UINT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetFloat, float, double, KV_VALUE_TYPE_FLOAT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetDouble, double, double, KV_VALUE_TYPE_FLOAT)
KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE(SetPointer, const void *, const void *, KV_VALUE_TYPE_POINTER)

#undef KV_VALUE_IMPL_SETTER_FUNC_TEMPLATE


HRESULT
KVValue::SetString(BSTR v)
{
    if(NULL == v) {
        return E_INVALIDARG;
    }

    Reset();

    CComBSTR str = v;
    if(!EnsureBufferSizeNoCopy(sizeof(BSTR))) {
        return E_FAIL;
    }

    BSTR *p = (BSTR *)m_pDataPtr;
    *p = str.Detach();
    m_nType = KV_VALUE_TYPE_STRING;
    m_nDataSize = sizeof(BSTR *);

    return S_OK;
}

HRESULT
KVValue::SetBuffer(const void *pBuffer, DWORD nBufferSize)
{
    if(NULL == pBuffer) {
        return E_INVALIDARG;
    }

    Reset();

    if(!EnsureBufferSizeNoCopy(nBufferSize)) {
        return E_FAIL;
    }

    if(nBufferSize > 0) {
        memcpy_s(m_pDataPtr, m_nDataBufferSize, pBuffer, nBufferSize);
    }

    m_nType = KV_VALUE_TYPE_BUFFER;
    m_nDataSize = nBufferSize;

    return S_OK;
}

HRESULT
KVValue::SetIUnknown(IUnknown *v)
{
    Reset();

    if(!EnsureBufferSizeNoCopy(sizeof(IUnknown *))) {
        return E_FAIL;
    }

    if(NULL != v) {
        v->AddRef();
    }

    IUnknown **ppData = (IUnknown **)m_pDataPtr;
    *ppData = v;
    m_nType = KV_VALUE_TYPE_ARRAY;
    m_nDataSize = sizeof(IUnknown *);

    return S_OK;
}

HRESULT
KVValue::SetArray(IKVArray *v)
{
    Reset();

    if(!EnsureBufferSizeNoCopy(sizeof(IKVArray *))) {
        return E_FAIL;
    }

    if(NULL != v) {
        v->AddRef();
    }

    IKVArray **ppData = (IKVArray **)m_pDataPtr;
    *ppData = v;
    m_nType = KV_VALUE_TYPE_ARRAY;
    m_nDataSize = sizeof(IKVArray *);

    return S_OK;
}

HRESULT
KVValue::SetMap(IKVMap *v)
{
    Reset();

    if(NULL != v) {
        v->AddRef();
    }

    IKVMap **ppData = (IKVMap **)m_pDataPtr;
    *ppData = v;
    m_nType = KV_VALUE_TYPE_MAP;
    m_nDataSize = sizeof(IKVMap *);

    return S_OK;
}

void
KVValue::Reset()
{
    switch(m_nType) {
    case KV_VALUE_TYPE_STRING:
        {
            CComBSTR str;
            BSTR *ppV = (BSTR *)(m_pDataPtr);
            str.Attach(*ppV);
            *ppV = 0;
        }
        break;
    case KV_VALUE_TYPE_IUNKNOWN:
        {
            IUnknown **ppV = (IUnknown **)m_pDataPtr;
            (*ppV)->Release();
            (*ppV) = 0;
        }
        break;
    case KV_VALUE_TYPE_ARRAY:
        {
            IKVArray **ppV = (IKVArray **)m_pDataPtr;
            (*ppV)->Release();
            (*ppV) = 0;
        }
        break;
    case KV_VALUE_TYPE_MAP:
        {
            IKVMap **ppV = (IKVMap **)m_pDataPtr;
            (*ppV)->Release();
            (*ppV) = 0;
        }
        break;
    }

    m_nType = KV_VALUE_TYPE_UNDEFINED;
    m_nDataSize = 0;
}

BOOL
KVValue::EnsureBufferSizeNoCopy(DWORD nBufferSize)
{
    if(m_nDataBufferSize >= nBufferSize) {
        return TRUE;
    }

    VOID *pNewDataPtr = new char[nBufferSize + 1];
    if(NULL == pNewDataPtr) {
        return FALSE;
    }
    
    if(m_pDataPtr != m_vDataBuffer) {
        delete m_pDataPtr;
        m_pDataPtr = NULL;
    }

    m_pDataPtr = pNewDataPtr;
    m_nDataBufferSize = nBufferSize;
    m_nDataSize = 0;

    return TRUE;
}