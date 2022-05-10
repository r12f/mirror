#pragma once

#include "base/com/simple_com_class.h"
#include "base/kv/kv.h"

class KVValue :
    public SimpleComClassT<KVValue, ComRef, IKVValue>
{
public:
    KVValue();
    virtual ~KVValue();

    STDMETHOD(GetType)(DWORD *pType);

    STDMETHOD(IsUndefined)(BOOL *pB);
    STDMETHOD(IsBool)(BOOL *pB);
    STDMETHOD(IsInt)(BOOL *pB);
    STDMETHOD(IsUInt)(BOOL *pB);
    STDMETHOD(IsFloat)(BOOL *pB);
    STDMETHOD(IsString)(BOOL *pB);
    STDMETHOD(IsBuffer)(BOOL *pB);
    STDMETHOD(IsPointer)(BOOL *pB);
    STDMETHOD(IsIUnknown)(BOOL *pB);
    STDMETHOD(IsArray)(BOOL *pB);
    STDMETHOD(IsMap)(BOOL *pB);

    STDMETHOD(GetRawBuffer)(void **ppBuffer, DWORD *pBufferSize, DWORD *pDataSize);

    STDMETHOD(GetBool)(BOOL *pV);
    STDMETHOD(GetInt)(INT *pV);
    STDMETHOD(GetUInt)(UINT *pV);
    STDMETHOD(GetInt64)(INT64 *pV);
    STDMETHOD(GetUInt64)(UINT64 *pV);
    STDMETHOD(GetFloat)(float *pV);
    STDMETHOD(GetDouble)(double *pV);
    STDMETHOD(GetString)(BSTR *pV);
    STDMETHOD(GetBufferSize)(DWORD *pBufferSize);
    STDMETHOD(GetBuffer)(void *pBuffer, DWORD nBufferSize);
    STDMETHOD(GetPointer)(void **pV);
    STDMETHOD(GetIUnknown)(IUnknown **pV);
    STDMETHOD(GetComPtr)(REFIID riid, void **pV);
    STDMETHOD(GetArray)(IKVArray **pV);
    STDMETHOD(GetMap)(IKVMap **pV);

    STDMETHOD(SetBool)(BOOL v);
    STDMETHOD(SetInt)(INT v);
    STDMETHOD(SetUInt)(UINT v);
    STDMETHOD(SetInt64)(INT64 v);
    STDMETHOD(SetUInt64)(UINT64 v);
    STDMETHOD(SetFloat)(float v);
    STDMETHOD(SetDouble)(double v);
    STDMETHOD(SetString)(BSTR v);
    STDMETHOD(SetBuffer)(const void *pBuffer, DWORD nBufferSize);
    STDMETHOD(SetPointer)(const void *v);
    STDMETHOD(SetIUnknown)(IUnknown *v);
    STDMETHOD(SetArray)(IKVArray *v);
    STDMETHOD(SetMap)(IKVMap *v);

protected:
    void Reset();
    BOOL EnsureBufferSizeNoCopy(DWORD nBufferSize);

private:
    DWORD m_nType;
    VOID *m_pDataPtr;
    DWORD m_nDataBufferSize;
    DWORD m_nDataSize;
    char m_vDataBuffer[8];
};