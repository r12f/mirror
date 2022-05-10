#pragma once

class LayoutNativeWindowEnhancer :
    public CWindowImpl<LayoutNativeWindowEnhancer>
{
public:
    LayoutNativeWindowEnhancer(HWND hWnd);
    virtual ~LayoutNativeWindowEnhancer();

    void SetDoubleBufferPaint(BOOL bNeedDoubleBufferPaint);
    void SetCustomBrush(BOOL bUseBrush, BOOL bUseParent, COLORREF nColor);

    BEGIN_MSG_MAP_EX(LayoutNativeWindowEnhancer)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_ERASEBKGND(OnEraseBkGnd)
    END_MSG_MAP()

protected:
    VOID OnPaint(HDC hDC);
    LRESULT OnEraseBkGnd(HDC hDC);
    VOID OnClose();
    VOID OnDestroy();

private:
    BOOL m_bNeedDoubleBufferPaint;
    BOOL m_bUseBrush;
    CBrush m_hCustomBrush;
};