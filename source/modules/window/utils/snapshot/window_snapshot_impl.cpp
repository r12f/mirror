#include "stdafx.h"
#include "modules/window/utils/snapshot/window_snapshot_impl.h"

WindowSnapshot::WindowSnapshot()
    : m_hWnd(NULL)
    , m_bVisible(FALSE)
    , m_nPid(0)
    , m_nTid(0)
    , m_pParam(NULL)
    , m_nTestResult(WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE)
{

}

WindowSnapshot::~WindowSnapshot()
{

}

HRESULT
WindowSnapshot::MakeSnapshot(HWND hWindow, BOOL bRecursive, DWORD nMethod)
{
    if(NULL == hWindow) {
        return E_INVALIDARG;
    }

    m_hWnd = hWindow;
    FetchWindowInfo();

    if(!bRecursive) {
        return S_OK;
    }

    switch(nMethod) {
    case WINDOW_SNAPSHOT_METHOD_GET_WINDOW:
        CreateChildrenByGetWindow();
        break;
    case WINDOW_SNAPSHOT_METHOD_ENUM_WINDOW:
        CreateChildrenByEnumWindow();
        break;
    }

    return S_OK;
}


HRESULT
WindowSnapshot::GetHWND(HWND *pWnd)
{
    if(NULL == pWnd) {
        return E_INVALIDARG;
    }

    *pWnd = m_hWnd;

    return S_OK;
}

HRESULT
WindowSnapshot::GetClassName(BSTR *pClassName)
{
    if(NULL == pClassName) {
        return E_INVALIDARG;
    }

    CComBSTR strClassName = m_strClassName;
    *pClassName = strClassName.Detach();

    return S_OK;
}

HRESULT
WindowSnapshot::GetTitle(BSTR *pTitle)
{
    if(NULL == pTitle) {
        return E_INVALIDARG;
    }

    CComBSTR strTitle = m_strTitle;
    *pTitle = strTitle.Detach();

    return S_OK;
}

HRESULT
WindowSnapshot::GetVisible(BOOL *pVisible)
{
    if(NULL == pVisible) {
        return E_INVALIDARG;
    }

    *pVisible = m_bVisible;

    return S_OK;
}

HRESULT
WindowSnapshot::GetPid(DWORD *pPid)
{
    if(NULL == pPid) {
        return E_INVALIDARG;
    }

    *pPid = m_nPid;

    return S_OK;
}

HRESULT
WindowSnapshot::GetTid(DWORD *pTid)
{
    if(NULL == pTid) {
        return E_INVALIDARG;
    }

    *pTid = m_nTid;

    return S_OK;
}

HRESULT
WindowSnapshot::GetChildrenCount(DWORD *pCount)
{
    if(NULL == pCount) {
        return E_INVALIDARG;
    }

    *pCount = (DWORD)m_vChildren.size();

    return S_OK;
}

HRESULT
WindowSnapshot::GetChildren(IWindowSnapshot **pChildren, DWORD nMaxCount)
{
    if(NULL == pChildren) {
        return E_INVALIDARG;
    }

    DWORD i = 0;
    CComPtr<IWindowSnapshot> pChild;
    WindowSnapshotList::iterator oChildIt = m_vChildren.begin();
    while(i < nMaxCount && oChildIt != m_vChildren.end()) {
        pChild = *(oChildIt);
        pChildren[i] = pChild.Detach();
        ++i;
        ++oChildIt;
    }

    return S_OK;
}

HRESULT
WindowSnapshot::SetPrivateData(PVOID pData)
{
    m_pParam = pData;
    return S_OK;
}

HRESULT
WindowSnapshot::GetPrivateData(PVOID *ppData)
{
    if(NULL == ppData) {
        return E_INVALIDARG;
    }

    *ppData = m_pParam;

    return S_OK;
}

HRESULT
WindowSnapshot::SaveToFile(BSTR strFile, DWORD nFileType)
{
    HANDLE hFile = ::CreateFile((LPCTSTR)strFile, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(NULL == hFile) {
        DWORD nErrCode = ::GetLastError();
        MLOG_ERROR("WindowSnapshot", "保存窗口列表失败，无法创建文件：%s，错误码：%lu", strFile, nErrCode);
        return E_FAIL;
    }

    std::vector<BOOL> vTreeStatus;
    vTreeStatus.resize(1);
    vTreeStatus[0] = 0;

    HRESULT hr = SaveToFileWithDepth(hFile, vTreeStatus);
    if(FAILED(hr)) {
        DWORD nErrCode = ::GetLastError();
        MLOG_ERROR("WindowSnapshot", "保存窗口列表失败，错误码：%lu", strFile, nErrCode);
    } else {
        MLOG_INFO("WindowSnapshot", "保存窗口列表成功");
    }

    ::CloseHandle(hFile);

    return hr;
}

HRESULT
WindowSnapshot::RunFilter(IWindowFilter **pFilters, DWORD nCount)
{
    BeforeRunFilter(pFilters, nCount);
    DoRunFilter(pFilters, nCount, FALSE);
    AfterRunFilter(pFilters, nCount);
    return S_OK;
}

HRESULT
WindowSnapshot::GetLastFilterStatus(DWORD *pTestResult)
{
    if(NULL == pTestResult) {
        return E_INVALIDARG;
    }

    *pTestResult = m_nTestResult;

    return S_OK;
}

VOID
WindowSnapshot::FetchWindowInfo()
{
    m_bVisible = ::IsWindowVisible(m_hWnd);

    m_nTid = ::GetWindowThreadProcessId(m_hWnd, &(m_nPid));

    TCHAR szClassNameBuffer[1024] = {0};
    ::GetClassName(m_hWnd, szClassNameBuffer, 1024);
    m_strClassName = szClassNameBuffer;

    TCHAR szTitleBuffer[1024] = {0};
    ::GetWindowText(m_hWnd, szTitleBuffer, 1024);
    m_strTitle = szTitleBuffer;
}

BOOL
WindowSnapshot::CreateChildrenByGetWindow()
{
    HWND hChild = GetWindow(m_hWnd, GW_CHILD);
    while(NULL != hChild) {
        CComPtr<IWindowSnapshot> pChild;
        WindowSnapshot::CreateObject(__uuidof(IWindowSnapshot), (void **)&pChild);
        if(NULL != pChild) {
            pChild->MakeSnapshot(hChild, TRUE, WINDOW_SNAPSHOT_METHOD_GET_WINDOW);
            m_vChildren.push_back(pChild);
        }
        hChild = GetWindow(hChild, GW_HWNDNEXT);
    }

    return TRUE;
}

BOOL
WindowSnapshot::CreateChildrenByEnumWindow()
{
    EnumChildWindows(m_hWnd, WindowSnapshot::EnumWindowsProc, (LPARAM)this);
    return TRUE;
}

BOOL
WindowSnapshot::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    WindowSnapshot *pThis = (WindowSnapshot *)(lParam);
    if(NULL == hwnd || NULL == pThis) {
        return FALSE;
    }

    // 构造当前节点信息，并添加到窗口列表中
    CComPtr<IWindowSnapshot> pChild;
    WindowSnapshot::CreateObject(__uuidof(IWindowSnapshot), (void **)&pChild);
    if(NULL == pChild) {
        return FALSE;
    }

    pChild->MakeSnapshot(hwnd, TRUE, WINDOW_SNAPSHOT_METHOD_ENUM_WINDOW);
    pThis->m_vChildren.push_back(pChild);

    return TRUE;
}

BOOL
WindowSnapshot::BeforeRunFilter(IWindowFilter **pFilters, DWORD nCount)
{
    if(NULL == pFilters || 0 == nCount) {
        return FALSE;
    }

    DWORD i = 0;
    for(i = 0; i < nCount; ++i) {
        CComQIPtr<IWindowFilterEvent> pFilterEvent = pFilters[i];
        if(NULL != pFilterEvent) {
            pFilterEvent->BeforeFilterTest();
        }
    }

    return TRUE;
}

BOOL
WindowSnapshot::AfterRunFilter(IWindowFilter **pFilters, DWORD nCount)
{
    if(NULL == pFilters || 0 == nCount) {
        return FALSE;
    }

    DWORD i = 0;
    for(i = 0; i < nCount; ++i) {
        CComQIPtr<IWindowFilterEvent> pFilterEvent = pFilters[i];
        if(NULL != pFilterEvent) {
            pFilterEvent->AfterFilterTest();
        }
    }

    return TRUE;
}

BOOL
WindowSnapshot::DoRunFilter(IWindowFilter **pFilters, DWORD nCount, BOOL bForceSelect)
{
    if(bForceSelect) {
        m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
    } else {
        RunFilterOnMe(pFilters, nCount);
        if(m_nTestResult == WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE) {
            return TRUE;
        }
    }

    BOOL bForceSelectChild = (m_nTestResult == WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE);
    DWORD nChild = 0, nChildrenCount = (DWORD)m_vChildren.size();
    for(nChild = 0; nChild < nChildrenCount; ++nChild) {
        WindowSnapshot *pRawChild = static_cast<WindowSnapshot *>(m_vChildren[nChild].p);
        if(NULL == pRawChild) {
            continue;
        }

        pRawChild->DoRunFilter(pFilters, nCount, bForceSelectChild);
        if(pRawChild->m_nTestResult == WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE) {
            m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
        }
    }

    return TRUE;
}

BOOL
WindowSnapshot::RunFilterOnMe(IWindowFilter **pFilters, DWORD nCount)
{
    if(NULL == pFilters) {
        m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
        return TRUE;
    }

    BOOL bHasInclude = FALSE;
    BOOL bHasIncludeFilter = FALSE;
    DWORD nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;
    DWORD i = 0;
    for(i = 0; i < nCount; ++i) {
        if(NULL == pFilters[i]) {
            continue;
        }

        DWORD nOp = 0;
        pFilters[i]->GetOp(&nOp);
        if(nOp == WINDOW_FILTER_OP_INCLUDE) {
            bHasIncludeFilter = TRUE;
        }

        if(FAILED((pFilters[i])->TestFilter(this, &nTestResult))) {
            continue;
        }

        switch(nTestResult) {
        case WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED:
            break;
        case WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE:
            bHasInclude = TRUE;
            break;
        case WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE:
            m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_EXCLUDE;
            return TRUE;
        }
    }

    if(!bHasIncludeFilter || bHasInclude) {
        m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE;
        return TRUE;
    }

    m_nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;

    return TRUE;
}

HRESULT
WindowSnapshot::SaveToFileWithDepth(HANDLE hFile, std::vector<BOOL> &vTreeStatus)
{
    if(WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE != m_nTestResult) {
        return E_FAIL;
    }

    CString strFmt;
    CString strPrefix;

    // vTreeStatus每一位代表这一层的窗口是不是最后一层窗口，1表示不是最后一层，0表示最后一层
    DWORD nDepth = (DWORD)vTreeStatus.size();
    if(0 < nDepth) {
        for(DWORD i = 0; i < nDepth - 1; ++i) {
            if(vTreeStatus[i]) {
                strPrefix += _T("|   ");
            } else {
                strPrefix += _T("    ");
            }
        }

        if(vTreeStatus[nDepth - 1]) {
            strPrefix += _T("|--- ");
        } else {
            strPrefix += _T("\\--- ");
        }
    }

    // 输出当前窗口的信息
    CString strInfo;
    strInfo.Format(_T("Window %08x \"%s\" %s (%lu:%lu) (%s)\r\n"), m_hWnd, (LPCTSTR)m_strTitle, (LPCTSTR)m_strClassName, m_nPid, m_nTid, m_bVisible ? _T("Visible") : _T("Invisible"));
    strInfo = strPrefix + strInfo;

    DWORD nWrittenNum = 0;
    ::WriteFile(hFile, strInfo.GetBuffer(), strInfo.GetLength() * sizeof(TCHAR), &nWrittenNum, NULL);

    // 首先需要获取
    WindowSnapshotList oTestOKChildren;
    WindowSnapshotList::iterator oChildIt = m_vChildren.begin();
    while(oChildIt != m_vChildren.end()) {
        DWORD nTestResult = WINDOW_SNAPSHOT_FILTER_TEST_RESULT_FAILED;
        (*oChildIt)->GetLastFilterStatus(&nTestResult);

        if(nTestResult == WINDOW_SNAPSHOT_FILTER_TEST_RESULT_INCLUDE) {
            oTestOKChildren.push_back(*oChildIt);
        }

        ++oChildIt;
    }

    // 输出子节点的信息
    nDepth += 1;
    vTreeStatus.resize(nDepth);

    oChildIt = oTestOKChildren.begin();
    while(oChildIt != oTestOKChildren.end()) {
        WindowSnapshot *pWindow = ((WindowSnapshot *)((*oChildIt).p));
        ++oChildIt;

        vTreeStatus[nDepth - 1] = (oChildIt != oTestOKChildren.end());
        pWindow->SaveToFileWithDepth(hFile, vTreeStatus);
    }

    vTreeStatus.resize(nDepth - 1);

    return S_OK;
}