#pragma once

#include "modules/window/utils/snapshot/window_filter_const.h"

__interface IWindowSnapshot;

MIDL_INTERFACE("F40C45EB-2CDD-4836-9BEC-77C1E162BDCA")
IWindowFilter : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetType(DWORD *pType) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetRelation(DWORD nRelation) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRelation(DWORD *pRelation) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetValue(BSTR strValue) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetValue(BSTR *pValue, DWORD *pValueData) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetOp(DWORD nOp) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetOp(DWORD *pOp) = 0;
    virtual HRESULT STDMETHODCALLTYPE TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult) = 0;
};

MIDL_INTERFACE("B60C8751-2D19-4bba-9E65-B5EA48BE0574")
IWindowFilterEvent : IUnknown
{
    virtual HRESULT STDMETHODCALLTYPE BeforeFilterTest() = 0;
    virtual HRESULT STDMETHODCALLTYPE AfterFilterTest() = 0;
};