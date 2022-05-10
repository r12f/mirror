#include "stdafx.h"
#include "ui/ui_engine/layout/layout_box_root.h"

LayoutBoxRoot::LayoutBoxRoot()
{

}

LayoutBoxRoot::~LayoutBoxRoot()
{

}

HRESULT
LayoutBoxRoot::SetHWND(HWND hWindow)
{
    HRESULT hr = LayoutNativeWindowBase::SetHWND(hWindow);
    if(FAILED(hr)) {
        return hr;
    }

    if(NULL == hWindow) {
        return S_OK;
    }

    DWORD nStyle = ::GetWindowLong(m_hWindow, GWL_STYLE);
    ::SetWindowLong(m_hWindow, GWL_STYLE, nStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);

    return S_OK;
}