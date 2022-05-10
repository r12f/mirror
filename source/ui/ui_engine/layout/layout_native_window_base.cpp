#include "stdafx.h"
#include "ui/ui_engine/layout/layout_native_window_base.h"

LayoutNativeWindowBase::LayoutNativeWindowBase()
    : m_hWindow(NULL)
{
    m_nNativeWindowCount = 1;
}

LayoutNativeWindowBase::~LayoutNativeWindowBase()
{

}

BOOL
LayoutNativeWindowBase::IsBoxVisible()
{
    return ::IsWindow(m_hWindow) && ::IsWindowVisible(m_hWindow);
}

HRESULT
LayoutNativeWindowBase::SetHWND(HWND hWindow)
{
    m_hWindow = hWindow;
    return S_OK;
}

HRESULT
LayoutNativeWindowBase::GetHWND(HWND *pWindow)
{
    ASSERT_RETURN(NULL != pWindow, E_POINTER);
    *pWindow = m_hWindow;
    return S_OK;
}

HRESULT
LayoutNativeWindowBase::AddNativeWindowCount(DWORD nChildCount)
{
    m_nNativeWindowCount += nChildCount;
    return S_OK;
}

HRESULT
LayoutNativeWindowBase::RemoveNativeWindowCount(DWORD nChildCount)
{
    m_nNativeWindowCount -= nChildCount;
    return S_OK;
}
