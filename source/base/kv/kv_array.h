#pragma once

#include "base/com/simple_com_class.h"
#include "base/kv/kv.h"
#include <vector>

class KVArray :
    public SimpleComClassT<KVArray, ComRef, IKVArray>
{
public:
    KVArray();
    virtual ~KVArray();

    STDMETHOD(GetCount)(INT *pCount);
    STDMETHOD(AddValue)(INT *pIndex);
    STDMETHOD(InsertValue)(INT nIndex);
    STDMETHOD(RemoveValue)(INT nIndex);

    STDMETHOD(GetValue)(INT nIndex, void **pValue);
    STDMETHOD(GetBool)(INT nIndex, BOOL *pV);
    STDMETHOD(GetInt)(INT nIndex, INT *pV);
    STDMETHOD(GetUInt)(INT nIndex, UINT *pV);
    STDMETHOD(GetInt64)(INT nIndex, INT64 *pV);
    STDMETHOD(GetUInt64)(INT nIndex, UINT64 *pV);
    STDMETHOD(GetFloat)(INT nIndex, float *pV);
    STDMETHOD(GetDouble)(INT nIndex, double *pV);
    STDMETHOD(GetString)(INT nIndex, BSTR *pV);
    STDMETHOD(GetBufferSize)(INT nIndex, DWORD *pBufferSize);
    STDMETHOD(GetBuffer)(INT nIndex, void *pBuffer, DWORD nBufferSize);
    STDMETHOD(GetPointer)(INT nIndex, void **pV);
    STDMETHOD(GetIUnknown)(INT nIndex, IUnknown **pV);
    STDMETHOD(GetComPtr)(INT nIndex, REFIID riid, void **pV);
    STDMETHOD(GetArray)(INT nIndex, IKVArray **pV);
    STDMETHOD(GetMap)(INT nIndex, IKVMap **pV);

    STDMETHOD(SetValue)(INT nIndex, IKVValue *pValue);
    STDMETHOD(SetBool)(INT nIndex, BOOL v);
    STDMETHOD(SetInt)(INT nIndex, INT v);
    STDMETHOD(SetUInt)(INT nIndex, UINT v);
    STDMETHOD(SetInt64)(INT nIndex, INT64 v);
    STDMETHOD(SetUInt64)(INT nIndex, UINT64 v);
    STDMETHOD(SetFloat)(INT nIndex, float v);
    STDMETHOD(SetDouble)(INT nIndex, double v);
    STDMETHOD(SetString)(INT nIndex, BSTR v);
    STDMETHOD(SetBuffer)(INT nIndex, const void *pBuffer, DWORD nBufferSize);
    STDMETHOD(SetPointer)(INT nIndex, const void *v);
    STDMETHOD(SetIUnknown)(INT nIndex, IUnknown *v);
    STDMETHOD(SetArray)(INT nIndex, IKVArray *v);
    STDMETHOD(SetMap)(INT nIndex, IKVMap *v);

private:
    std::vector<CComPtr<IKVValue> > m_vValues;
};