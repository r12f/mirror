#pragma once

#include "base/com/com_class.h"
#include "ui/layout.h"
#include "ui/ui_engine/layout/layout_container.h"

class LayoutVContainer :
    public LayoutContainer,
    public ILayoutVContainer
{
public:
    LayoutVContainer();
    virtual ~LayoutVContainer();
    
    COM_CLASS_FACTORY(LayoutVContainer)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(LayoutContainer)
        BEGIN_QI_MAP(LayoutVContainer)
            QI_MAPPING(ILayoutVContainer)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override ILayoutVContainer
    STDMETHODIMP SetRowSize(DWORD nRow);
    STDMETHODIMP SetRowHeight(DWORD nRow, INT32 nHeight);
    STDMETHODIMP SetRowWeight(DWORD nRow, DWORD nWeight);
    STDMETHODIMP SetChildBox(DWORD nRowId, ILayoutBox *pBox);

    STDMETHODIMP GetRowSize(DWORD *pRow);
    STDMETHODIMP GetRowHeight(DWORD nRow, INT32 *pHeight);
    STDMETHODIMP GetRowWeight(DWORD nRow, DWORD *pWeight);
    STDMETHODIMP GetChildBox(DWORD nRowId, ILayoutBox **ppBox);

protected:
    virtual HRESULT DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext);
};