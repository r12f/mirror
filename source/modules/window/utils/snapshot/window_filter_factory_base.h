#pragma once

#include <list>
#include "base/com/com_class.h"
#include "modules/window/utils/snapshot/window_filter_factory.h"

class WindowFilterFactoryBase :
    public IWindowFilterFactory
{
public:
    WindowFilterFactoryBase(DWORD *pRelations, DWORD nRelationCount, TCHAR **pValues, DWORD *pValueDatas, DWORD nValueCount);
    WindowFilterFactoryBase(DWORD *pRelations, DWORD nRelationCount, TCHAR **pValues, DWORD *pValueDatas, DWORD nValueCount, DWORD *pOps, DWORD nOpCount);
    virtual ~WindowFilterFactoryBase();

    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(WindowFilterFactoryBase)
            QI_MAPPING(IWindowFilterFactory)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(PrepareSupportedData)(IWindowFilter **ppFilters, DWORD nCount);
    STDMETHOD(ClearSupportedData)();
    STDMETHOD(GetSupportedRelationCount)(DWORD *pCount);
    STDMETHOD(GetSupportedRelations)(DWORD *pRelations, DWORD nMaxCount);
    STDMETHOD(GetSupportedValueCount)(DWORD *pCount);
    STDMETHOD(GetSupportedValues)(BSTR *pValues, DWORD *pValueDatas, DWORD nMaxCount);
    STDMETHOD(GetSupportedOpCount)(DWORD *pCount);
    STDMETHOD(GetSupportedOps)(DWORD *pOps, DWORD nMaxCount);

protected:
    HRESULT InitWindowFilter(CComPtr<IWindowFilter> spFilter, DWORD nRelation, BSTR strValue, DWORD nFilterOp, IWindowFilter **ppFilter);

protected:
    DWORD *m_pRelations;
    DWORD m_nRelationCount;
    TCHAR **m_pValues;
    DWORD *m_pValueDatas;
    DWORD m_nValueCount;
    DWORD *m_pOps;
    DWORD m_nOpCount;
    IWindowFilter **m_pCurrentFilters;
    DWORD m_nCurrentFilterNum;
};