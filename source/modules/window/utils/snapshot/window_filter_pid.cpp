#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_pid.h"
#include "base/system/system_snapshot_walker.h"
#include "base/system/process_control.h"

WindowFilterPid::WindowFilterPid()
{

}

WindowFilterPid::~WindowFilterPid()
{

}

HRESULT
WindowFilterPid::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_PID;

    return S_OK;
}

HRESULT
WindowFilterPid::GetValue(BSTR *pValue, DWORD *pValueData)
{
    if(NULL != pValue) {
        CString strProcName;
        ProcessControl oProc(m_nValue, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION);
        oProc.GetExeFileName(strProcName);

        CString strValue;
        strValue.Format(_T("[%lu] %s"), m_nValue, strProcName);
        *pValue = CComBSTR(strValue).Detach();
    }

    if(NULL != pValueData) {
        *pValueData = m_nValue;
    }

    return S_OK;
}

HRESULT
WindowFilterPid::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    WindowSnapshot *pRawSnapshot = GetRawSnapshot(pSnapshot);
    if(NULL == pRawSnapshot || NULL == pTestResult) {
        return E_INVALIDARG;
    }

    return TestNumberFilter(pRawSnapshot->m_nPid, pTestResult);
}

WindowFilterFactoryPid::WindowFilterFactoryPid()
{

}

WindowFilterFactoryPid::~WindowFilterFactoryPid()
{

}

HRESULT
WindowFilterFactoryPid::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_PID;

    return S_OK;
}

HRESULT
WindowFilterFactoryPid::PrepareSupportedData(IWindowFilter **ppFilters, DWORD nCount)
{
    WindowFilterFactoryBase::PrepareSupportedData(ppFilters, nCount);

    Process32Walker oWalker;
    if(!oWalker.Start()) {
        return E_FAIL;
    }

    CString strProcName;
    do {
        PROCESSENTRY32 *pEntry = oWalker.Get();
        strProcName.Format(_T("[%lu] %s"), pEntry->th32ProcessID, pEntry->szExeFile);
        m_vProcNameList.push_back(CComBSTR(strProcName));
        m_vPidList.push_back(pEntry->th32ProcessID);
    } while(oWalker.WalkNext());

    m_nValueCount = (DWORD)m_vPidList.size();
    if(0 < m_nValueCount) {
        m_pValues = (TCHAR **)&(m_vProcNameList[0].m_str);
        m_pValueDatas = &(m_vPidList[0]);
    }

    return S_OK;
}

HRESULT
WindowFilterFactoryPid::ClearSupportedData()
{
    m_pValues = NULL;
    m_pValueDatas = NULL;
    m_nValueCount = 0;

    m_vPidList.clear();
    m_vProcNameList.clear();

    WindowFilterFactoryBase::ClearSupportedData();

    return S_OK;
}

HRESULT
WindowFilterFactoryPid::CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter)
{
    CComPtr<IWindowFilter> spFilter;
    if(FAILED(WindowFilterPid::CreateObject(__uuidof(IWindowFilter), (void **)&spFilter)) && NULL == spFilter) {
        return E_FAIL;
    }

    return InitWindowFilter(spFilter, nRelation, strValue, nFilterOp, pFilter);
}