#pragma once

#include "base/ref_count.h"

template<class T, class Ref, class A>
class SimpleComClassT : public A
{
public:
    SimpleComClassT()
    {

    }

    virtual ~SimpleComClassT()
    {

    }

    STDMETHOD(QueryInterface)(REFIID riid, void** ppv)
    {
        if (ppv == NULL) {
            return E_INVALIDARG;
        }

        if (riid == IID_IUnknown) {
            *ppv = (A*)this;
            AddRef();
            return S_OK;
        }

        if (SUCCEEDED(GetInterface<A>(riid, ppv))) {
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    template<class Q>
    HRESULT GetInterface(REFIID riid, void** ppv)
    {
        if (riid == __uuidof(Q))
        {
            *ppv = (Q*)this;
            AddRef();
            return S_OK;
        }

        return E_NOINTERFACE;
    }

    STDMETHOD_(ULONG, AddRef)()
    {
        return m_oRefCount.AddRef();
    }

    STDMETHOD_(ULONG, Release)()
    {
        ULONG nRef = m_oRefCount.Release();

        if (nRef == 0) {
            delete this;
            return 0;
        }

        return nRef;
    }

    static HRESULT CreateObject(REFIID riid, void** ppv)
    {
        T* pObj = new T;
        HRESULT hr = pObj->QueryInterface(riid, ppv);
        pObj->Release();
        return hr;
    }

protected:
    Ref m_oRefCount;
};
