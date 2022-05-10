#pragma once

#include "base/com/com_class.h"
#include "modules/window/utils/snapshot/window_filter_const.h"
#include "modules/window/utils/snapshot/window_filter.h"
#include "modules/window/utils/snapshot/window_snapshot_impl.h"

class WindowFilterBase :
    public IWindowFilter
{
public:
    WindowFilterBase();
    virtual ~WindowFilterBase();

    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(WindowFilterBase)
            QI_MAPPING(IWindowFilter)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(GetType)(DWORD *pType);

    STDMETHOD(SetRelation)(DWORD nRelation);
    STDMETHOD(GetRelation)(DWORD *pRelation);
 
    STDMETHOD(SetValue)(BSTR strValue);
    STDMETHOD(GetValue)(BSTR *pValue, DWORD *pValueData);

    STDMETHOD(SetOp)(DWORD nOp);
    STDMETHOD(GetOp)(DWORD *pOp);

    STDMETHOD(TestFilter)(IWindowSnapshot *pSnapshot, DWORD *pTestResult);

protected:
    WindowSnapshot *GetRawSnapshot(IWindowSnapshot *pSnapshot);

protected:
    DWORD m_nRelation;
    DWORD m_nOp;
};