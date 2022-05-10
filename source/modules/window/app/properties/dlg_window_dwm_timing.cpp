#include "stdafx.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/app/properties/dlg_window_dwm_timing.h"

enum {
    DWM_TIMING_DATA_TYPE_UNSIGNED_RATIO     = 1,
    DWM_TIMING_DATA_TYPE_QPC_TIME,
    DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT,
    DWM_TIMING_DATA_TYPE_UINT,
    DWM_TIMING_DATA_TYPE_ULONGLONG,
};

struct DlgDwmTimingField {
    UINT m_nCol;
    DWORD m_nDataType;
    DWORD m_nOffset;
    LPCTSTR m_pLabel;
    LPCTSTR m_pTip;
};

#define MEMBER_OFFSET(c, m)     (DWORD)(&(((c *)(NULL))->m))

static DlgDwmTimingField s_vDwmTimingFields[] = {
    { 0, DWM_TIMING_DATA_TYPE_UNSIGNED_RATIO, MEMBER_OFFSET(DWM_TIMING_INFO, rateRefresh), _T("显示器刷新率:"), _T("Monitor refresh rate") },
    { 0, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcRefreshPeriod), _T("显示器刷新耗时:"), _T("Actual period") },
    { 0, DWM_TIMING_DATA_TYPE_UNSIGNED_RATIO, MEMBER_OFFSET(DWM_TIMING_INFO, rateCompose), _T("合成时间:"), _T("composition rate") },
    { 0, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcVBlank), _T("垂直同步中断耗时:"), _T("QPC time at a VSync interupt") },
    { 0, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefresh), _T("垂直同步刷新次数:"), _T("DWM refresh count of the last vsync.\r\nDWM refresh count is a 64bit number where zero is the first refresh the DWM woke up to process.") },
    { 0, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cDXRefresh), _T("垂直同步DX刷新次数:"), _T("DX refresh count at the last Vsync Interupt.\r\nDX refresh count is a 32bit number with zero being the first refresh after the card was initialized.\r\nDX increments a counter when ever a VSync ISR is processed.\r\nIt is possible for DX to miss VSyncs\r\n\r\nThere is not a fixed mapping between DX and DWM refresh counts because the DX will rollover and may miss VSync interupts.") },
    { 0, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcCompose), _T("合成耗时:"), _T("QPC time at a compose time.  ") },
    { 0, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFrame), _T("合成帧数:"), _T("Frame number that was composed at qpcCompose") },
    { 0, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cDXPresent), _T("DXPresent:"), _T("The present number DX uses to identify renderer frames") },
    { 0, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshFrame), _T("刷新合成帧数:"), _T("Refresh count of the frame that was composed at qpcCompose") },
    { 1, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFrameSubmitted), _T("提交帧数:"), _T("DWM frame number that was last submitted") },
    { 1, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cDXPresentSubmitted), _T("提交帧数（DX）:"), _T("DX Present number that was last submitted") },
    { 1, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFrameConfirmed), _T("确认帧数:"), _T("DWM frame number that was last confirmed presented") },
    { 1, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cDXPresentConfirmed), _T("确认帧数（DX）:"), _T("DX Present number that was last confirmed presented") },
    { 1, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshConfirmed), _T("确认刷新帧数:"), _T("The target refresh count of the last frame confirmed completed by the GPU") },
    { 1, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cDXRefreshConfirmed), _T("确认刷新帧数（DX）:"), _T("DX refresh count when the frame was confirmed presented") },
    { 1, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesLate), _T("延迟帧数:"), _T("Number of frames the DWM presented late AKA Glitches") },
    { 1, DWM_TIMING_DATA_TYPE_UINT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesOutstanding), _T("未完成帧数:"), _T("The number of composition frames that have been issued but not confirmed completed") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFrameDisplayed), _T("显示帧数:"), _T("Last frame displayed") },
    { 2, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcFrameDisplayed), _T("显示时间:"), _T("QPC time of the composition pass when the frame was displayed") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshFrameDisplayed), _T("刷新帧显示数:"), _T("Count of the VSync when the frame should have become visible") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFrameComplete), _T("完成帧数:"), _T("ID of the the last frame marked complete (starts at 0)") },
    { 2, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcFrameComplete), _T("完成耗时:"), _T("QPC time when the last frame was marked complete") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramePending), _T("等待帧数:"), _T("ID of the the last frame marked pending (starts at 0)") },
    { 2, DWM_TIMING_DATA_TYPE_QPC_TIME, MEMBER_OFFSET(DWM_TIMING_INFO, qpcFramePending), _T("等待耗时:"), _T("QPC time when the last frame was marked pending") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesDisplayed), _T("特殊帧显示数:"), _T("number of unique frames displayed") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesComplete), _T("新完成帧数:"), _T("number of new completed frames that have been received") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesPending), _T("新等待帧数:"), _T("number of new frames submitted to DX but not yet complete") },
    { 2, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesAvailable), _T("可用帧数:"), _T("number of frames available but not displayed, used or dropped") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesDropped), _T("遗弃帧数:"), _T("number of rendered frames that were never displayed because composition occured too late") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cFramesMissed), _T("丢失帧数:"), _T("number of times an old frame was composed when a new frame should have been used but was not available") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshNextDisplayed), _T("RefreshNextDisplayed:"), _T("the refresh at which the next frame is scheduled to be displayed") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshNextPresented), _T("RefreshNextPresented:"), _T("the refresh at which the next DX present is scheduled to be displayed") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshesDisplayed), _T("RefreshesDisplayed:"), _T("The total number of refreshes worth of content for this HWND that have been displayed by the DWM since DwmSetPresentParameters was called") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshesPresented), _T("RefreshesPresented:"), _T("The total number of refreshes worth of content that have been presented by the application since DwmSetPresentParameters was called") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cRefreshStarted), _T("RefreshStarted:"), _T("The actual refresh # when content for this window started to be displayed it may be different than that requested DwmSetPresentParameters") },
    { 3, DWM_TIMING_DATA_TYPE_ULONGLONG, MEMBER_OFFSET(DWM_TIMING_INFO, cPixelsReceived), _T("PixelsReceived:"), _T("Total number of pixels DX redirected to the DWM. If Queueing is used the full buffer is transfered on each present. If not queuing it is possible only a dirty region is updated") },
    { 3, DWM_TIMING_DATA_TYPE_ULONGLONG, MEMBER_OFFSET(DWM_TIMING_INFO, cPixelsDrawn), _T("PixelsDrawn:"), _T("Total number of pixels drawn. Does not take into account if if the window is only partial drawn do to clipping or dirty rect management ") },
    { 3, DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT, MEMBER_OFFSET(DWM_TIMING_INFO, cBuffersEmpty), _T("BuffersEmpty:"), _T("The number of buffers in the flipchain that are empty. An application can present that number of times and guarantee it won't be blocked waiting for a buffer to become empty to present to") },
};

static DWORD s_nDwmTimingFieldNum = sizeof(s_vDwmTimingFields) / sizeof(DlgDwmTimingField);

#define DWM_TIMING_DATA_COLUMN_LABEL_WIDTH    130
#define DWM_TIMING_DATA_COLUMN_VALUE_WIDTH    50
#define DWM_TIMING_DATA_LINE_HEIGHT           40

CDlgWindowDwmTiming::CDlgWindowDwmTiming()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgWindowDwmTiming::~CDlgWindowDwmTiming()
{

}

LRESULT
CDlgWindowDwmTiming::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    _Module.GetMessageLoop()->AddMessageFilter(this);

    m_oTooltip.Create(m_hWnd);
    m_oTooltip.Activate(TRUE);

    m_pDwmLabels = new CStatic[s_nDwmTimingFieldNum];
    m_pDwmTimingInfos = new CStatic[s_nDwmTimingFieldNum];
    if(NULL == m_pDwmLabels || NULL == m_pDwmTimingInfos) {
        if(NULL != m_pDwmLabels) {
            delete [] m_pDwmLabels;
            m_pDwmLabels = NULL;
        }

        if(NULL != m_pDwmTimingInfos) {
            delete [] m_pDwmTimingInfos;
            m_pDwmTimingInfos = NULL;
        }

        return FALSE;
    }

    HFONT hWindowFont = (HFONT)::SendMessage(m_hWnd, WM_GETFONT, 0, 0);
    if (hWindowFont != NULL) {
        CLogFont oLogFont(hWindowFont);
        m_oFont = oLogFont.CreateFontIndirect();
    }

    DWORD i = 0;
    for(i = 0; i < s_nDwmTimingFieldNum; ++i) {
        m_pDwmLabels[i].Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY);
        m_pDwmLabels[i].SetWindowText(s_vDwmTimingFields[i].m_pLabel);
        if(m_oFont != NULL) {
            m_pDwmLabels[i].SetFont(m_oFont, TRUE);
        }

        m_pDwmTimingInfos[i].Create(m_hWnd, NULL, NULL, WS_CHILD | WS_VISIBLE | SS_LEFT | SS_NOTIFY);
        if(m_oFont != NULL) {
            m_pDwmTimingInfos[i].SetFont(m_oFont, TRUE);
        }

        m_oTooltip.AddTool(m_pDwmLabels[i], s_vDwmTimingFields[i].m_pTip);
        m_oTooltip.AddTool(m_pDwmTimingInfos[i], s_vDwmTimingFields[i].m_pTip);
    }

    return TRUE;
}

VOID
CDlgWindowDwmTiming::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowDwmTiming::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowDwmTiming::OnClose()
{
    SetMsgHandled(FALSE);

    DWORD i = 0;
    for(i = 0; i < s_nDwmTimingFieldNum; ++i) {
        m_pDwmLabels[i].DestroyWindow();
        m_pDwmTimingInfos[i].DestroyWindow();
    }

    delete [] m_pDwmLabels;
    m_pDwmLabels = NULL;

    delete [] m_pDwmTimingInfos;
    m_pDwmTimingInfos = NULL;

    _Module.GetMessageLoop()->RemoveMessageFilter(this);

    DestroyWindow();

    return;
}

VOID
CDlgWindowDwmTiming::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

BOOL
CDlgWindowDwmTiming::PreTranslateMessage(MSG* pMsg)
{
    if(m_oTooltip.IsWindow()) {
        m_oTooltip.RelayEvent(pMsg);
    }

    return FALSE;
}

void
CDlgWindowDwmTiming::OnFinalMessage(HWND)
{
    return;
}

BOOL
CDlgWindowDwmTiming::DoCustomLayout()
{
    WTL::CRect oRect;
    GetClientRect(&oRect);
    oRect.DeflateRect(10, 10, 10, 10);

    DWORD i = 0, nCurLine = 0, nLine = 0, nCol = 0;
    for(i = 0; i < s_nDwmTimingFieldNum; ++i) {
        if(nCol != s_vDwmTimingFields[i].m_nCol) {
            nCurLine = 0;
            nCol = s_vDwmTimingFields[i].m_nCol;
        }

        ++nCurLine;

        if(nCurLine > nLine) {
            nLine = nCurLine;
        }
    }

    ++nCol;

    DWORD nLastFieldCol = 0, nFieldLine = 0;
    DWORD nFieldWidth = oRect.Width() / nCol, nFieldHeight = oRect.Height() / nLine;
    DWORD nFieldX = 0, nFieldY = 0;
    WTL::CRect oLabelRect, oValueRect;
    for(i = 0; i < s_nDwmTimingFieldNum; ++i) {
        if(nLastFieldCol != s_vDwmTimingFields[i].m_nCol) {
            nFieldLine = 0;
            nLastFieldCol = s_vDwmTimingFields[i].m_nCol;
        }

        nFieldX = oRect.left + s_vDwmTimingFields[i].m_nCol * nFieldWidth;
        nFieldY = oRect.top + nFieldLine * nFieldHeight;

        oLabelRect.SetRect(nFieldX, nFieldY, nFieldX + DWM_TIMING_DATA_COLUMN_LABEL_WIDTH, nFieldY + DWM_TIMING_DATA_LINE_HEIGHT);
        m_pDwmLabels[i].SetWindowPos(NULL, &oLabelRect, SWP_NOZORDER);

        oValueRect.SetRect(oLabelRect.right, nFieldY, oLabelRect.right + DWM_TIMING_DATA_COLUMN_VALUE_WIDTH, nFieldY + DWM_TIMING_DATA_LINE_HEIGHT);
        m_pDwmTimingInfos[i].SetWindowPos(NULL, &oValueRect, SWP_NOZORDER);

        ++nFieldLine;
    }

    return TRUE;
}

BOOL
CDlgWindowDwmTiming::ResetData(DWORD nParam)
{
    memset(&m_oDwmTimingInfo, 0, sizeof(m_oDwmTimingInfo));
    return TRUE;
}

BOOL
CDlgWindowDwmTiming::FetchData(DWORD nParam)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return FALSE;
    }

    m_oDwmTimingInfo.cbSize = sizeof(m_oDwmTimingInfo);
    AdaDwmGetCompositionTimingInfo(oWatchWindow, &m_oDwmTimingInfo);

    return TRUE;
}

BOOL
CDlgWindowDwmTiming::UpdateUI(DWORD nParam)
{
    DWORD i = 0;
    PVOID pData = NULL;
    CString strTimingInfo;
    for(i = 0; i < s_nDwmTimingFieldNum; ++i) {
        pData = (((BYTE *)&m_oDwmTimingInfo) + s_vDwmTimingFields[i].m_nOffset);
        switch(s_vDwmTimingFields[i].m_nDataType) {
        case DWM_TIMING_DATA_TYPE_UNSIGNED_RATIO:
            {
                UNSIGNED_RATIO *pInfo = (UNSIGNED_RATIO *)pData;
                strTimingInfo.Format(_T("%lu.%lu"), pInfo->uiNumerator, pInfo->uiDenominator);
            }
            break;
        case DWM_TIMING_DATA_TYPE_QPC_TIME:
            {
                QPC_TIME *pInfo = (QPC_TIME *)pData;
                strTimingInfo.Format(_T("%I64u"), *pInfo);
            }
            break;
        case DWM_TIMING_DATA_TYPE_DWM_FRAME_COUNT:
            {
                DWM_FRAME_COUNT *pInfo = (DWM_FRAME_COUNT *)pData;
                strTimingInfo.Format(_T("%I64u"), *pInfo);
            }
            break;
        case DWM_TIMING_DATA_TYPE_UINT:
            {
                UINT *pInfo = (UINT *)pData;
                strTimingInfo.Format(_T("%lu"), *pInfo);
            }
            break;
        case DWM_TIMING_DATA_TYPE_ULONGLONG:
            {
                ULONGLONG *pInfo = (ULONGLONG *)pData;
                strTimingInfo.Format(_T("%I64u"), *pInfo);
            }
            break;
        }

        m_pDwmTimingInfos[i].SetWindowText(strTimingInfo);
    }

    return TRUE;
}