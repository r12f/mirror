#pragma once

#include <set>
#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_filter_string.h"

class WindowFilterProcessName :
    public WindowFilterString,
    public IWindowFilterEvent
{
public:
    WindowFilterProcessName();
    virtual ~WindowFilterProcessName();

    COM_CLASS_FACTORY(WindowFilterProcessName)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(WindowFilterString)
        BEGIN_QI_MAP(WindowFilterProcessName)
            QI_MAPPING(IWindowFilterEvent)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);

    STDMETHOD(BeforeFilterTest)();
    STDMETHOD(AfterFilterTest)();

private:
    std::set<DWORD> m_vValidPids;
};

class WindowFilterFactoryProcessName :
    public WindowFilterStringFactory
{
public:
    WindowFilterFactoryProcessName();
    virtual ~WindowFilterFactoryProcessName();

    COM_CLASS_FACTORY(WindowFilterFactoryProcessName)

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(CreateWindowFilter)(DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **pFilter);
};