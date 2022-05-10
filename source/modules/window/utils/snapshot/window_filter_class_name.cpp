#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_class_name.h"

WindowFilterClassName::WindowFilterClassName()
{

}

WindowFilterClassName::~WindowFilterClassName()
{

}

HRESULT
WindowFilterClassName::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_CLASSNAME;

    return S_OK;
}

HRESULT
WindowFilterClassName::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    WindowSnapshot *pRawSnapshot = GetRawSnapshot(pSnapshot);
    if(NULL == pRawSnapshot || NULL == pTestResult) {
        return E_INVALIDARG;
    }

    return TestStringFilter(pRawSnapshot->m_strClassName, pTestResult);
}

WindowFilterFactoryClassName::WindowFilterFactoryClassName()
{

}

WindowFilterFactoryClassName::~WindowFilterFactoryClassName()
{

}

HRESULT
WindowFilterFactoryClassName::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_CLASSNAME;

    return S_OK;
}

HRESULT
WindowFilterFactoryClassName::CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter)
{
    CComPtr<IWindowFilter> spFilter;
    if(FAILED(WindowFilterClassName::CreateObject(__uuidof(IWindowFilter), (void **)&spFilter)) && NULL == spFilter) {
        return E_FAIL;
    }

    return InitWindowFilter(spFilter, nRelation, strValue, nFilterOp, pFilter);
}