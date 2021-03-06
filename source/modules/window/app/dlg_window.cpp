#include "stdafx.h"
#include "modules/component_id.h"
#include "modules/builtin/component_config.h"
#include "modules/window/component_highlight_service.h"
#include "base/string_number_convertions.h"
#include "ui/ui_engine/base/keycode.h"
#include "modules/window/app/dlg_window.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindow::CDlgWindow()
    : CLayout(IDD)
    , CUIControl(this)
    , m_bIsInCapture(FALSE)
    , m_hOldCursor(NULL)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW, (IUnknown *)(IHotkeyNotifyHandler *)this);
}

CDlgWindow::~CDlgWindow()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW);
}

HRESULT
CDlgWindow::OnRegister(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey)
{
    CString strHotkeySection = strSection, strHotkeyName = strName;
    if(strHotkeySection != HOTKEY_SECTION_WINDOW || strHotkeyName != HOTKEY_NAME_WINDOW_SELECT_WINDOW) {
        return S_OK;
    }

    CString strHotkey = _T("未设置！");
    GetWatchWindowHotkey(strHotkey);

    CString strNotice;
    strNotice.Format(_T("拖拽图标选择窗口\r\n热键：%s"), strHotkey);
    m_oStaCaptureNotice.SetWindowText(strNotice);

    return S_OK;
}

HRESULT
CDlgWindow::OnUnregister(BSTR strSection, BSTR strName, IHotkey *pHotkey)
{
    CString strHotkeySection = strSection, strHotkeyName = strName;
    if(strHotkeySection != HOTKEY_SECTION_WINDOW || strHotkeyName != HOTKEY_NAME_WINDOW_SELECT_WINDOW) {
        return S_OK;
    }

    m_oStaCaptureNotice.SetWindowText(_T("拖拽图标选择窗口\r\n热键：未设置！"));

    return S_OK;
}

LRESULT
CDlgWindow::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CString strHotkey = _T("未设置！");
    GetWatchWindowHotkey(strHotkey);

    m_oPicCapture.Attach(GetDlgItem(IDC_STATIC_PIC_CAPTURE));

    m_oStaCaptureNotice.Attach(GetDlgItem(IDC_STATIC_WINDOW_LIST_CAPTURE_NOTICE));

    CString strNotice;
    strNotice.Format(_T("拖拽图标选择窗口\r\n热键：%s"), strHotkey);
    m_oStaCaptureNotice.SetWindowText(strNotice);

    m_oEditWatchWindow = GetDlgItem(IDC_EDIT_WINDOW_WATCH_WINDOW);
    m_oEditWatchWindowOffsetX = GetDlgItem(IDC_EDIT_WINDOW_WATCH_WINDOW_OFFSET_X);
    m_oEditWatchWindowOffsetY = GetDlgItem(IDC_EDIT_WINDOW_WATCH_WINDOW_OFFSET_Y);
    m_oEditWatchWindowClass = GetDlgItem(IDC_EDIT_WINDOW_WATCH_WINDOW_CLASS);
    m_oEditWatchWindowTitle = GetDlgItem(IDC_EDIT_WINDOW_WATCH_WINDOW_TITLE);

    return TRUE;
}

VOID
CDlgWindow::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindow::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    m_oTabWindow.ShowWindow(SW_SHOW);
    return;
}

VOID
CDlgWindow::OnClose()
{
    SetMsgHandled(FALSE);

    CComPtr<IHotkeyManager> pHotkeyMgr = GetComponentInterface<IHotkeyManager>(COMPONENT_HOTKEY);
    if(NULL != pHotkeyMgr) {
        pHotkeyMgr->RemoveHotkeyNotifyHandler(this);
    }

    m_oTabWindow.DestroyWindow();
    m_oDlgWindowProperties.DestroyWindow();

    DestroyWindow();

    return;
}

VOID
CDlgWindow::OnLButtonDown(UINT nFlag, WTL::CPoint oPoint)
{
    WTL::CPoint oCursorPos;
    ::GetCursorPos(&oCursorPos);

    WTL::CRect oRect;
    m_oPicCapture.GetWindowRect(&oRect);
    if(!oRect.PtInRect(oCursorPos)) {
        SetMsgHandled(FALSE);
        return;
    }

    StartCaptureWatchWindow();

    return;
}

VOID
CDlgWindow::OnLButtonUp(UINT nFlag, WTL::CPoint oPoint)
{
    if(!StopCaptureWatchWindow()) {
        SetMsgHandled(FALSE);
    }

    return;
}

VOID
CDlgWindow::OnMouseMove(UINT nFlag, WTL::CPoint oPoint)
{
    if(!m_bIsInCapture) {
        SetMsgHandled(FALSE);
        return;
    }

    CaptureWatchWindow();
    Refresh(FALSE, TRUE, 0);
}

VOID
CDlgWindow::OnTimer(UINT_PTR nTimerId)
{

}

VOID
CDlgWindow::OnBtnSetWatchWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    HWND hWatchWindow = NULL;
    INT nX = 0, nY = 0;

    CString strTemp;
    m_oEditWatchWindow.GetWindowText(strTemp);
    hWatchWindow = (HWND)(DWORD_PTR)HexToInt(strTemp);

    m_oEditWatchWindowOffsetX.GetWindowText(strTemp);
    nX = _ttoi(strTemp);

    m_oEditWatchWindowOffsetY.GetWindowText(strTemp);
    nY = _ttoi(strTemp);
    
    SetWatchWindowWithOffset(hWatchWindow, nX, nY);

    Refresh();

    return;
}

VOID
CDlgWindow::OnBtnSetWindowTitle(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CString strWindowTitle;
    m_oEditWatchWindowTitle.GetWindowText(strWindowTitle);

    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    oWatchWindow.SetWindowText(strWindowTitle);

    return;
}

VOID
CDlgWindow::OnBtnTrackWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    if(nNotifyCode != BN_CLICKED || !UpdateTrackWindow(nId)) {
        SetMsgHandled(FALSE);
    }

    return;
}

HWND
CDlgWindow::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;

    if(0 == strResId.CompareNoCase("IDC_TAB_WINDOW")) {
        m_oTabWindow.Create(m_hWnd);

        m_oDlgWindowList.Create(m_oTabWindow.m_tab);
        m_oTabWindow.AddPage(m_oDlgWindowList, _T("窗口列表"));

        m_oDlgWindowProperties.Create(m_oTabWindow.m_tab);
        m_oTabWindow.AddPage(m_oDlgWindowProperties, _T("窗口信息"));

        m_oTabWindow.SetActivePage(0);

        ProvideHotkey();

        return m_oTabWindow;
    }

    return NULL;
}

BOOL
CDlgWindow::DoOperation(UINT nOpId, LPARAM lParam)
{
    switch(nOpId) {
    case UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND:
        SwitchToMe(COMPONENT_UI_DLG_MAIN);
        SwitchToTabByHWND(m_oTabWindow, (HWND)lParam);
        break;
    case UIOP_WINDOW_CAPTURE_WINDOW_UNDER_MOUSE:
        CaptureWatchWindow(FALSE);
        Refresh();
        break;
    case UIOP_WINDOW_SHOW_WINDOW_TREE:
        SwitchToTabByHWND(m_oTabWindow, m_oDlgWindowList.m_hWnd);
        break;
    case UIOP_WINDOW_SHOW_WINDOW_PROPERTIES:
        SwitchToTabByHWND(m_oTabWindow, m_oDlgWindowProperties.m_hWnd);
        break;
    case UIOP_WBI_TRACK_WATCH_WINDOW:
        {
            CButton oCheck = GetDlgItem(IDC_CHECK_WINDOW_TRACK_WATCH_WINDOW);
            if(!oCheck.IsWindow()) {
                return TRUE;
            }
            oCheck.SetCheck(!oCheck.GetCheck());
            UpdateTrackWindow(IDC_CHECK_WINDOW_TRACK_WATCH_WINDOW);
        }
        break;
    }
    return TRUE;
}

BOOL
CDlgWindow::ResetData(DWORD nParam)
{
    return TRUE;
}

BOOL
CDlgWindow::FetchData(DWORD nParam)
{
    return TRUE;
}

BOOL
CDlgWindow::UpdateUI(DWORD nParam)
{
    CWindow oWatchWindow;
    INT nX = 0, nY = 0;
    if(!GetWatchWindowWithOffset(&oWatchWindow.m_hWnd, &nX, &nY)) {
        return FALSE;
    }

    CString strTemp;
    strTemp.Format(_T("%08X"), oWatchWindow.m_hWnd);
    m_oEditWatchWindow.SetWindowText(strTemp);

    strTemp.Format(_T("%ld"), nX);
    m_oEditWatchWindowOffsetX.SetWindowText(strTemp);

    strTemp.Format(_T("%ld"), nY);
    m_oEditWatchWindowOffsetY.SetWindowText(strTemp);

    TCHAR szClassName[1024] = {0};
    if(oWatchWindow.IsWindow()) {
        ::GetClassName(oWatchWindow, szClassName, 1023);
        oWatchWindow.GetWindowText(strTemp);
    } else {
        strTemp = _T("");
    }

    m_oEditWatchWindowClass.SetWindowText(szClassName);
    m_oEditWatchWindowTitle.SetWindowText(strTemp);

    UpdateTrackWindow(IDC_CHECK_WINDOW_TRACK_WATCH_WINDOW);

    return TRUE;
}

BOOL
CDlgWindow::RefreshChildren(BOOL bForceReload, DWORD nParam)
{
    m_oDlgWindowList.RequestDoOperation(UIOP_WINDOW_LIST_SELECT_WATCH_WINDOW, NULL);
    m_oDlgWindowProperties.RequestRefresh(TRUE, TRUE, nParam);

    return TRUE;
}

void
CDlgWindow::StartCaptureWatchWindow()
{
    m_bIsInCapture = TRUE;

    HCURSOR hCursor = ::LoadCursor(GetModuleHandle(NULL), MAKEINTRESOURCE(IDC_CURSOR_PICKER));
    m_hOldCursor = ::SetCursor(hCursor);

    SetCapture();

    return;
}

BOOL
CDlgWindow::StopCaptureWatchWindow()
{
    if(!m_bIsInCapture) {
        return FALSE;
    }

    ReleaseCapture();

    if(NULL != m_hOldCursor) {
        ::SetCursor(m_hOldCursor);
    }

    CaptureWatchWindow();
    Refresh();

    CComPtr<IHighlightService> pHighlightService = GetComponentInterface<IHighlightService>(COMPONENT_WINDOW_HIGHLIGHT_SERVICE);
    if(NULL == pHighlightService) {
        return FALSE;
    }

    pHighlightService->StopHighlight(CComBSTR(_T("鼠标下窗口")));

    m_bIsInCapture = FALSE;

    return TRUE;
}


VOID
CDlgWindow::CaptureWatchWindow(BOOL bHighlight)
{
    WTL::CPoint oCursorPos;
    ::GetCursorPos(&oCursorPos);

    HWND hWindow = ::WindowFromPoint(oCursorPos);
    if(NULL == hWindow || !::IsWindow(hWindow)) {
        return;
    }

    WTL::CPoint oChildPos = oCursorPos;
    ::ScreenToClient(hWindow, &oChildPos);

    HWND hRealChildWindow = ::RealChildWindowFromPoint(hWindow, oChildPos);
    ::ScreenToClient(hRealChildWindow, &oCursorPos);
    if(NULL == hRealChildWindow) {
        hRealChildWindow = hWindow;
    }

    SetWatchWindowWithOffset(hRealChildWindow, oCursorPos.x, oCursorPos.y);
    
    if(!bHighlight) {
        return;
    }

    CComPtr<IHighlightService> pHighlightService = GetComponentInterface<IHighlightService>(COMPONENT_WINDOW_HIGHLIGHT_SERVICE);
    if(NULL == pHighlightService) {
        return;
    }

    pHighlightService->StartHighlight(CComBSTR(_T("鼠标下窗口")), hRealChildWindow, RGB(16, 16, 16));

    return;
}

VOID
CDlgWindow::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_WINDOW, HOTKEY_NAME_WINDOW_SELECT_WINDOW, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WINDOW_CAPTURE_WINDOW_UNDER_MOUSE, 0 },
        { HOTKEY_SECTION_WINDOW, HOTKEY_NAME_WINDOW_TRACK_WATCH_WINDOW, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_WATCH_WINDOW, 0 },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_LIST, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgWindowList.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INFO, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgWindowProperties.m_hWnd },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    CComPtr<IHotkeyManager> pHotkeyMgr = GetComponentInterface<IHotkeyManager>(COMPONENT_HOTKEY);
    if(NULL != pHotkeyMgr) {
        pHotkeyMgr->AddHotkeyNotifyHandler(this);
    }
    
    return;
}

BOOL
CDlgWindow::UpdateTrackWindow(UINT nCheckId)
{
    CString strTrackHint;
    HWND hTrackWindow = NULL;
    COLORREF nColor = 0;

    switch(nCheckId) {
    case IDC_CHECK_WINDOW_TRACK_WATCH_WINDOW:
        strTrackHint = _T("观察窗口");
        hTrackWindow = GetWatchWindow();
        nColor = RGB(16, 16, 16);
        break;
    default:
        SetMsgHandled(FALSE);
        return FALSE;
    }

    CButton oButton = GetDlgItem(nCheckId);
    if(!oButton.IsWindow()) {
        return FALSE;
    }

    CComPtr<IHighlightService> pHighlightService = GetComponentInterface<IHighlightService>(COMPONENT_WINDOW_HIGHLIGHT_SERVICE);
    if(NULL == pHighlightService) {
        return FALSE;
    }

    if(oButton.GetCheck()) {
        pHighlightService->StartHighlight(CComBSTR(strTrackHint), hTrackWindow, nColor);
    } else {
        pHighlightService->StopHighlight(CComBSTR(strTrackHint));
    }

    return TRUE;
}

BOOL
CDlgWindow::GetWatchWindowHotkey(CString &strHotkey)
{
    CComPtr<IHotkeyManager> pHotkeyMgr = GetComponentInterface<IHotkeyManager>(COMPONENT_HOTKEY);
    if(NULL == pHotkeyMgr) {
        return FALSE;
    }

    CComPtr<IHotkey> pHotkey;
    pHotkeyMgr->GetHotkey(CComBSTR(HOTKEY_SECTION_WINDOW), CComBSTR(HOTKEY_NAME_WINDOW_SELECT_WINDOW), &pHotkey);
    if(NULL == pHotkey) {
        return FALSE;
    }

    BOOL bIsHotkeyGlobal = FALSE;
    UINT nHotkeyModifier = 0, nHotkeyKey = 0;
    pHotkey->GetModifier(&nHotkeyModifier);
    pHotkey->GetKey(&nHotkeyKey);
    pHotkey->GetGlobal(&bIsHotkeyGlobal);

    if(0 == nHotkeyModifier && 0 == nHotkeyKey) {
        return FALSE;
    }

    CString strModifier, strKeyCode;
    ModifierToModifierName(nHotkeyModifier, strModifier);
    KeyCodeToKeyName(NativeKeyToKeyCode(nHotkeyKey), strKeyCode);
    if(strModifier.GetLength() == 0 && strKeyCode.GetLength() == 0) {
        return FALSE;
    }

    if(strModifier.GetLength() == 0) {
        strHotkey.Format(_T("%s （%s）"), strKeyCode, bIsHotkeyGlobal ? _T("全局") : _T("非全局"));
    } else if(strKeyCode.GetLength() == 0) {
        strHotkey.Format(_T("%s （%s）"), strModifier, bIsHotkeyGlobal ? _T("全局") : _T("非全局"));
    } else {
        strHotkey.Format(_T("%s+%s （%s）"), strModifier, strKeyCode, bIsHotkeyGlobal ? _T("全局") : _T("非全局"));
    }

    return TRUE;
}
