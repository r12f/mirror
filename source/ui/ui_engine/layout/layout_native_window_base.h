#pragma once

#include "ui/ui_engine/layout/layout_box_base.h"

class LayoutNativeWindowBase :
    public LayoutBoxBase,
    public ILayoutNativeWindow
{
public:
    LayoutNativeWindowBase();
    virtual ~LayoutNativeWindowBase();

    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(LayoutBoxBase)
        BEGIN_QI_MAP(LayoutNativeWindowBase)
            QI_MAPPING(ILayoutNativeWindow)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override ILayoutBox
    STDMETHODIMP_(BOOL) IsBoxVisible();

    // Override ILayoutNativeWindow
    STDMETHODIMP SetHWND(HWND hWindow);
    STDMETHODIMP GetHWND(HWND *pWindow);

    // Override ILayoutBoxInternal
    // Native children for BeginDeferWindowPos
    STDMETHODIMP AddNativeWindowCount(DWORD nChildCount);
    STDMETHODIMP RemoveNativeWindowCount(DWORD nChildCount);

protected:
    HWND m_hWindow;
};