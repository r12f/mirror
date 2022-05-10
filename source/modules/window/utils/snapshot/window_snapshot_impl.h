#pragma once

#include <vector>
#include "base/com/com_class.h"
#include "modules/window/utils/snapshot/window_snapshot.h"

class WindowSnapshot :
    public IWindowSnapshot
{
    typedef std::vector<CComPtr<IWindowSnapshot>> WindowSnapshotList;

public:
    WindowSnapshot();
    virtual ~WindowSnapshot();

    COM_CLASS_FACTORY(WindowSnapshot)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(WindowSnapshot)
            QI_MAPPING(IWindowSnapshot)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(MakeSnapshot)(HWND hWindow, BOOL bRecursive, DWORD nMethod);
    STDMETHOD(GetHWND)(HWND *pWnd);
    STDMETHOD(GetClassName)(BSTR *pClassName);
    STDMETHOD(GetTitle)(BSTR *pTitle);
    STDMETHOD(GetVisible)(BOOL *pVisible);
    STDMETHOD(GetPid)(DWORD *pPid);
    STDMETHOD(GetTid)(DWORD *pTid);
    STDMETHOD(GetChildrenCount)(DWORD *pCount);
    STDMETHOD(GetChildren)(IWindowSnapshot **pChildren, DWORD nMaxCount);
    STDMETHOD(SetPrivateData)(PVOID pData);
    STDMETHOD(GetPrivateData)(PVOID *ppData);
    STDMETHOD(SaveToFile)(BSTR strFile, DWORD nFileType);
    STDMETHOD(RunFilter)(IWindowFilter **pFilters, DWORD nCount);
    STDMETHOD(GetLastFilterStatus)(DWORD *pTestResult);

protected:
    VOID FetchWindowInfo();
    BOOL CreateChildrenByGetWindow();
    BOOL CreateChildrenByEnumWindow();
    static BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
    BOOL BeforeRunFilter(IWindowFilter **pFilters, DWORD nCount);
    BOOL AfterRunFilter(IWindowFilter **pFilters, DWORD nCount);
    BOOL DoRunFilter(IWindowFilter **pFilters, DWORD nCount, BOOL bForceSelect);
    BOOL RunFilterOnMe(IWindowFilter **pFilters, DWORD nCount);
    HRESULT SaveToFileWithDepth(HANDLE hFile, std::vector<BOOL> &vTreeStatus);

public:
    HWND m_hWnd;
    BOOL m_bVisible;
    DWORD m_nPid;
    DWORD m_nTid;
    CComBSTR m_strClassName;
    CComBSTR m_strTitle;
    WindowSnapshotList m_vChildren;
    PVOID m_pParam;
    DWORD m_nTestResult;
};