#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_factory_base.h"

static DWORD s_vWindowFilterOps[] = {
    WINDOW_FILTER_OP_INCLUDE,
    WINDOW_FILTER_OP_EXCLUDE,
};

static DWORD s_nWindowFilterOpNum = sizeof(s_vWindowFilterOps) / sizeof(DWORD);

WindowFilterFactoryBase::WindowFilterFactoryBase(DWORD *pRelations, DWORD nRelationCount, TCHAR **pValues, DWORD *pValueDatas, DWORD nValueCount)
    : m_pRelations(pRelations)
    , m_nRelationCount(nRelationCount)
    , m_pValues(pValues)
    , m_pValueDatas(pValueDatas)
    , m_nValueCount(nValueCount)
    , m_pOps(s_vWindowFilterOps)
    , m_nOpCount(s_nWindowFilterOpNum)
    , m_pCurrentFilters(NULL)
    , m_nCurrentFilterNum(0)
{

}

WindowFilterFactoryBase::WindowFilterFactoryBase(DWORD *pRelations, DWORD nRelationCount, TCHAR **pValues, DWORD *pValueDatas, DWORD nValueCount, DWORD *pOps, DWORD nOpCount)
    : m_pRelations(pRelations)
    , m_nRelationCount(nRelationCount)
    , m_pValues(pValues)
    , m_pValueDatas(pValueDatas)
    , m_nValueCount(nValueCount)
    , m_pOps(pOps)
    , m_nOpCount(nOpCount)
    , m_pCurrentFilters(NULL)
    , m_nCurrentFilterNum(0)
{

}

WindowFilterFactoryBase::~WindowFilterFactoryBase()
{

}

HRESULT
WindowFilterFactoryBase::PrepareSupportedData(IWindowFilter **ppFilters, DWORD nCount)
{
    m_pCurrentFilters = ppFilters;
    m_nCurrentFilterNum = nCount;

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::ClearSupportedData()
{
    m_pCurrentFilters = NULL;
    m_nCurrentFilterNum = 0;

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedRelationCount(DWORD *pCount)
{
    if(NULL == pCount) {
        return E_INVALIDARG;
    }

    *pCount = m_nRelationCount;

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedRelations(DWORD *pRelations, DWORD nMaxCount)
{
    if(NULL == pRelations) {
        return E_INVALIDARG;
    }

    if(NULL == m_pRelations) {
        return S_OK;
    }

    DWORD i = 0, nCount = 0;
    while(i < m_nRelationCount && nCount < nMaxCount) {
        pRelations[i] = m_pRelations[i];
        ++i;
        ++nCount;
    }

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedValueCount(DWORD *pCount)
{
    if(NULL == pCount) {
        return E_INVALIDARG;
    }

    *pCount = m_nValueCount;

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedValues(BSTR *pValueNames, DWORD *pValueDatas, DWORD nMaxCount)
{
    if(NULL == pValueNames || NULL == pValueDatas) {
        return E_INVALIDARG;
    }

    if(NULL == m_pValues) {
        return S_OK;
    }

    DWORD i = 0, nCount = 0;
    while(i < m_nValueCount && nCount < nMaxCount) {
        if(NULL != m_pValues) {
            pValueNames[i] = CComBSTR(m_pValues[i]).Detach();
        } else {
            pValueNames[i] = NULL;
        }

        if(NULL != m_pValueDatas) {
            pValueDatas[i] = m_pValueDatas[i];
        } else {
            pValueDatas[i] = 0;
        }

        ++i;
        ++nCount;
    }

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedOpCount(DWORD *pCount)
{
    if(NULL == pCount) {
        return E_INVALIDARG;
    }

    *pCount = m_nOpCount;

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::GetSupportedOps(DWORD *pOps, DWORD nMaxCount)
{
    if(NULL == pOps) {
        return E_INVALIDARG;
    }

    if(NULL == m_pOps) {
        return S_OK;
    }

    DWORD i = 0, nCount = 0;
    while(i < m_nOpCount && nCount < nMaxCount) {
        pOps[i] = m_pOps[i];
        ++i;
        ++nCount;
    }

    return S_OK;
}

HRESULT
WindowFilterFactoryBase::InitWindowFilter(CComPtr<IWindowFilter> spFilter, DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **ppFilter)
{
    if(NULL == ppFilter) {
        return E_INVALIDARG;
    }

    if(FAILED(spFilter->SetRelation(nRelation))) {
        return E_FAIL;
    }

    if(FAILED(spFilter->SetValue(strValue))) {
        return E_FAIL;
    }

    if(FAILED(spFilter->SetOp(nFilterOp))) {
        return E_FAIL;
    }

    *ppFilter = spFilter.Detach();

    return S_OK;
}
