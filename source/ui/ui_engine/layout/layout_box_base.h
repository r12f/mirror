#pragma once

#include "base/com/com_class.h"
#include "ui/layout.h"
#include "ui/ui_engine/layout/layout_internal_interfaces.h"
#include <vector>

class LayoutBoxBase :
    public ILayoutBoxInternal
{
    typedef std::vector<CComPtr<ILayoutBoxInternal>> ChildrenList;

public:
    LayoutBoxBase();
    virtual ~LayoutBoxBase();

    COM_CLASS_FACTORY(LayoutBoxBase)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(LayoutBoxBase)
            QI_MAPPING(ILayoutBox)
            QI_MAPPING(ILayoutBoxInternal)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override ILayoutBox
    // Hierarchy
    STDMETHODIMP AddChild(ILayoutBox *pBox);
    STDMETHODIMP RemoveChild(ILayoutBox *pBox);
    STDMETHODIMP_(DWORD) GetChildrenCount();
    STDMETHODIMP GetChildren(ILayoutBox **ppBox, DWORD nCount);
    STDMETHODIMP GetParent(ILayoutBox **ppBox);
    STDMETHODIMP GetRoot(ILayoutBox **ppBox);

    // Box attribute setters
    STDMETHODIMP SetLayoutType(DWORD nLayout);
    STDMETHODIMP SetLayoutX(INT32 nX);
    STDMETHODIMP SetLayoutY(INT32 nY);
    STDMETHODIMP SetLayoutPosition(POINT *pPosition);
    STDMETHODIMP SetLayoutWidth(INT32 nWidth);
    STDMETHODIMP SetLayoutHeight(INT32 nHeight);
    STDMETHODIMP SetLayoutSize(SIZE *pSize);
    STDMETHODIMP SetLayoutMinSize(SIZE *pSize);
    STDMETHODIMP SetLayoutMaxSize(SIZE *pSize);
    STDMETHODIMP SetLayoutMargin(RECT *pRect);
    STDMETHODIMP SetLayoutPadding(RECT *pRect);

    // Box attribute getters
    STDMETHODIMP_(DWORD) GetLayoutType();
    STDMETHODIMP_(INT32) GetLayoutX();
    STDMETHODIMP_(INT32) GetLayoutY();
    STDMETHODIMP GetLayoutPosition(POINT *pPosition);
    STDMETHODIMP_(INT32) GetLayoutWidth();
    STDMETHODIMP_(INT32) GetLayoutHeight();
    STDMETHODIMP GetLayoutSize(SIZE *pSize);
    STDMETHODIMP GetLayoutMinSize(SIZE *pSize);
    STDMETHODIMP GetLayoutMaxSize(SIZE *pSize);
    STDMETHODIMP GetLayoutMargin(RECT *pRect);
    STDMETHODIMP GetLayoutPadding(RECT *pRect);

    // Layout
    STDMETHODIMP_(BOOL) IsBoxVisible();
    STDMETHODIMP GetBoxClientRect(RECT *pRect);
    STDMETHODIMP Layout(RECT *pRect, BOOL bForce);

    // Override ILayoutBoxInternal
    STDMETHODIMP SetParent(ILayoutBoxInternal *pParent);

    // Native children for BeginDeferWindowPos
    STDMETHODIMP AddNativeWindowCount(DWORD nChildCount);
    STDMETHODIMP RemoveNativeWindowCount(DWORD nChildCount);
    STDMETHODIMP_(DWORD) GetNativeWindowCount();
    STDMETHODIMP LayoutRelatedChildren(RECT *pUsefulRect, LayoutContext *pContext);
    STDMETHODIMP LayoutChildrenNoSelf(BOOL bForce);

protected:
    void CreateLayoutContext(LayoutContext *pContext);
    void EndLayoutRelateChildren(LayoutContext *pContext);
    void DestroyLayoutContext(LayoutContext *pContext);
    virtual HRESULT DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext);
    HRESULT LayoutChildToRect(ILayoutBoxInternal *pBox, RECT *pRect, RECT *pChildRect, LayoutContext *pContext);
    HRESULT LayoutChildrenToRect(ChildrenList &vChildren, RECT *pRect, LayoutContext *pContext);

protected:
    ChildrenList m_vChildren;
    ILayoutBoxInternal *m_pParent;
    DWORD m_nLayoutType;
    CPoint m_oBoxPosition;
    CSize m_oBoxSize;
    CSize m_oBoxMinSize;
    CSize m_oBoxMaxSize;
    CRect m_oBoxMargin;
    CRect m_oBoxPadding;
    CRect m_oBoxClientRect;
    CRect m_oBoxUsefulRect;
    DWORD m_nNativeWindowCount;
};