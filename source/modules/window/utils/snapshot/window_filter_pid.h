#pragma once

#include <vector>
#include "modules/window/utils/snapshot/window_filter_number.h"

class WindowFilterPid :
    public WindowFilterNumber
{
public:
    WindowFilterPid();
    virtual ~WindowFilterPid();

    COM_CLASS_FACTORY(WindowFilterPid)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(GetValue)(BSTR *pValue, DWORD *pValueData);
    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);
};

class WindowFilterFactoryPid :
    public WindowFilterNumberFactory
{
public:
    WindowFilterFactoryPid();
    virtual ~WindowFilterFactoryPid();

    COM_CLASS_FACTORY(WindowFilterFactoryPid)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(PrepareSupportedData)(IWindowFilter **ppFilters, DWORD nCount);
    STDMETHOD(ClearSupportedData)();
    STDMETHOD(CreateWindowFilter)(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter);

private:
    std::vector<DWORD> m_vPidList;
    std::vector<CComBSTR> m_vProcNameList;
};