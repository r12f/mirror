/**
 * @file
 * @brief Com�ӿ���IUnknown�ӿڵ�ʵ�֣��ṩ���̰߳�ȫ�ͷ��̰߳�ȫ�����ü�������ӿں;ۺϵ�QI����
 * @note ����ʵ�ֵĶ�����Ϊ�˽������Ҫʹ��һ��Com�������һ��Com�ӿ�����ͬ���һ����Com���������
 * @note �����������£���Ϊ����ʹ��Ĭ���ػ�������ʵ�֣�template��ʵ�ֽ����Ե÷ǳ��鷳
 * @note ���ԣ���������ʹ�ú�������������
 */

#pragma once

#include <shlwapi.h>

#include "base/ref_count.h"

#define BEGIN_QUERY_INTERFACE()                             \
    STDMETHOD(QueryInterface)(REFIID riid, void** ppv)      \
    {                                                       \
        if (ppv == NULL) {                                  \
            return E_INVALIDARG;                            \
        }                                                   \
                                                            \
        if(riid == __uuidof(IUnknown)) {                    \
            AddRef();                                       \
            *ppv = this;                                    \
            return S_OK;                                    \
        }                                                   \
                                                            \
        HRESULT hr = S_OK;

#define QI_CHAIN_CLASS(c)                   \
    hr = c::QueryInterface(riid, ppv);      \
    if(SUCCEEDED(hr)) {                     \
        return hr;                          \
    }

#define QI_CHAIN_OBJECT_PTR(p)              \
    hr = p->QueryInterface(riid, ppv);      \
    if(SUCCEEDED(hr)) {                     \
        return hr;                          \
    }

#define QI_CHAIN_OBJECT(o)                  \
    hr = o.QueryInterface(riid, ppv);       \
    if(SUCCEEDED(hr)) {                     \
        return hr;                          \
    }

#define BEGIN_QI_MAP(c)                     \
    typedef c _THIS_CLASS;                  \
    static QITAB qimap[] = {

#define QI_MAPPING_EX(Ifoo, Iimpl)                                  \
        { &__uuidof(Ifoo), OFFSETOFCLASS(Iimpl, _THIS_CLASS) },

#define QI_MAPPING(Ifoo) QI_MAPPING_EX(Ifoo, Ifoo)

#define END_QI_MAP()                                                    \
        { 0 }                                                           \
    };                                                                  \
                                                                        \
    if(SUCCEEDED(QISearch(this, qimap, riid, ppv)) && NULL != *ppv) {   \
        return S_OK;                                                    \
    }

#define END_QUERY_INTERFACE()                   \
    return E_FAIL;                              \
}

#define COM_CLASS_FACTORY(c)                                \
    static HRESULT CreateObject(REFIID riid, void** ppv)    \
    {                                                       \
        c* pObj = new c;                                    \
        HRESULT hr = pObj->QueryInterface(riid, ppv);       \
        pObj->Release();                                    \
        return hr;                                          \
    }

#define COM_CLASS_REF_COUNT_IMPL()                          \
    public:                                                 \
        STDMETHOD_(ULONG, AddRef)()                         \
        {                                                   \
            return m_oRefCount.AddRef();                    \
        }                                                   \
                                                            \
        STDMETHOD_(ULONG, Release)()                        \
        {                                                   \
            ULONG nRefCount = m_oRefCount.Release();        \
                                                            \
            if (nRefCount == 0) {                           \
                delete this;                                \
                return 0;                                   \
            }                                               \
                                                            \
            return nRefCount;                               \
        }

#define PERSISTENT_COM_CLASS()                          \
    private:                                            \
        ComRefPersistent m_oRefCount;                   \
                                                        \
    COM_CLASS_REF_COUNT_IMPL()
        

#define STA_THREAD_MODAL()                              \
    private:                                            \
        ComRef m_oRefCount;                             \
                                                        \
    COM_CLASS_REF_COUNT_IMPL()

#define MTA_THREAD_MODAL()                              \
    private:                                            \
        ComRef m_oRefCount;                             \
                                                        \
    COM_CLASS_REF_COUNT_IMPL()