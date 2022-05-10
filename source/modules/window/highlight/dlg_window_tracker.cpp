#include "stdafx.h"
#include "modules/window/highlight/dlg_window_tracker.h"

CDlgWindowTracker::CDlgWindowTracker()
{
    LOGFONT oFont = {0};
    oFont.lfHeight = TRACK_RECT_FONT_SIZE;
    oFont.lfWidth = 0;
    oFont.lfEscapement = 0;
    oFont.lfOrientation = 0;
    oFont.lfWeight = FW_NORMAL;
    oFont.lfItalic = FALSE;
    oFont.lfUnderline = FALSE;
    oFont.lfStrikeOut = FALSE;
    oFont.lfCharSet = ANSI_CHARSET;
    oFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
    oFont.lfClipPrecision = OUT_DEFAULT_PRECIS;
    oFont.lfQuality = DEFAULT_QUALITY;
    oFont.lfPitchAndFamily = DEFAULT_PITCH|FF_SWISS;
    _tcscpy_s(oFont.lfFaceName, LF_FACESIZE, _T("宋体"));
    m_oFont.CreateFontIndirect(&oFont);
}

CDlgWindowTracker::~CDlgWindowTracker()
{

}

BOOL
CDlgWindowTracker::StartTrack(CString &strHint, HWND hTrackWindow, COLORREF nColor)
{
    if(strHint.GetLength() == 0) {
        return FALSE;
    }

    return UpdateTrackWindow(strHint, hTrackWindow, nColor, FALSE);
}

BOOL
CDlgWindowTracker::StopTrack(CString &strHint)
{
    if(strHint.GetLength() == 0) {
        return FALSE;
    }

    BOOL bIsTracking = (m_vTrackWindows.size() != 0);

    TrackWindowList::iterator oWindowIt = m_vTrackWindows.find(strHint);
    if(oWindowIt != m_vTrackWindows.end()) {
        m_vTrackWindows.erase(oWindowIt);
    }

    if(bIsTracking && m_vTrackWindows.size() == 0) {
        KillTimer(TIMER_ID_TRACK_WINDOW);
        KillTimer(TIMER_ID_CHECK_SPARKCLE_WINDOW);
        ShowTrackWindow(FALSE, NULL);
    }

    return TRUE;
}

BOOL
CDlgWindowTracker::Sparkcle(CString &strHint, HWND hWindow, COLORREF nColor)
{
    if(strHint.GetLength() == 0) {
        return FALSE;
    }

    return UpdateTrackWindow(strHint, hWindow, nColor, TRUE);
}

VOID
CDlgWindowTracker::OnTimer(UINT_PTR nTimerId)
{
    switch(nTimerId) {
    case TIMER_ID_TRACK_WINDOW:
        TrackWindow();
        return;
    case TIMER_ID_CHECK_SPARKCLE_WINDOW:
        UpdateSparkcleWindowTimes();
        return;
    }

    SetMsgHandled(FALSE);
}

VOID
CDlgWindowTracker::OnPaint(HDC hDC)
{
    CPaintDC oDC(m_hWnd);

    WTL::CRect oTrackWindowRect;
    GetClientRect(&oTrackWindowRect);

    HBRUSH hBkBrush = (HBRUSH)::GetStockObject(BLACK_BRUSH);
    oDC.FillRect(&oTrackWindowRect, hBkBrush);

    HFONT hOldFont = (HFONT)::SelectObject(oDC, m_oFont);

    CString strHint;
    TrackWindowPaintList vPaintList;
    TrackWindowList::iterator oWindowIt = m_vTrackWindows.begin();
    while(oWindowIt != m_vTrackWindows.end()) {
        if(/*!oWindowIt->second.m_bIsVisible || */oWindowIt->second.m_oRect.IsRectEmpty()) {
            ++oWindowIt;
            continue;
        }

        strHint = oWindowIt->first;

        CPoint oTopLeft = oWindowIt->second.m_oRect.TopLeft();
        vPaintList[MAKELPARAM(oTopLeft.x, oTopLeft.y)].push_back(oWindowIt);
        ++oWindowIt;
    }

    TrackWindowPaintList::iterator oPaintIt = vPaintList.begin();
    while(oPaintIt != vPaintList.end()) {
        MarkWindow(oDC, oPaintIt->first, oPaintIt->second);
        ++oPaintIt;
    }

    ::SelectObject(oDC, hOldFont);
}

VOID
CDlgWindowTracker::OnClose()
{
    SetMsgHandled(FALSE);
    KillTimer(TIMER_ID_TRACK_WINDOW);
    KillTimer(TIMER_ID_CHECK_SPARKCLE_WINDOW);
    DestroyWindow();
}

VOID
CDlgWindowTracker::OnDestroy()
{
    SetMsgHandled(FALSE);
}

BOOL
CDlgWindowTracker::CreateTrackerWindow()
{
    if(NULL != m_hWnd) {
        return TRUE;
    }

    if(NULL == Create(NULL, NULL, NULL, WS_POPUP, WS_EX_TOOLWINDOW | WS_EX_TOPMOST | WS_EX_LAYERED | WS_EX_TRANSPARENT)) {
        return FALSE;
    }

    COLORREF nColor = RGB(0x00, 0x00, 0x00);
    ::SetLayeredWindowAttributes(m_hWnd, nColor, 0, LWA_COLORKEY);

    return TRUE;
}

VOID
CDlgWindowTracker::TrackWindow()
{
    DWORD i = 0;
    WTL::CRect oTotalRect;
    BOOL bHasTrackWindow = FALSE;
    TrackWindowList::iterator oWindowIt = m_vTrackWindows.begin();
    while(oWindowIt != m_vTrackWindows.end()) {
        CWindow oWindow = oWindowIt->second.m_hTrackWindow;
        if(oWindow.IsWindow() && oWindow.GetWindowProcessID() != ::GetCurrentProcessId()) {
            bHasTrackWindow = TRUE;
            oWindowIt->second.m_bIsVisible = oWindow.IsWindowVisible();
            oWindow.GetWindowRect(oWindowIt->second.m_oRect);
            oTotalRect.UnionRect(&oTotalRect, &(oWindowIt->second.m_oRect));
        } else {
            oWindowIt->second.m_bIsVisible = FALSE;
            oWindowIt->second.m_oRect.SetRectEmpty();
        }

        ++oWindowIt;
    }

    if(oTotalRect.Width() < 1000) {
        oTotalRect.right = oTotalRect.left + 1000;
    }

    if(oTotalRect.Height() < 200) {
        oTotalRect.bottom = oTotalRect.top + 200;
    }

    oTotalRect.InflateRect(TRACK_RECT_PADDING, TRACK_RECT_FONT_SIZE + TRACK_RECT_PADDING + TRACK_RECT_TEXT_PADDING_Y * 2, TRACK_RECT_PADDING, TRACK_RECT_PADDING);
    ShowTrackWindow(bHasTrackWindow, &oTotalRect);

    InvalidateRect(NULL, TRUE);

    return;
}

BOOL
CDlgWindowTracker::UpdateTrackWindow(CString & strHint, HWND hTrackWindow, COLORREF nColor, BOOL bIsSparkcle)
{
    BOOL bIsTracking = (m_vTrackWindows.size() != 0);

    TrackWindowList::iterator oWindowIt = m_vTrackWindows.find(strHint);
    if(oWindowIt == m_vTrackWindows.end()) {
        TrackWindowInfo oInfo;
        oInfo.m_hTrackWindow = hTrackWindow;
        oInfo.m_bIsVisible = FALSE;
        oInfo.m_nColor = nColor;
        oInfo.m_oRect.SetRectEmpty();
        oInfo.m_bIsSparkcle = bIsSparkcle;
        oInfo.m_nSparkcleTime = (bIsSparkcle ? TRACK_SPARKCLE_TIME : 0);
        m_vTrackWindows[strHint] = oInfo;
    } else {
        oWindowIt->second.m_hTrackWindow = hTrackWindow;
        oWindowIt->second.m_bIsVisible = FALSE;
        oWindowIt->second.m_nColor = nColor;
        oWindowIt->second.m_oRect.SetRectEmpty();
        oWindowIt->second.m_bIsSparkcle = bIsSparkcle;
        oWindowIt->second.m_nSparkcleTime = (bIsSparkcle ? TRACK_SPARKCLE_TIME : 0);
    }

    if(!bIsTracking) {
        CreateTrackerWindow();
        TrackWindow();
        SetTimer(TIMER_ID_TRACK_WINDOW, TIMER_INTERVAL_TRACK_WINDOW);
        SetTimer(TIMER_ID_CHECK_SPARKCLE_WINDOW, TIMER_INTERVAL_CHECK_SPARKCLE_WINDOW);
    }

    return TRUE;
}

VOID
CDlgWindowTracker::UpdateSparkcleWindowTimes()
{
    BOOL bIsTracking = (m_vTrackWindows.size() != 0);

    TrackWindowList::iterator oWindowIt = m_vTrackWindows.begin();
    while(oWindowIt != m_vTrackWindows.end()) {
        if(oWindowIt->second.m_bIsSparkcle) {
            --oWindowIt->second.m_nSparkcleTime;

            if(oWindowIt->second.m_nSparkcleTime == 0) {
                oWindowIt = m_vTrackWindows.erase(oWindowIt);
                continue;
            }
        }
        ++oWindowIt;
    }

    if(bIsTracking && m_vTrackWindows.size() == 0) {
        KillTimer(TIMER_ID_TRACK_WINDOW);
        KillTimer(TIMER_ID_CHECK_SPARKCLE_WINDOW);
        ShowTrackWindow(FALSE, NULL);
    }

    return ;
}

VOID
CDlgWindowTracker::ShowTrackWindow(BOOL bShow, LPCRECT lpRect)
{
    BOOL bIsTrackWindowVisiable = IsWindowVisible();
    if(!bShow) {
        if(bIsTrackWindowVisiable) {
            SetWindowPos(NULL, 0, 0, 0, 0, SWP_HIDEWINDOW | SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOSIZE);
        }
        return;
    }

    DWORD nFlag = (!bIsTrackWindowVisiable ? SWP_SHOWWINDOW : 0) | SWP_NOACTIVATE;
    SetWindowPos(HWND_TOPMOST, lpRect, nFlag);

    return;
}

VOID
CDlgWindowTracker::MarkWindow(HDC hDC, DWORD nTopLeft, std::list<TrackWindowList::iterator> &vWindowList)
{
    CDCHandle oDC = hDC;

    WTL::CRect oTrackWindowRect;
    GetClientRect(&oTrackWindowRect);

    WTL::CPoint oTopleft(GET_X_LPARAM(nTopLeft), GET_Y_LPARAM(nTopLeft));
    ScreenToClient(&oTopleft);

    WTL::CRect oTextRect, oNextTextRect;
    oTextRect.left = oTopleft.x - TRACK_RECT_PADDING;
    oTextRect.right = oTrackWindowRect.right;
    oTextRect.top = oTopleft.y - (TRACK_RECT_PADDING + TRACK_RECT_FONT_SIZE + TRACK_RECT_TEXT_PADDING_Y * 2);
    oTextRect.bottom = oTopleft.y - TRACK_RECT_PADDING;

    std::list<TrackWindowList::iterator>::iterator oWindowIt = vWindowList.begin();
    while(oWindowIt != vWindowList.end()) {
        if(!(*oWindowIt)->second.m_bIsSparkcle || ((*oWindowIt)->second.m_nSparkcleTime % 2 == 1)) {
            // 画文字
            MarkWindowHint(hDC, oTextRect, (*oWindowIt)->first, (*oWindowIt)->second, oNextTextRect);
            oTextRect = oNextTextRect;

            // 画矩形框
            MarkWindowRect(hDC, (*oWindowIt)->second);
        }

        ++oWindowIt;
    }

    return;
}

VOID
CDlgWindowTracker::MarkWindowHint(HDC hDC, WTL::CRect &oHintRect, const CString &strWindowHint, TrackWindowInfo &oInfo, WTL::CRect &oNextHintRect)
{
    CDCHandle oDC = hDC;
    oNextHintRect = oHintRect;

    CString strHint;
    strHint.Format(_T("%08x: %s"), oInfo.m_hTrackWindow, strWindowHint);

    // 获取文字大小
    WTL::CSize oTextSize;
    oDC.GetTextExtent(strHint, strHint.GetLength(), &oTextSize);

    // 画背景
    LOGBRUSH oTextBkBrushArg = {0};
    oTextBkBrushArg.lbColor = TRACK_RECT_TEXT_BK_COLOR;
    oTextBkBrushArg.lbStyle = BS_SOLID;

    CBrush oTextBkBrush;
    oTextBkBrush.CreateBrushIndirect(&oTextBkBrushArg);

    oHintRect.right = oHintRect.left + oTextSize.cx + TRACK_RECT_TEXT_PADDING_X * 2;
    oDC.FillRect(&oHintRect, oTextBkBrush);

    // 画文字
    oDC.SetBkColor(TRACK_RECT_TEXT_BK_COLOR);
    oDC.SetTextColor(oInfo.m_nColor);
    oDC.TextOut(oHintRect.left + TRACK_RECT_TEXT_PADDING_X, oHintRect.top + TRACK_RECT_TEXT_PADDING_Y, strHint, strHint.GetLength());
    oDC.SetBkColor(TRANSPARENT);

    oNextHintRect.left = oHintRect.right;

    return;
}

VOID
CDlgWindowTracker::MarkWindowRect(HDC hDC, TrackWindowInfo &oInfo)
{
    CDCHandle oDC = hDC;

    WTL::CRect oRect = oInfo.m_oRect, oBrushRect;
    ScreenToClient(&oRect);
    oRect.InflateRect(TRACK_RECT_PADDING, TRACK_RECT_PADDING, TRACK_RECT_PADDING, TRACK_RECT_PADDING);

    LOGBRUSH oTextBkBrushArg = {0};
    oTextBkBrushArg.lbColor = oInfo.m_nColor;
    oTextBkBrushArg.lbStyle = BS_SOLID;

    CBrush hRectBrush;
    hRectBrush.CreateBrushIndirect(&oTextBkBrushArg);
    oBrushRect.SetRect(oRect.left, oRect.top, oRect.right, oRect.top + TRACK_RECT_BORDER);
    oDC.FillRect(oBrushRect, hRectBrush);

    oBrushRect.SetRect(oRect.left, oRect.top, oRect.left + TRACK_RECT_BORDER, oRect.bottom);
    oDC.FillRect(oBrushRect, hRectBrush);

    oBrushRect.SetRect(oRect.right - TRACK_RECT_BORDER, oRect.top, oRect.right, oRect.bottom);
    oDC.FillRect(oBrushRect, hRectBrush);

    oBrushRect.SetRect(oRect.left, oRect.bottom - TRACK_RECT_BORDER, oRect.right, oRect.bottom);
    oDC.FillRect(oBrushRect, hRectBrush);

    return;
}