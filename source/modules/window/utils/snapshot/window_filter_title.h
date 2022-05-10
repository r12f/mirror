#pragma once

#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_filter_string.h"

class WindowFilterTitle :
    public WindowFilterString
{
public:
    WindowFilterTitle();
    virtual ~WindowFilterTitle();

    COM_CLASS_FACTORY(WindowFilterTitle)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);
};

class WindowFilterFactoryTitle :
    public WindowFilterStringFactory
{
public:
    WindowFilterFactoryTitle();
    virtual ~WindowFilterFactoryTitle();

    COM_CLASS_FACTORY(WindowFilterFactoryTitle)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(CreateWindowFilter)(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter);
};