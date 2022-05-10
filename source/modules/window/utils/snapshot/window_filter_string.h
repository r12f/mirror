#pragma once

#include "modules/window/utils/snapshot/window_filter_base.h"
#include "modules/window/utils/snapshot/window_filter_factory_base.h"

class WindowFilterString :
    public WindowFilterBase
{
public:
    WindowFilterString();
    virtual ~WindowFilterString();

    STDMETHOD(SetValue)(BSTR strValue);
    STDMETHOD(GetValue)(BSTR *pValue, DWORD *pValueData);

protected:
    HRESULT TestStringFilter(BSTR strValue, DWORD *pTestResult);

protected:
    CString m_strValue;
    CString m_strValueLower;
};

class WindowFilterStringFactory :
    public WindowFilterFactoryBase
{
public:
    WindowFilterStringFactory();
    virtual ~WindowFilterStringFactory();
};