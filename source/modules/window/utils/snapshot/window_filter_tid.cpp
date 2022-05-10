#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_tid.h"
#include "base/system/system_snapshot_walker.h"
#include "base/system/process_control.h"
#include "base/system/thread_control.h"

WindowFilterTid::WindowFilterTid()
{

}

WindowFilterTid::~WindowFilterTid()
{

}

HRESULT
WindowFilterTid::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_TID;

    return S_OK;
}

HRESULT
WindowFilterTid::GetValue(BSTR *pValue, DWORD *pValueData)
{
    if(NULL != pValue) {
        CString strThreadModule;
        ThreadControl oThread(m_nValue, THREAD_QUERY_INFORMATION);
        oThread.GetThreadModule(strThreadModule);
        
        CString strProcName;
        ProcessControl oProcess(oThread.GetProcessId(), PROCESS_VM_READ | PROCESS_QUERY_INFORMATION);
        oProcess.GetExeFileName(strProcName);

        CString strOutput;
        strOutput.Format(_T("[T:%lu] %s - [P:%lu] %s"), m_nValue, strThreadModule, oThread.GetProcessId(), strProcName);
        *pValue = CComBSTR(strOutput).Detach();
    }

    if(NULL != pValueData) {
        *pValueData = m_nValue;
    }

    return S_OK;
}

HRESULT
WindowFilterTid::TestFilter(IWindowSnapshot *pSnapshot, DWORD *pTestResult)
{
    WindowSnapshot *pRawSnapshot = GetRawSnapshot(pSnapshot);
    if(NULL == pRawSnapshot || NULL == pTestResult) {
        return E_INVALIDARG;
    }

    return TestNumberFilter(pRawSnapshot->m_nTid, pTestResult);
}

WindowFilterFactoryTid::WindowFilterFactoryTid()
{

}

WindowFilterFactoryTid::~WindowFilterFactoryTid()
{

}

HRESULT
WindowFilterFactoryTid::GetType(DWORD *pType)
{
    if(NULL == pType) {
        return E_INVALIDARG;
    }

    *pType = WINDOW_FILTER_TYPE_TID;

    return S_OK;
}

HRESULT
WindowFilterFactoryTid::PrepareSupportedData(IWindowFilter **ppFilters, DWORD nCount)
{
    WindowFilterFactoryBase::PrepareSupportedData(ppFilters, nCount);

    if(NULL == ppFilters || 0 == nCount) {
        return S_OK;
    }

    std::map<DWORD, CString> vProcNameList;
    Process32Walker oWalker;
    if(!oWalker.Start()) {
        return E_FAIL;
    }

    do {
        PROCESSENTRY32 *pEntry = oWalker.Get();
        vProcNameList[pEntry->th32ProcessID] = pEntry->szExeFile;
    } while(oWalker.WalkNext());

    std::map<DWORD, CString> vProcesses;
    DWORD i = 0;
    for(i = 0; i < nCount; ++i) {
        if(NULL == ppFilters[i]) {
            continue;
        }

        DWORD nFilterType = 0;
        if(FAILED(ppFilters[i]->GetType(&nFilterType)) || WINDOW_FILTER_TYPE_PID != nFilterType) {
            continue;
        }

        DWORD nValueData = 0;
        if(FAILED(ppFilters[i]->GetValue(NULL, &nValueData)) || 0 == nValueData) {
            continue;
        }

        std::map<DWORD, CString>::iterator oProcNameIt = vProcNameList.find(nValueData);
        if(oProcNameIt != vProcNameList.end()) {
            vProcesses[nValueData] = oProcNameIt->second;
        }
    }

    FetchProcessThreadToValueList(vProcesses);

    m_nValueCount = (DWORD)m_vTidList.size();
    if(0 < m_nValueCount) {
        m_pValues = (TCHAR **)&(m_vThreadNameList[0].m_str);
        m_pValueDatas = &(m_vTidList[0]);
    }

    return S_OK;
}

HRESULT
WindowFilterFactoryTid::ClearSupportedData()
{
    m_pValues = NULL;
    m_pValueDatas = NULL;
    m_nValueCount = 0;

    m_vTidList.clear();
    m_vThreadNameList.clear();

    WindowFilterFactoryBase::ClearSupportedData();

    return S_OK;
}

HRESULT
WindowFilterFactoryTid::CreateWindowFilter(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter)
{
    CComPtr<IWindowFilter> spFilter;
    if(FAILED(WindowFilterTid::CreateObject(__uuidof(IWindowFilter), (void **)&spFilter)) && NULL == spFilter) {
        return E_FAIL;
    }

    return InitWindowFilter(spFilter, nRelation, strValue, nFilterOp, pFilter);
}

BOOL
WindowFilterFactoryTid::FetchProcessThreadToValueList(std::map<DWORD, CString> &vProcesses)
{
    Thread32Walker oWalker;
    if(!oWalker.Start()) {
        return FALSE;
    }

    std::map<DWORD, CString>::iterator oProcIt;
    CString strThreadName;
    do {
        THREADENTRY32 *pEntry = oWalker.Get();
        oProcIt = vProcesses.find(pEntry->th32OwnerProcessID);
        if(oProcIt != vProcesses.end()) {
            CString strThreadModule;
            ThreadControl oThread(pEntry->th32ThreadID, THREAD_QUERY_INFORMATION);
            oThread.GetThreadModule(strThreadModule);

            strThreadName.Format(_T("[T:%lu] %s - [P:%lu] %s"), pEntry->th32ThreadID, strThreadModule.GetBuffer(), pEntry->th32OwnerProcessID, oProcIt->second.GetBuffer());
            m_vThreadNameList.push_back(CComBSTR(strThreadName));
            m_vTidList.push_back(pEntry->th32ThreadID);
        }
    } while(oWalker.WalkNext());

    return TRUE;
}