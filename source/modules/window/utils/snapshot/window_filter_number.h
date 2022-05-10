#pragma once

#include "modules/window/utils/snapshot/window_filter_base.h"
#include "modules/window/utils/snapshot/window_filter_factory_base.h"

class WindowFilterNumber :
    public WindowFilterBase
{
public:
    WindowFilterNumber();
    virtual ~WindowFilterNumber();

    STDMETHOD(SetValue)(BSTR strValue);
    STDMETHOD(GetValue)(BSTR *pValue, DWORD *pValueData);

protected:
    HRESULT TestNumberFilter(DWORD nValue, DWORD *pTestResult);

protected:
    DWORD m_nValue;
};

class WindowFilterNumberFactory :
    public WindowFilterFactoryBase
{
public:
    WindowFilterNumberFactory();
    virtual ~WindowFilterNumberFactory();
};