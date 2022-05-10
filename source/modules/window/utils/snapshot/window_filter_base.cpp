#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_base.h"

WindowFilterBase::WindowFilterBase()
    : m_nRelation(0)
    , m_nOp(0)
{

}

WindowFilterBase::~WindowFilterBase()
{

}

HRESULT
WindowFilterBase::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = 0;

    return E_FAIL;
}

HRESULT
WindowFilterBase::SetRelation(DWORD nRelation)
{
    m_nRelation = nRelation;
    return S_OK;
}

HRESULT
WindowFilterBase::GetRelation(DWORD *pRelation)
{
    if(NULL == pRelation) {
        return E_INVALIDARG;
    }

    *pRelation = m_nRelation;

    return S_OK;
}

HRESULT
WindowFilterBase::SetValue(BSTR strValue)
{
    return E_FAIL;
}

HRESULT
WindowFilterBase::GetValue(BSTR *pValue, DWORD *pValueData)
{
    return E_FAIL;
}

HRESULT
WindowFilterBase::SetOp(DWORD nOp)
{
    m_nOp = nOp;
    return S_OK;
}

HRESULT
WindowFilterBase::GetOp(DWORD *pOp)
{
    if(NULL == pOp) {
        return E_INVALIDARG;
    }

    *pOp = m_nOp;

    return S_OK;
}

HRESULT
WindowFilterBase::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    if(NULL == pTestResult) {
        return E_INVALIDARG;
    }

    *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;

    return E_FAIL;
}

WindowSnapshot *
WindowFilterBase::GetRawSnapshot(IWindowSnapshot *pSnapshot)
{
    if(NULL == pSnapshot) {
        return NULL;
    }

    return static_cast<WindowSnapshot *>(pSnapshot);
}