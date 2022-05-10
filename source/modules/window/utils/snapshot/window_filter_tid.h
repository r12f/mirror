#pragma once

#include <vector>
#include <map>
#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_filter_number.h"

class WindowFilterTid :
    public WindowFilterNumber
{
public:
    WindowFilterTid();
    virtual ~WindowFilterTid();

    COM_CLASS_FACTORY(WindowFilterTid)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(GetValue)(BSTR *pValue, DWORD *pValueData);
    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);
};

class WindowFilterFactoryTid :
    public WindowFilterNumberFactory
{
public:
    WindowFilterFactoryTid();
    virtual ~WindowFilterFactoryTid();

    COM_CLASS_FACTORY(WindowFilterFactoryTid)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(PrepareSupportedData)(IWindowFilter **ppFilters, DWORD nCount);
    STDMETHOD(ClearSupportedData)();
    STDMETHOD(CreateWindowFilter)(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter);

protected:
    BOOL FetchProcessThreadToValueList(std::map<DWORD, CString> &vProcesses);

private:
    std::vector<DWORD> m_vTidList;
    std::vector<CComBSTR> m_vThreadNameList;
};