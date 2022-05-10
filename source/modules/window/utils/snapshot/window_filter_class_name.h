#pragma once

#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_filter_string.h"

class WindowFilterClassName :
    public WindowFilterString
{
public:
    WindowFilterClassName();
    virtual ~WindowFilterClassName();

    COM_CLASS_FACTORY(WindowFilterClassName)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);
};

class WindowFilterFactoryClassName :
    public WindowFilterStringFactory
{
public:
    WindowFilterFactoryClassName();
    virtual ~WindowFilterFactoryClassName();

    COM_CLASS_FACTORY(WindowFilterFactoryClassName)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(CreateWindowFilter)(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter);
};