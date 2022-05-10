#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_process_name.h"
#include "base/system/system_snapshot_walker.h"

WindowFilterProcessName::WindowFilterProcessName()
{

}

WindowFilterProcessName::~WindowFilterProcessName()
{

}

HRESULT
WindowFilterProcessName::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_PROCESS_NAME;

    return S_OK;
}

HRESULT
WindowFilterProcessName::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    WindowSnapshot *pRawSnapshot = GetRawSnapshot(pSnapshot);
    if(NULL == pRawSnapshot || NULL == pTestResult) {
        return E_INVALIDARG;
    }

    *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;

    std::set<DWORD>::iterator oPidIt = m_vValidPids.find(pRawSnapshot->m_nPid);
    if(oPidIt == m_vValidPids.end()) {
        return S_OK;
    }
    
    if(m_nOp == WINDOW_FILTER_OP_INCLUDE) {
        *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
    } else {
        *pTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE;
    }

    return S_OK;
}

HRESULT
WindowFilterProcessName::BeforeFilterTest()
{
    m_vValidPids.clear();

    Process32Walker oWalker;
    if(!oWalker.Start(0)) {
        return S_OK;
    }

    CString strProcName;
    PROCESSENTRY32 *pProcInfo = NULL;
    do {
        pProcInfo = oWalker.Get();
        strProcName = pProcInfo->szExeFile;
        strProcName.MakeLower();
        if(strProcName.Find(m_strValueLower) >= 0) {
            m_vValidPids.insert(pProcInfo->th32ProcessID);
        }
    } while(oWalker.WalkNext());

    return S_OK;
}

HRESULT
WindowFilterProcessName::AfterFilterTest()
{
    m_vValidPids.clear();
    return S_OK;
}

WindowFilterFactoryProcessName::WindowFilterFactoryProcessName()
{

}

WindowFilterFactoryProcessName::~WindowFilterFactoryProcessName()
{

}

HRESULT
WindowFilterFactoryProcessName::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_PROCESS_NAME;

    return S_OK;
}

HRESULT
WindowFilterFactoryProcessName::CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter)
{
    CComPtr<IWindowFilter> spFilter;
    if(FAILED(WindowFilterProcessName::CreateObject(__uuidof(IWindowFilter), (void **)&spFilter)) && NULL == spFilter) {
        return E_FAIL;
    }

    return InitWindowFilter(spFilter, nRelation, strValue, nFilterOp, pFilter);
}