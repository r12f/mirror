#include "stdafx.h"
#include "ui/ui_engine/layout/layout_native_window.h"

LayoutNativeWindow::LayoutNativeWindow()
    : m_pEnhancer(NULL)
{

}

LayoutNativeWindow::~LayoutNativeWindow()
{
    if(NULL != m_pEnhancer) {
        delete m_pEnhancer;
    }
}

HRESULT
LayoutNativeWindow::SetHWND(HWND hWindow)
{
    HRESULT hr = LayoutNativeWindowBase::SetHWND(hWindow);
    if(FAILED(hr)) {
        return hr;
    }

    if(NULL == hWindow) {
        return S_OK;
    }

    TCHAR szClassName[1024] = {0};
    ::GetClassName(m_hWindow, szClassName, 1024);

    CString strClassName = szClassName;
    if(strClassName.CompareNoCase(_T("ListBox")) == 0 || strClassName.CompareNoCase(_T("SysTreeView32")) == 0 || strClassName.CompareNoCase(_T("SysListView32")) == 0) {
        m_pEnhancer = new LayoutNativeWindowEnhancer(m_hWindow);
        m_pEnhancer->SetDoubleBufferPaint(FALSE);
        m_pEnhancer->SetCustomBrush(TRUE, FALSE, RGB(255, 255, 255));
    } else if(strClassName.CompareNoCase(_T("Button")) == 0 && ((::GetWindowLong(m_hWindow, GWL_STYLE) & BS_TYPEMASK) == BS_GROUPBOX)) {
        DWORD nStyle = ::GetWindowLong(m_hWindow, GWL_STYLE);
        ::SetWindowLong(m_hWindow, GWL_STYLE, nStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
        m_pEnhancer = new LayoutNativeWindowEnhancer(m_hWindow);
        m_pEnhancer->SetDoubleBufferPaint(FALSE);
        m_pEnhancer->SetCustomBrush(TRUE, TRUE, 0);
        ::SetWindowPos(m_hWindow, HWND_BOTTOM, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
    } else if(strClassName.CompareNoCase(_T("SysLink")) == 0) {
        DWORD nStyle = ::GetWindowLong(m_hWindow, GWL_STYLE);
        ::SetWindowLong(m_hWindow, GWL_STYLE, nStyle | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
    }

    return S_OK;
}

HRESULT
LayoutNativeWindow::LayoutChildrenNoSelf(BOOL bForce)
{
    // We should use the client area of this native window to layout his children
    CRect oClientRect;
    ::GetClientRect(m_hWindow, &oClientRect);

    LayoutContext oContext = {0};
    oContext.m_bForce = bForce;
    CreateLayoutContext(&oContext);

    HRESULT hr = LayoutBoxBase::DoLayoutRelativeChildren(&oClientRect, &oContext);

    EndLayoutRelateChildren(&oContext);

    DWORD i = 0;
    for(i = 0; i < oContext.m_nAffectedNum; ++i) {
        oContext.m_pAffected[i]->LayoutChildrenNoSelf(bForce);
    }

    DestroyLayoutContext(&oContext);

    return S_OK;
}

HRESULT
LayoutNativeWindow::DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext)
{
    if(NULL == pContext || NULL == pContext->m_hDWP) {
        ::SetWindowPos(m_hWindow, NULL,
            pUsefulRect->left, pUsefulRect->top, pUsefulRect->right - pUsefulRect->left, pUsefulRect->bottom - pUsefulRect->top,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_DEFERERASE | SWP_ASYNCWINDOWPOS);
    } else {
        pContext->m_hDWP = ::DeferWindowPos(pContext->m_hDWP, m_hWindow, NULL,
            pUsefulRect->left, pUsefulRect->top, pUsefulRect->right - pUsefulRect->left, pUsefulRect->bottom - pUsefulRect->top,
            SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOREDRAW);
    }

    if(NULL != pContext && NULL != pContext->m_pAffected) {
        QueryInterface(__uuidof(ILayoutBoxInternal), (void **)&(pContext->m_pAffected[pContext->m_nAffectedNum++]));
    }

    return S_OK;
}
