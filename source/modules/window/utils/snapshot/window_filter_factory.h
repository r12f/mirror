#pragma once

#include "modules/window/utils/snapshot/window_filter_const.h"
#include "modules/window/utils/snapshot/window_filter.h"

MIDL_INTERFACE("82B962CF-66F0-464a-8734-B0FE65E23A30")
IWindowFilterFactory : IUnknown
{
public:
    // 过滤器类厂类型
    virtual HRESULT STDMETHODCALLTYPE GetType(DWORD *pType) = 0;

    // 在获取支持的数据时，可以设置进当前的FilterList，帮助FilterFactory获取到最正确的List
    virtual HRESULT STDMETHODCALLTYPE PrepareSupportedData(IWindowFilter **ppFilters, DWORD nCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE ClearSupportedData() = 0;

    // 获取支持的操作类型
    virtual HRESULT STDMETHODCALLTYPE GetSupportedRelationCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSupportedRelations(DWORD *pRelations, DWORD nMaxCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSupportedValueCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSupportedValues(BSTR *pValues, DWORD *pValueDatas, DWORD nMaxCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSupportedOpCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSupportedOps(DWORD *pOps, DWORD nMaxCount) = 0;

    // 创建过滤器
    virtual HRESULT STDMETHODCALLTYPE CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter) = 0;
};

CComPtr<IWindowFilterFactory> CreateWindowFilterFactory(DWORD nFilterType);