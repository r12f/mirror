#pragma once

#include "ui/ui_engine/layout/layout_native_window_base.h"
#include "ui/ui_engine/layout/layout_native_window_enhancer.h"

class LayoutBoxRoot :
    public LayoutNativeWindowBase
{
public:
    LayoutBoxRoot();
    virtual ~LayoutBoxRoot();

    COM_CLASS_FACTORY(LayoutBoxRoot)

    // Override ILayoutNativeWindow
    STDMETHODIMP SetHWND(HWND hWindow);
};