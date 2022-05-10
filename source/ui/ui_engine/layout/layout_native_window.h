#pragma once

#include "ui/ui_engine/layout/layout_native_window_base.h"
#include "ui/ui_engine/layout/layout_native_window_enhancer.h"

class LayoutNativeWindow :
    public LayoutNativeWindowBase
{
public:
    LayoutNativeWindow();
    virtual ~LayoutNativeWindow();

    COM_CLASS_FACTORY(LayoutNativeWindow)

    // Override ILayoutNativeWindow
    STDMETHODIMP SetHWND(HWND hWindow);

    // Override ILayoutBoxInternal
    STDMETHODIMP LayoutChildrenNoSelf(BOOL bForce);

protected:
    virtual HRESULT DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext);

protected:
    LayoutNativeWindowEnhancer *m_pEnhancer;
};