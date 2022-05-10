#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_title.h"

WindowFilterTitle::WindowFilterTitle()
{

}

WindowFilterTitle::~WindowFilterTitle()
{

}

HRESULT
WindowFilterTitle::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_TITLE;

    return S_OK;
}

HRESULT
WindowFilterTitle::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    WindowSnapshot *pRawSnapshot = GetRawSnapshot(pSnapshot);
    if(NULL == pRawSnapshot || NULL == pTestResult) {
        return E_INVALIDARG;
    }

    return TestStringFilter(pRawSnapshot->m_strTitle, pTestResult);
}

WindowFilterFactoryTitle::WindowFilterFactoryTitle()
{

}

WindowFilterFactoryTitle::~WindowFilterFactoryTitle()
{

}

HRESULT
WindowFilterFactoryTitle::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_TITLE;

    return S_OK;
}

HRESULT
WindowFilterFactoryTitle::CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter)
{
    CComPtr<IWindowFilter> spFilter;
    if(FAILED(WindowFilterTitle::CreateObject(__uuidof(IWindowFilter), (void **)&spFilter)) && NULL == spFilter) {
        return E_FAIL;
    }

    return InitWindowFilter(spFilter, nRelation, strValue, nFilterOp, pFilter);
}