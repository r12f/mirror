#pragma once

#include <list>
#include <map>

class CDlgWindowTracker :
    public CWindowImpl<CDlgWindowTracker>
{
    enum {
        TIMER_ID_TRACK_WINDOW                       = 0x100,
        TIMER_ID_CHECK_SPARKCLE_WINDOW              = 0x101,
        TIMER_INTERVAL_TRACK_WINDOW                 = 50,   // ms
        TIMER_INTERVAL_CHECK_SPARKCLE_WINDOW        = 500,   // ms
    };

    enum {
        TRACK_RECT_FONT_SIZE        = 14,
        TRACK_RECT_PADDING          = 0,
        TRACK_RECT_BORDER           = 2,
        TRACK_RECT_TEXT_PADDING_X   = 4,
        TRACK_RECT_TEXT_PADDING_Y   = 2,
        TRACK_RECT_TEXT_BK_COLOR    = RGB(0xdd, 0xdd, 0xdd),
    };

    enum {
        TRACK_SPARKCLE_TIME         = 5,
    };

    struct TrackWindowInfo {
        HWND m_hTrackWindow;
        BOOL m_bIsVisible;
        COLORREF m_nColor;
        WTL::CRect m_oRect;
        BOOL m_bIsSparkcle;
        DWORD m_nSparkcleTime;
    };

    typedef std::map<CString, TrackWindowInfo> TrackWindowList;
    typedef std::map<DWORD, std::list<TrackWindowList::iterator>> TrackWindowPaintList;

public:
    enum {
        TRACK_WINDOW_FOREGROUND_INDEX       = 0,
        TRACK_WINDOW_GLOBAL_FOCUS_INDEX,
        TRACK_WINDOW_GLOBAL_ACTIVE_INDEX,
        TRACK_WINDOW_GLOBAL_CAPTURE_INDEX,
        TRACK_WINDOW_NUM,
    };

    DECLARE_WND_CLASS(_T("mirror_track_window"))

    CDlgWindowTracker();
    virtual ~CDlgWindowTracker();

    BOOL StartTrack(CString &strHint, HWND hTrackWindow, COLORREF nColor);
    BOOL StopTrack(CString &strHint);
    BOOL Sparkcle(CString &strHint, HWND hWindow, COLORREF nColor);

    BEGIN_MSG_MAP_EX(CDlgWindowTracker)
        MSG_WM_TIMER(OnTimer)
        MSG_WM_PAINT(OnPaint)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

protected:
    VOID OnTimer(UINT_PTR nTimerId);
    VOID OnPaint(HDC hDC);
    VOID OnClose();
    VOID OnDestroy();

    BOOL UpdateTrackWindow(CString & strHint, HWND hTrackWindow, COLORREF nColor, BOOL bIsSparkcle);
    VOID UpdateSparkcleWindowTimes();
    BOOL CreateTrackerWindow();
    VOID TrackWindow();
    VOID ShowTrackWindow(BOOL bShow, LPCRECT lpRect);
    VOID MarkWindow(HDC hDC, DWORD nTopLeft, std::list<TrackWindowList::iterator> &vWindowList);
    VOID MarkWindowHint(HDC hDC, WTL::CRect &oHintRect, const CString &strWindowHint, TrackWindowInfo &oInfo, WTL::CRect &oNextHintRect);
    VOID MarkWindowRect(HDC hDC, TrackWindowInfo &oInfo);

private:
    CFont m_oFont;
    TrackWindowList m_vTrackWindows;
};