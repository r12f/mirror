#include "stdafx.h"
#include "ui/ui_engine/layout/layout_native_window_enhancer.h"

LayoutNativeWindowEnhancer::LayoutNativeWindowEnhancer(HWND hWnd)
: m_bNeedDoubleBufferPaint(FALSE)
, m_bUseBrush(TRUE)
{
    SubclassWindow(hWnd);
}

LayoutNativeWindowEnhancer::~LayoutNativeWindowEnhancer()
{
    
}

void
LayoutNativeWindowEnhancer::SetDoubleBufferPaint(BOOL bNeedDoubleBufferPaint)
{
    m_bNeedDoubleBufferPaint = bNeedDoubleBufferPaint;
}

void
LayoutNativeWindowEnhancer::SetCustomBrush(BOOL bUseBrush, BOOL bUseParent, COLORREF nColor)
{
    m_bUseBrush = bUseBrush;

    if(!m_bUseBrush) {
        if(NULL != m_hCustomBrush) {
            m_hCustomBrush.DeleteObject();
            m_hCustomBrush = NULL;
        }
        return;
    }

    if(!bUseParent) {
        m_hCustomBrush.CreateSolidBrush(nColor);
        return;
    }

    m_hCustomBrush.CreateSolidBrush(::GetSysColor(COLOR_3DFACE | COLOR_BTNFACE));
}

VOID
LayoutNativeWindowEnhancer::OnPaint(HDC hDC)
{
    if(!m_bNeedDoubleBufferPaint && NULL == m_hCustomBrush) {
        SetMsgHandled(FALSE);
        return;
    }

    CRect oPaintRect;
    GetClientRect(&oPaintRect);

    CPaintDC oWindowDC(m_hWnd);
    CDCHandle oDC = oWindowDC;
    CDC oMemDC;
    CBitmap oBitmap;
    CBitmapHandle oOldBitmap;
    if(m_bNeedDoubleBufferPaint) {
        if(!oMemDC.CreateCompatibleDC(hDC)) {
            return;
        }

        if(!oBitmap.CreateCompatibleBitmap(hDC, oPaintRect.Width(), oPaintRect.Height())) {
            return;
        }

        oOldBitmap = oMemDC.SelectBitmap(oBitmap);
        
        oDC = oMemDC;
    }

    if(NULL != m_hCustomBrush) {
        oDC.FillRect(oPaintRect, m_hCustomBrush);
    }

    DefWindowProc(WM_PAINT, (WPARAM)oDC.m_hDC, NULL);

    if(m_bNeedDoubleBufferPaint) {
        ::BitBlt(hDC, 0, 0, oPaintRect.Width(), oPaintRect.Height(), oDC, 0, 0, SRCCOPY);
        oMemDC.SelectBitmap(oOldBitmap);
    }

    return;
}

LRESULT
LayoutNativeWindowEnhancer::OnEraseBkGnd(HDC hDC)
{
    if(m_bUseBrush && NULL == m_hCustomBrush) {
        return DefWindowProc();
    }

    return 1;
}

VOID
LayoutNativeWindowEnhancer::OnClose()
{
    SetMsgHandled(FALSE);
    UnsubclassWindow();
    DefWindowProc();
    return;
}

VOID
LayoutNativeWindowEnhancer::OnDestroy()
{
    SetMsgHandled(FALSE);
    UnsubclassWindow();
    DefWindowProc();
    return;
}