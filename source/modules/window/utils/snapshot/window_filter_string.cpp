#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_string.h"

WindowFilterString::WindowFilterString()
{

}

WindowFilterString::~WindowFilterString()
{

}

HRESULT
WindowFilterString::SetValue(BSTR strValue)
{
    if(NULL == strValue) {
        return E_INVALIDARG;
    }

    m_strValue = strValue;
    m_strValueLower = m_strValue;
    m_strValueLower.MakeLower();

    return S_OK;
}

HRESULT
WindowFilterString::GetValue(BSTR *pValue, DWORD *pValueData)
{
    if(NULL == pValue) {
        return E_INVALIDARG;
    }

    *pValue = CComBSTR(m_strValue).Detach();

    return S_OK;
}

HRESULT
WindowFilterString::TestStringFilter(BSTR strValue, DWORD *pTestResult)
{
    ASSERT_RETURN(NULL != pTestResult, E_INVALIDARG);

    if(NULL == strValue) {
        return E_FAIL;
    }

    *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;

    CString strValueLower = strValue;
    strValueLower.MakeLower();
    DWORD n = 0;
    BOOL bTestOK = FALSE;
    switch(m_nRelation) {
    case WINDOW_FILTER_RELATION_EQ:
        bTestOK = (strValueLower == m_strValueLower);
        break;
    case WINDOW_FILTER_RELATION_NE:
        bTestOK = (strValueLower != m_strValueLower);
        break;
    case WINDOW_FILTER_RELATION_CONTAIN:
        bTestOK = (strValueLower.Find(m_strValueLower) >= 0);
        break;
    case WINDOW_FILTER_RELATION_NOT_CONTAIN:
        bTestOK = (strValueLower.Find(m_strValueLower) < 0);
        break;
    }

    if(!bTestOK) {
        return S_OK;
    }

    if(m_nOp == WINDOW_FILTER_OP_INCLUDE) {
        *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
    } else {
        *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE;
    }

    return S_OK;
}

static DWORD s_vWindowFilterStringRelations[] = {
    WINDOW_FILTER_RELATION_CONTAIN,
    WINDOW_FILTER_RELATION_NOT_CONTAIN,
    WINDOW_FILTER_RELATION_EQ,
    WINDOW_FILTER_RELATION_NE,
};

static DWORD s_nWindowFilterStringRelationNum = sizeof(s_vWindowFilterStringRelations) / sizeof(DWORD);

WindowFilterStringFactory::WindowFilterStringFactory()
    : WindowFilterFactoryBase(s_vWindowFilterStringRelations, s_nWindowFilterStringRelationNum, NULL, NULL, 0)
{

}

WindowFilterStringFactory::~WindowFilterStringFactory()
{

}
