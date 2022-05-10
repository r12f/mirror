#include "stdafx.h"
#include "modules/builtin/component_hotkey_id.h"
#include "modules/window/utils/dwmapi_adapter.h"
#include "modules/window/app/properties/dlg_window_dwm_op.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/app/properties/dlg_window_get_rect.h"

CDlgWindowDwmOp::CDlgWindowDwmOp()
    : CLayout(IDD)
    , CUIControl(this)
    , m_oBtnNCRenderPolicy(CDwmOpMenu::BMI_DWMWA_NCRENDERING_POLICY)
    , m_oBtnTransitionDisabled(CDwmOpMenu::BMI_DWMWA_TRANSITIONS_FORCEDISABLED)
    , m_oBtnAllowNCPaint(CDwmOpMenu::BMI_DWMWA_ALLOW_NCPAINT)
    , m_oBtnNCRtlLayout(CDwmOpMenu::BMI_DWMWA_NONCLIENT_RTL_LAYOUT)
    , m_oBtnForceIconicRpst(CDwmOpMenu::BMI_DWMWA_FORCE_ICONIC_REPRESENTATION)
    , m_oBtnFlip3DPolicy(CDwmOpMenu::BMI_DWMWA_FLIP3D_POLICY)
    , m_oBtnHasIconicBitmap(CDwmOpMenu::BMI_DWMWA_HAS_ICONIC_BITMAP)
    , m_oBtnDisallowPeek(CDwmOpMenu::BMI_DWMWA_DISALLOW_PEEK)
    , m_oBtnExcluedFromPeek(CDwmOpMenu::BMI_DWMWA_EXCLUDED_FROM_PEEK)
{

}

CDlgWindowDwmOp::~CDlgWindowDwmOp()
{

}

LRESULT
CDlgWindowDwmOp::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oBtnTransitionDisabled.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_TRANSITION_DISABLED));
    m_oBtnNCRenderPolicy.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_NC_RENDER_POLICY));
    m_oBtnAllowNCPaint.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_ALLOW_NC_PAINT));
    m_oBtnNCRtlLayout.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_NC_RTL_LAYOUT));
    m_oBtnForceIconicRpst.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_FORCE_ICONIC_RPST));
    m_oBtnFlip3DPolicy.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_FLIP3D_POLICY));
    m_oBtnHasIconicBitmap.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_HAS_ICONIC_BITMAP));
    m_oBtnDisallowPeek.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_DISALLOW_PEEK));
    m_oBtnExcluedFromPeek.SubclassWindow(GetDlgItem(IDC_BUTTON_WINDOW_DWM_EXCLUDE_FROM_PEEK));
    m_oListDwmStatus = GetDlgItem(IDC_LIST_WINDOW_DWM_STATUS);
    m_oListDwmStatus.SetExtendedListViewStyle(m_oListDwmStatus.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMN oColumn = {0};
    oColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    oColumn.fmt = LVCFMT_LEFT;

    oColumn.pszText = _T("ÊôÐÔ");
    oColumn.iSubItem = 0;
    oColumn.cx = 170;
    m_oListDwmStatus.InsertColumn(0, &oColumn);

    oColumn.pszText = _T("×´Ì¬");
    oColumn.iSubItem = 0;
    oColumn.cx = 170;
    m_oListDwmStatus.InsertColumn(1, &oColumn);

    m_oListDwmStatus.InsertItem(WDS_LIST_DWMWA_NCRENDERING_ENABLED_INDEX, _T("NCRENDERING_ENABLED"));
    m_oListDwmStatus.InsertItem(WDS_LIST_DWMWA_CAPTION_BUTTON_BOUNDS_INDEX, _T("CAPTION_BUTTON_BOUNDS"));
    m_oListDwmStatus.InsertItem(WDS_LIST_DWMWA_EXTENDED_FRAME_BOUNDS_INDEX, _T("EXTENDED_FRAME_BOUNDS"));

    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowDwmOp::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowDwmOp::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowDwmOp::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowDwmOp::OnDestroy()
{
    SetMsgHandled(FALSE);

    m_oBtnTransitionDisabled.UnsubclassWindow();
    m_oBtnNCRenderPolicy.UnsubclassWindow();
    m_oBtnAllowNCPaint.UnsubclassWindow();
    m_oBtnNCRtlLayout.UnsubclassWindow();
    m_oBtnForceIconicRpst.UnsubclassWindow();
    m_oBtnFlip3DPolicy.UnsubclassWindow();
    m_oBtnHasIconicBitmap.UnsubclassWindow();
    m_oBtnDisallowPeek.UnsubclassWindow();
    m_oBtnExcluedFromPeek.UnsubclassWindow();

    return;
}

VOID
CDlgWindowDwmOp::OnBtnDwmExtendFrame(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    MARGINS oMargins = { -1, -1, -1, -1 };
    CDlgWindowGetRect oDlgGetRect;
    oDlgGetRect.SetWindowTitle(_T("DWM Extend Frame Into Client Area"));
    oDlgGetRect.SetMargin(oMargins);
    if(IDOK != oDlgGetRect.DoModal()) {
        return;
    }

    oDlgGetRect.GetMargin(&oMargins);

    AdaDwmExtendFrameIntoClientArea(oWatchWindow, &oMargins);

    return;
}

VOID
CDlgWindowDwmOp::OnBtnDwmInvalidateIconicBitmaps(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    AdaDwmInvalidateIconicBitmaps(oWatchWindow);

    return;
}

BOOL
CDlgWindowDwmOp::ResetData(DWORD nParam)
{
    m_bNCRenderingEnabled = FALSE;
    m_oCaptionButtonBounds.SetRectEmpty();
    m_oExtendedFrameBounds.SetRectEmpty();

    return TRUE;
}

BOOL
CDlgWindowDwmOp::FetchData(DWORD nParam)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return FALSE;
    }

    AdaDwmGetWindowAttribute(oWindow.m_hWnd, DWMWA_NCRENDERING_ENABLED, &m_bNCRenderingEnabled, sizeof(m_bNCRenderingEnabled));
    AdaDwmGetWindowAttribute(oWindow.m_hWnd, DWMWA_CAPTION_BUTTON_BOUNDS, &m_oCaptionButtonBounds, sizeof(m_oCaptionButtonBounds));
    AdaDwmGetWindowAttribute(oWindow.m_hWnd, DWMWA_EXTENDED_FRAME_BOUNDS, &m_oExtendedFrameBounds, sizeof(m_oExtendedFrameBounds));
    
    return TRUE;
}

BOOL
CDlgWindowDwmOp::UpdateUI(DWORD nParam)
{
    m_oListDwmStatus.SetItemText(WDS_LIST_DWMWA_NCRENDERING_ENABLED_INDEX, 1, m_bNCRenderingEnabled ? _T("Yes") : _T("No"));

    CString strRect;
    strRect.Format(_T("(%lu, %lu) - (%lu, %lu)"), m_oCaptionButtonBounds.left, m_oCaptionButtonBounds.top, m_oCaptionButtonBounds.right, m_oCaptionButtonBounds.bottom);
    m_oListDwmStatus.SetItemText(WDS_LIST_DWMWA_CAPTION_BUTTON_BOUNDS_INDEX, 1, strRect);

    strRect.Format(_T("(%lu, %lu) - (%lu, %lu)"), m_oExtendedFrameBounds.left, m_oExtendedFrameBounds.top, m_oExtendedFrameBounds.right, m_oExtendedFrameBounds.bottom);
    m_oListDwmStatus.SetItemText(WDS_LIST_DWMWA_EXTENDED_FRAME_BOUNDS_INDEX, 1, strRect);

    return TRUE;
}

VOID
CDlgWindowDwmOp::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_USEWINDOWSTYLE, m_oBtnNCRenderPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMNCRP_USEWINDOWSTYLE, 0},
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_DISABLED, m_oBtnNCRenderPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMNCRP_DISABLED, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_ENABLED, m_oBtnNCRenderPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMNCRP_ENABLED, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_TRANSITIONS_FORCEDISABLED_ON, m_oBtnTransitionDisabled, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_TRANSITIONS_FORCEDISABLED_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_TRANSITIONS_FORCEDISABLED_OFF, m_oBtnTransitionDisabled, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_TRANSITIONS_FORCEDISABLED_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_ALLOW_NCPAINT_ON, m_oBtnAllowNCPaint, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_ALLOW_NCPAINT_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_ALLOW_NCPAINT_OFF, m_oBtnAllowNCPaint, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_ALLOW_NCPAINT_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_NONCLIENT_RTL_LAYOUT_ON, m_oBtnNCRtlLayout, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_NONCLIENT_RTL_LAYOUT_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_NONCLIENT_RTL_LAYOUT_OFF, m_oBtnNCRtlLayout, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_NONCLIENT_RTL_LAYOUT_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_FORCE_ICONIC_REPRESENTATION_ON, m_oBtnForceIconicRpst, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_FORCE_ICONIC_REPRESENTATION_OFF, m_oBtnForceIconicRpst, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_DEFAULT, m_oBtnFlip3DPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMFLIP3D_DEFAULT, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_EXCLUDEBELOW, m_oBtnFlip3DPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMFLIP3D_EXCLUDEBELOW, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_EXCLUDEABOVE, m_oBtnFlip3DPolicy, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMFLIP3D_EXCLUDEABOVE, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_HAS_ICONIC_BITMAP_ON, m_oBtnHasIconicBitmap, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_HAS_ICONIC_BITMAP_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_HAS_ICONIC_BITMAP_OFF, m_oBtnHasIconicBitmap, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_HAS_ICONIC_BITMAP_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_DISALLOW_PEEK_ON, m_oBtnDisallowPeek, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_DISALLOW_PEEK_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_DISALLOW_PEEK_OFF, m_oBtnDisallowPeek, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_DISALLOW_PEEK_OFF, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_EXCLUDED_FROM_PEEK_ON, m_oBtnExcluedFromPeek, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_EXCLUDED_FROM_PEEK_ON, 0 },
        { HOTKEY_SECTION_WINDOW_DWM, HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_EXCLUDED_FROM_PEEK_OFF, m_oBtnExcluedFromPeek, WM_COMMAND, (WPARAM)CDwmOpMenu::BMII_DWMWA_EXCLUDED_FROM_PEEK_OFF, 0 },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}