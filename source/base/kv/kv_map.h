#pragma once

#include "base/com/simple_com_class.h"
#include "base/kv/kv.h"
#include <map>

typedef std::map<CComBSTR, CComPtr<IKVValue> > KVMapValueList;

class KVMapEnumerator :
    public SimpleComClassT<KVMapEnumerator, ComRef, IKVMapEnumerator>
{
public:
    KVMapEnumerator() : m_pValues(NULL) {}
    virtual ~KVMapEnumerator() {}

    HRESULT Init(IKVMap *pMap);

    STDMETHOD(Start)();
    STDMETHOD_(BOOL, IsBegin)();
    STDMETHOD_(BOOL, IsEnd)();
    STDMETHOD_(void, Prev)();
    STDMETHOD_(void, Next)();
    STDMETHOD(GetKey)(BSTR *pKey);
    STDMETHOD(GetValue)(IKVValue **ppValue);

private:
    CComPtr<IKVMap>             m_pMap;
    KVMapValueList              *m_pValues;
    KVMapValueList::iterator    m_oValueIt;
};

class KVMap :
    public SimpleComClassT<KVMap, ComRef, IKVMap>
{
    friend class KVMapEnumerator;

public:
    KVMap();
    virtual ~KVMap();

    STDMETHOD(CreateEnumerator)(IKVMapEnumerator **ppEnumerator);

    STDMETHOD_(UINT, GetCount)();
    STDMETHOD(RemoveValue)(BSTR strKey);

    STDMETHOD(GetValue)(BSTR strKey, IKVValue **ppValue);
    STDMETHOD(GetBool)(BSTR strKey, BOOL *pV);
    STDMETHOD(GetInt)(BSTR strKey, INT *pV);
    STDMETHOD(GetUInt)(BSTR strKey, UINT *pV);
    STDMETHOD(GetInt64)(BSTR strKey, INT64 *pV);
    STDMETHOD(GetUInt64)(BSTR strKey, UINT64 *pV);
    STDMETHOD(GetFloat)(BSTR strKey, float *pV);
    STDMETHOD(GetDouble)(BSTR strKey, double *pV);
    STDMETHOD(GetString)(BSTR strKey, BSTR *pV);
    STDMETHOD(GetBufferSize)(BSTR strKey, DWORD *pBufferSize);
    STDMETHOD(GetBuffer)(BSTR strKey, void *pBuffer, DWORD nBufferSize);
    STDMETHOD(GetPointer)(BSTR strKey, void **pV);
    STDMETHOD(GetIUnknown)(BSTR strKey, IUnknown **pV);
    STDMETHOD(GetComPtr)(BSTR strKey, REFIID riid, void **pV);
    STDMETHOD(GetArray)(BSTR strKey, IKVArray **pV);
    STDMETHOD(GetMap)(BSTR strKey, IKVMap **pV);

    STDMETHOD(SetValue)(BSTR strKey, IKVValue *pValue);
    STDMETHOD(SetBool)(BSTR strKey, BOOL v);
    STDMETHOD(SetInt)(BSTR strKey, INT v);
    STDMETHOD(SetUInt)(BSTR strKey, UINT v);
    STDMETHOD(SetInt64)(BSTR strKey, INT64 v);
    STDMETHOD(SetUInt64)(BSTR strKey, UINT64 v);
    STDMETHOD(SetFloat)(BSTR strKey, float v);
    STDMETHOD(SetDouble)(BSTR strKey, double v);
    STDMETHOD(SetString)(BSTR strKey, BSTR v);
    STDMETHOD(SetBuffer)(BSTR strKey, const void *pBuffer, DWORD nBufferSize);
    STDMETHOD(SetPointer)(BSTR strKey, const void *v);
    STDMETHOD(SetIUnknown)(BSTR strKey, IUnknown *v);
    STDMETHOD(SetArray)(BSTR strKey, IKVArray *v);
    STDMETHOD(SetMap)(BSTR strKey, IKVMap *v);

private:
    KVMapValueList m_vValues;
};