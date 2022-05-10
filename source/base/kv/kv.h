#pragma once

enum {
    KV_VALUE_TYPE_UNDEFINED     = 0,
    KV_VALUE_TYPE_BOOL,
    KV_VALUE_TYPE_INT,
    KV_VALUE_TYPE_UINT,
    KV_VALUE_TYPE_FLOAT,
    KV_VALUE_TYPE_STRING,
    KV_VALUE_TYPE_POINTER,
    KV_VALUE_TYPE_IUNKNOWN,
    KV_VALUE_TYPE_ARRAY,
    KV_VALUE_TYPE_MAP,
    KV_VALUE_TYPE_BUFFER,
};

__interface IKVArray;
__interface IKVMap;

MIDL_INTERFACE("9DE6A5EA-C5DA-4d4b-9CE8-DC45304766DD")
IKVValue : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetType(DWORD *pType) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE IsUndefined(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsBool(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsInt(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsUInt(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsFloat(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsString(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsBuffer(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsPointer(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsIUnknown(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsArray(BOOL *pB) = 0;
    virtual HRESULT STDMETHODCALLTYPE IsMap(BOOL *pB) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetRawBuffer(void **ppBuffer, DWORD *pBufferSize, DWORD *pDataSize) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetBool(BOOL *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt(INT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt(UINT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt64(INT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt64(UINT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFloat(float *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDouble(double *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetString(BSTR *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBufferSize(DWORD *pBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPointer(void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIUnknown(IUnknown **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComPtr(REFIID riid, void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetArray(IKVArray **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMap(IKVMap **pV) = 0;
    
    virtual HRESULT STDMETHODCALLTYPE SetBool(BOOL v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt(INT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt(UINT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt64(INT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt64(UINT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFloat(float v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDouble(double v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetString(BSTR v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBuffer(const void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPointer(const void *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetIUnknown(IUnknown *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetArray(IKVArray *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMap(IKVMap *v) = 0;
};

MIDL_INTERFACE("A1F16391-CE2A-4feb-B389-238E0493FEF8")
IKVArray : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetCount(INT *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddValue(INT *pIndex) = 0;
    virtual HRESULT STDMETHODCALLTYPE InsertValue(INT nIndex) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveValue(INT nIndex) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetValue(INT nIndex, void **pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBool(INT nIndex, BOOL *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt(INT nIndex, INT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt(INT nIndex, UINT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt64(INT nIndex, INT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt64(INT nIndex, UINT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFloat(INT nIndex, float *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDouble(INT nIndex, double *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetString(INT nIndex, BSTR *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBufferSize(INT nIndex, DWORD *pBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(INT nIndex, void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPointer(INT nIndex, void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIUnknown(INT nIndex, IUnknown **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComPtr(INT nIndex, REFIID riid, void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetArray(INT nIndex, IKVArray **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMap(INT nIndex, IKVMap **pV) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetValue(INT nIndex, IKVValue *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBool(INT nIndex, BOOL v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt(INT nIndex, INT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt(INT nIndex, UINT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt64(INT nIndex, INT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt64(INT nIndex, UINT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFloat(INT nIndex, float v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDouble(INT nIndex, double v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetString(INT nIndex, BSTR v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBuffer(INT nIndex, const void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPointer(INT nIndex, const void *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetIUnknown(INT nIndex, IUnknown *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetArray(INT nIndex, IKVArray *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMap(INT nIndex, IKVMap *v) = 0;
};

MIDL_INTERFACE("44BAF675-757B-4DF2-875C-E01546B22CEA")
IKVMapEnumerator : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Start() = 0;
    virtual BOOL STDMETHODCALLTYPE IsBegin() = 0;
    virtual BOOL STDMETHODCALLTYPE IsEnd() = 0;
    virtual void STDMETHODCALLTYPE Prev() = 0;
    virtual void STDMETHODCALLTYPE Next() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetKey(BSTR *pKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetValue(IKVValue **ppValue) = 0;
};

MIDL_INTERFACE("4D636124-BC31-4ee2-AA77-206C9E485413")
IKVMap : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CreateEnumerator(IKVMapEnumerator **ppEnumerator) = 0;

    virtual UINT STDMETHODCALLTYPE GetCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveValue(BSTR strKey) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetValue(BSTR strKey, IKVValue **ppValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBool(BSTR strKey, BOOL *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt(BSTR strKey, INT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt(BSTR strKey, UINT *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetInt64(BSTR strKey, INT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetUInt64(BSTR strKey, UINT64 *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetFloat(BSTR strKey, float *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDouble(BSTR strKey, double *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetString(BSTR strKey, BSTR *pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBufferSize(BSTR strKey, DWORD *pBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetBuffer(BSTR strKey, void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPointer(BSTR strKey, void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIUnknown(BSTR strKey, IUnknown **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetComPtr(BSTR strKey, REFIID riid, void **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetArray(BSTR strKey, IKVArray **pV) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetMap(BSTR strKey, IKVMap **pV) = 0;

    virtual HRESULT STDMETHODCALLTYPE SetValue(BSTR strKey, IKVValue *pValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBool(BSTR strKey, BOOL v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt(BSTR strKey, INT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt(BSTR strKey, UINT v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetInt64(BSTR strKey, INT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetUInt64(BSTR strKey, UINT64 v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetFloat(BSTR strKey, float v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetDouble(BSTR strKey, double v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetString(BSTR strKey, BSTR v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBuffer(BSTR strKey, const void *pBuffer, DWORD nBufferSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetPointer(BSTR strKey, const void *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetIUnknown(BSTR strKey, IUnknown *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetArray(BSTR strKey, IKVArray *v) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMap(BSTR strKey, IKVMap *v) = 0;
};

CComPtr<IKVValue> CreateKVBoolNode(BOOL v);
CComPtr<IKVValue> CreateKVIntNode(INT v);
CComPtr<IKVValue> CreateKVUIntNode(UINT v);
CComPtr<IKVValue> CreateKVInt64Node(INT64 v);
CComPtr<IKVValue> CreateKVUInt64Node(UINT64 v);
CComPtr<IKVValue> CreateKVStringNode(BSTR v);
CComPtr<IKVValue> CreateKVPointerNode(void *v);
CComPtr<IKVValue> CreateKVArrayNode(IKVArray *v);
CComPtr<IKVValue> CreateKVMapNode(IKVMap *v);
CComPtr<IKVArray> CreateKVArray();
CComPtr<IKVMap> CreateKVMap();