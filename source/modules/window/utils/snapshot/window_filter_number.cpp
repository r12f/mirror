#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_number.h"
#include "base/string_number_convertions.h"

WindowFilterNumber::WindowFilterNumber()
    : m_nValue(0)
{

}

WindowFilterNumber::~WindowFilterNumber()
{

}

HRESULT
WindowFilterNumber::SetValue(BSTR strValue)
{
    m_nValue = findIntInString(strValue);

    return S_OK;
}

HRESULT
WindowFilterNumber::GetValue(BSTR *pValue, DWORD *pValueData)
{
    if(NULL != pValue) {
        CString strValue;
        strValue.Format(_T("%ld"), m_nValue);
        *pValue = CComBSTR(strValue).Detach();
    }

    if(NULL != pValueData) {
        *pValueData = m_nValue;
    }
    
    return S_OK;
}


HRESULT
WindowFilterNumber::TestNumberFilter(DWORD nValue, DWORD *pTestResult)
{
    ASSERT_RETURN(NULL != pTestResult, E_INVALIDARG);

    *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;

    BOOL bTestOK = FALSE;
    switch(m_nRelation) {
    case WINDOW_FILTER_RELATION_EQ:
        bTestOK = (nValue == m_nValue);
        break;
    case WINDOW_FILTER_RELATION_NE:
        bTestOK = (nValue != m_nValue);
        break;
    case WINDOW_FILTER_RELATION_GT:
        bTestOK = (nValue > m_nValue);
        break;
    case WINDOW_FILTER_RELATION_GE:
        bTestOK = (nValue >= m_nValue);
        break;
    case WINDOW_FILTER_RELATION_LT:
        bTestOK = (nValue < m_nValue);
        break;
    case WINDOW_FILTER_RELATION_LE:
        bTestOK = (nValue <= m_nValue);
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

static DWORD s_vWindowFilterNumberRelations[] = {
    WINDOW_FILTER_RELATION_EQ,
    WINDOW_FILTER_RELATION_NE,
    WINDOW_FILTER_RELATION_GT,
    WINDOW_FILTER_RELATION_GE,
    WINDOW_FILTER_RELATION_LT,
    WINDOW_FILTER_RELATION_LE,
};

static DWORD s_nWindowFilterNumberRelationNum = sizeof(s_vWindowFilterNumberRelations) / sizeof(DWORD);

WindowFilterNumberFactory::WindowFilterNumberFactory()
    : WindowFilterFactoryBase(s_vWindowFilterNumberRelations, s_nWindowFilterNumberRelationNum, NULL, NULL, 0)
{

}

WindowFilterNumberFactory::~WindowFilterNumberFactory()
{

}
