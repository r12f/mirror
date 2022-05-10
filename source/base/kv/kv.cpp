#include "stdafx.h"
#include "base/kv/kv.h"
#include "base/kv/kv_value.h"
#include "base/kv/kv_array.h"
#include "base/kv/kv_map.h"

#define KV_FACTROY_TEMPLATE(func, setfunc, vt)      \
    CComPtr<IKVValue>                               \
    func(vt v)                                      \
    {                                               \
        CComPtr<IKVValue> spValue;                  \
        if(FAILED(KVValue::CreateObject(__uuidof(IKVValue), (void **)&spValue)) || NULL == spValue) {       \
            return NULL;                            \
        }                                           \
                                                    \
        HRESULT hr = spValue->setfunc(v);           \
        if(FAILED(hr)) {                            \
            return NULL;                            \
        }                                           \
                                                    \
        return spValue;                             \
    }

KV_FACTROY_TEMPLATE(CreateKVBoolNode, SetBool, BOOL)
KV_FACTROY_TEMPLATE(CreateKVIntNode, SetInt, INT)
KV_FACTROY_TEMPLATE(CreateKVUIntNode, SetUInt, UINT)
KV_FACTROY_TEMPLATE(CreateKVInt64Node, SetInt64, INT64)
KV_FACTROY_TEMPLATE(CreateKVUInt64Node, SetUInt64, UINT64)
KV_FACTROY_TEMPLATE(CreateKVStringNode, SetString, BSTR)
KV_FACTROY_TEMPLATE(CreateKVPointerNode, SetPointer, void *)
KV_FACTROY_TEMPLATE(CreateKVArrayNode, SetArray, IKVArray *)
KV_FACTROY_TEMPLATE(CreateKVMapNode, SetMap, IKVMap *)

#undef KV_FACTROY_TEMPLATE

CComPtr<IKVArray>
CreateKVArray()
{
    CComPtr<IKVArray> spValue;
    if(FAILED(KVArray::CreateObject(__uuidof(IKVArray), (void **)&spValue)) || NULL == spValue) {
        return NULL;
    }

    return spValue;
}

CComPtr<IKVMap>
CreateKVMap()
{
    CComPtr<IKVMap> spValue;
    if(FAILED(KVMap::CreateObject(__uuidof(IKVMap), (void **)&spValue)) || NULL == spValue) {
        return NULL;
    }

    return spValue;
}
