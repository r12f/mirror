#pragma once

#include "base/com/com_class.h"
#include "ui/layout.h"
#include "ui/ui_engine/layout/layout_container.h"

class LayoutHContainer :
    public LayoutContainer,
    public ILayoutHContainer
{
public:
    LayoutHContainer();
    virtual ~LayoutHContainer();
    
    COM_CLASS_FACTORY(LayoutHContainer)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(LayoutContainer)
        BEGIN_QI_MAP(LayoutHContainer)
            QI_MAPPING(ILayoutHContainer)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override ILayoutHContainer
    STDMETHODIMP SetColSize(DWORD nCol);
    STDMETHODIMP SetColWidth(DWORD nCol, INT32 nWidth);
    STDMETHODIMP SetColWeight(DWORD nCol, DWORD nWeight);
    STDMETHODIMP SetChildBox(DWORD nColId, ILayoutBox *pBox);

    STDMETHODIMP GetColSize(DWORD *pCol);
    STDMETHODIMP GetColWidth(DWORD nCol, INT32 *pWidth);
    STDMETHODIMP GetColWeight(DWORD nCol, DWORD *pWeight);
    STDMETHODIMP GetChildBox(DWORD nColId, ILayoutBox **ppBox);

protected:
    virtual HRESULT DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext);
};