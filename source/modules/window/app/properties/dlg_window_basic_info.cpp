#include "stdafx.h"
#include "modules/builtin/component_config.h"
#include "modules/builtin/component_hotkey.h"
#include "modules/window/component_highlight_service.h"
#include "base/string_number_convertions.h"
#include "base/system/process_control.h"
#include "base/system/thread_control.h"
#include "modules/window/utils/window_output.h"
#include "modules/window/utils/window_utils.h"
#include "modules/window/app/properties/dlg_window_basic_info.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/app/properties/dlg_window_get_window.h"
#include "modules/window/app/properties/dlg_window_get_rect.h"
#include "modules/window/app/properties/dlg_window_set_layered_window.h"

CDlgWindowBasicInfo::CDlgWindowBasicInfo()
    : CLayout(IDD)
    , CUIControl(this)
    , m_oBtnAddStyle(AddStyleMenu::ADD_STYLE_MENU_WINDOW_STYLE)
    , m_oBtnAddExStyle(AddStyleMenu::ADD_STYLE_MENU_WINDOW_EXSTYLE)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_BASIC_INFO, this);
}

CDlgWindowBasicInfo::~CDlgWindowBasicInfo()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_BASIC_INFO);
}

LRESULT
CDlgWindowBasicInfo::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_vEditList[WBI_LIST_PID_INDEX] = GetDlgItem(IDC_EDIT_WBI_PROCESS);
    m_vEditList[WBI_LIST_WINDOW_PROC_INDEX] = GetDlgItem(IDC_EDIT_WBI_WND_PROC);
    m_vEditList[WBI_LIST_USER_DATA] = GetDlgItem(IDC_EDIT_WBI_USER_DATA);
    m_vEditList[WBI_LIST_STYLE_INDEX] = GetDlgItem(IDC_EDIT_WBI_STYLE);
    m_vEditList[WBI_LIST_EXSTYLE_INDEX] = GetDlgItem(IDC_EDIT_WBI_EXSTYLE);
    m_vEditList[WBI_LIST_RECT] = GetDlgItem(IDC_EDIT_WBI_WINDOW_RECT);
    m_vEditList[WBI_LIST_CLIENT_RECT] = GetDlgItem(IDC_EDIT_WBI_CLIENT_RECT);
    m_vEditList[WBI_LIST_PARENT_INDEX] = GetDlgItem(IDC_EDIT_WBI_PARENT);
    m_vEditList[WBI_LIST_OWNER_INDEX] = GetDlgItem(IDC_EDIT_WBI_OWNER);
    m_vEditList[WBI_LIST_CHILD_INDEX] = GetDlgItem(IDC_EDIT_WBI_CHILD);
    m_vEditList[WBI_LIST_HWNDPREV_INDEX] = GetDlgItem(IDC_EDIT_WBI_HWNDPREV);
    m_vEditList[WBI_LIST_HWNDNEXT_INDEX] = GetDlgItem(IDC_EDIT_WBI_HWNDNEXT);
    m_vEditList[WBI_LIST_OPACITY_INDEX] = GetDlgItem(IDC_EDIT_WBI_OPACITY);
    m_vEditList[WBI_LIST_FOCUS_INDEX] = GetDlgItem(IDC_EDIT_WBI_FOCUS);
    m_vEditList[WBI_LIST_ACTIVE_INDEX] = GetDlgItem(IDC_EDIT_WBI_ACTIVE);
    m_vEditList[WBI_LIST_CAPTURE_INDEX] = GetDlgItem(IDC_EDIT_WBI_CAPTURE);
    m_vEditList[WBI_LIST_CARET_INDEX] = GetDlgItem(IDC_EDIT_WBI_CARET);
    m_vEditList[WBI_LIST_FOREGROUND_INDEX] = GetDlgItem(IDC_EDIT_WBI_FOREGROUND);
    m_vEditList[WBI_LIST_GLOBAL_FOCUS_INDEX] = GetDlgItem(IDC_EDIT_WBI_GLOBAL_FOCUS);
    m_vEditList[WBI_LIST_GLOBAL_ACTIVE_INDEX] = GetDlgItem(IDC_EDIT_WBI_GLOBAL_ACTIVE);
    m_vEditList[WBI_LIST_GLOBAL_CAPTURE_INDEX] = GetDlgItem(IDC_EDIT_WBI_GLOBAL_CAPTURE);
    m_vEditList[WBI_LIST_GLOBAL_CARET_INDEX] = GetDlgItem(IDC_EDIT_WBI_GLOBAL_CARET);

    m_oBtnAddStyle.SubclassWindow(GetDlgItem(IDC_BUTTON_WBI_ADD_STYLE));
    m_oBtnAddExStyle.SubclassWindow(GetDlgItem(IDC_BUTTON_WBI_ADD_EXSTYLE));

    m_oListStyle = GetDlgItem(IDC_LIST_WBI_STYLE);
    m_oListExStyle = GetDlgItem(IDC_LIST_WBI_EXSTYLE);

    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowBasicInfo::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowBasicInfo::OnClose()
{
    SetMsgHandled(FALSE);

    m_oBtnAddStyle.UnsubclassWindow();
    m_oBtnAddExStyle.UnsubclassWindow();

    DestroyWindow();

    return;
}

VOID
CDlgWindowBasicInfo::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowBasicInfo::OnBtnMoveWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    WTL::CRect oRect;
    oWatchWindow.GetWindowRect(&oRect);

    CDlgWindowGetRect oDlgGetRect;
    oDlgGetRect.SetWindowTitle(_T("设置窗口区域 - SetWindowPos"));
    oDlgGetRect.SetRect(oRect);
    if(IDOK != oDlgGetRect.DoModal()) {
        return;
    }

    oDlgGetRect.GetRect(&oRect);
    oWatchWindow.SetWindowPos(NULL, &oRect, SWP_NOZORDER);

    return;
}

VOID
CDlgWindowBasicInfo::OnBtnSetParent(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    CDlgWindowGetWindow oDlgGetWindow;
    oDlgGetWindow.SetWindowTitle(_T("设置父窗口"));
    oDlgGetWindow.SetWindowHWND(m_hParent);
    if(IDOK != oDlgGetWindow.DoModal()) {
        return;
    }

    HWND hParent = oDlgGetWindow.GetWindowHWND();
    oWatchWindow.SetParent(hParent);

    RefreshAllWindowInfo();

    return;
}

VOID
CDlgWindowBasicInfo::OnBtnSetWatchWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    HWND hNewWatchWindow = NULL;
    switch(nId) {
    case IDC_BUTTON_WBI_GOTO_PARENT:
        hNewWatchWindow = m_hParent;
        break;
    case IDC_BUTTON_WBI_GOTO_OWNER:
        hNewWatchWindow = m_hOwner;
        break;
    case IDC_BUTTON_WBI_GOTO_CHILD:
        hNewWatchWindow = m_hChild;
        break;
    case IDC_BUTTON_WBI_GOTO_HWNDPREV:
        hNewWatchWindow = m_hHwndPrev;
        break;
    case IDC_BUTTON_WBI_GOTO_HWNDNEXT:
        hNewWatchWindow = m_hHwndNext;
        break;
    case IDC_BUTTON_WBI_GOTO_FOCUS:
        hNewWatchWindow = m_oThreadInfo.hwndFocus;
        break;
    case IDC_BUTTON_WBI_GOTO_ACTIVE:
        hNewWatchWindow = m_oThreadInfo.hwndActive;
        break;
    case IDC_BUTTON_WBI_GOTO_CAPTURE:
        hNewWatchWindow = m_oThreadInfo.hwndCapture;
        break;
    case IDC_BUTTON_WBI_GOTO_CARET:
        hNewWatchWindow = m_oThreadInfo.hwndCaret;
        break;
    case IDC_BUTTON_WBI_GOTO_FOREGROUND:
        hNewWatchWindow = m_hForeground;
        break;
    case IDC_BUTTON_WBI_GOTO_GLOBAL_FOCUS:
        hNewWatchWindow = m_oForegroundThreadInfo.hwndFocus;
        break;
    case IDC_BUTTON_WBI_GOTO_GLOBAL_ACTIVE:
        hNewWatchWindow = m_oForegroundThreadInfo.hwndActive;
        break;
    case IDC_BUTTON_WBI_GOTO_GLOBAL_CAPTURE:
        hNewWatchWindow = m_oForegroundThreadInfo.hwndCapture;
        break;
    case IDC_BUTTON_WBI_GOTO_GLOBAL_CARET:
        hNewWatchWindow = m_oForegroundThreadInfo.hwndCaret;
        break;
    }

    UpdateWatchWindow(hNewWatchWindow);

    return;
}

VOID
CDlgWindowBasicInfo::OnBtnTrackWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    if(nNotifyCode != BN_CLICKED || !UpdateTrackWindow(nId)) {
        SetMsgHandled(FALSE);
        return;
    }

    return;
}

VOID
CDlgWindowBasicInfo::OnBtnRemoveStyle(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWindow = GetWatchWindow();
    if(!oWindow.IsWindow()) {
        return;
    }

    int nSelectIndex = m_oListStyle.GetCurSel();
    if(nSelectIndex < 0) {
        return;
    }

    StyleInfo *pInfo = (StyleInfo *)m_oListStyle.GetItemDataPtr(nSelectIndex);
    if(NULL == pInfo) {
        return;
    }

    DWORD nNewStyle = m_nStyle;
    nNewStyle &= ~(pInfo->m_nMask);
    nNewStyle &= ~(pInfo->m_nCode);

    oWindow.SetWindowLongPtr(GWL_STYLE, nNewStyle);

    RefreshAllWindowInfo();

    return;
}

VOID
CDlgWindowBasicInfo::OnBtnRemoveExStyle(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWindow = GetWatchWindow();
    if(!oWindow.IsWindow()) {
        return;
    }

    int nSelectIndex = m_oListExStyle.GetCurSel();
    if(nSelectIndex < 0) {
        return;
    }

    StyleInfo *pInfo = (StyleInfo *)m_oListExStyle.GetItemDataPtr(nSelectIndex);
    if(NULL == pInfo) {
        return;
    }

    DWORD nNewStyle = m_nExStyle;
    nNewStyle &= ~(pInfo->m_nMask);
    nNewStyle &= ~(pInfo->m_nCode);

    oWindow.SetWindowLongPtr(GWL_EXSTYLE, nNewStyle);

    RefreshAllWindowInfo();

    return;
}

VOID 
CDlgWindowBasicInfo::OnBtnSetLayeredWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CDlgWindowSetLayeredWindow oDlgSetLayeredWindow;
    oDlgSetLayeredWindow.SetColor(m_nLayeredColor);
    oDlgSetLayeredWindow.SetAlpha(m_nLayeredAlpha);
    oDlgSetLayeredWindow.SetFlag(m_nLayeredFlag);
    if(IDOK != oDlgSetLayeredWindow.DoModal()) {
        return;
    }

    CWindow oWindow = GetWatchWindow();
    if(!oWindow.IsWindow()) {
        return;
    }

    oWindow.ModifyStyleEx(0, WS_EX_LAYERED);

    COLORREF nLayeredColor = oDlgSetLayeredWindow.GetColor();
    BYTE nLayeredAlpha = oDlgSetLayeredWindow.GetAlpha();
    DWORD nLayeredFlag = oDlgSetLayeredWindow.GetFlag();
    ::SetLayeredWindowAttributes(oWindow, nLayeredColor, nLayeredAlpha, nLayeredFlag);

    RefreshAllWindowInfo();

    return;
}

BOOL
CDlgWindowBasicInfo::DoOperation(UINT nOpId, LPARAM lParam)
{
    UINT nCheckId = 0;

    switch(nOpId) {
    case UIOP_WBI_TRACK_FOCUS:
        nCheckId = IDC_CHECK_WBI_TRACK_FOCUS;
        break;
    case UIOP_WBI_TRACK_ACTIVE:
        nCheckId = IDC_CHECK_WBI_TRACK_ACTIVE;
        break;
    case UIOP_WBI_TRACK_CAPTURE:
        nCheckId = IDC_CHECK_WBI_TRACK_CAPTURE;
        break;
    case UIOP_WBI_TRACK_FOREGROUND:
        nCheckId = IDC_CHECK_WBI_TRACK_FOREGROUND;
        break;
    case UIOP_WBI_TRACK_GLOBAL_FOCUS:
        nCheckId = IDC_CHECK_WBI_TRACK_GLOBAL_FOCUS;
        break;
    case UIOP_WBI_TRACK_GLOBAL_ACTIVE:
        nCheckId = IDC_CHECK_WBI_TRACK_GLOBAL_ACTIVE;
        break;
    case UIOP_WBI_TRACK_GLOBAL_CAPTURE:
        nCheckId = IDC_CHECK_WBI_TRACK_GLOBAL_CAPTURE;
        break;
    }

    CButton oCheck = GetDlgItem(nCheckId);
    if(!oCheck.IsWindow()) {
        return TRUE;
    }

    oCheck.SetCheck(!oCheck.GetCheck());

    UpdateTrackWindow(nCheckId);

    return TRUE;
}

BOOL
CDlgWindowBasicInfo::ResetData(DWORD nParam)
{
    if(REFRESH_PARAM_TRACK_MODE != nParam) {
        m_nPid = 0;
        m_strExeName = _T("");
        m_nTid = 0;
        m_strThreadModule = _T("");
        m_strClassName = _T("");
        m_bIsUnicode = FALSE;
        m_nWndProc = NULL;
        m_oWindowRect.SetRectEmpty();
        m_oClientRect.SetRectEmpty();
        m_nUserData = NULL;
        m_nStyle = 0;
        m_vStyles.clear();
        m_nExStyle = 0;
        m_vExStyles.clear();
        m_hParent = NULL;
        m_hOwner = NULL;
        m_hChild = NULL;
        m_hHwndPrev = NULL;
        m_hHwndNext = NULL;
        m_nLayeredColor = 0;
        m_nLayeredAlpha = 0;
        m_nLayeredFlag = 0;
    }

    memset(&m_oThreadInfo, 0, sizeof(m_oThreadInfo));
    m_hForeground = NULL;
    memset(&m_oForegroundThreadInfo, 0, sizeof(m_oForegroundThreadInfo));

    return TRUE;
}

BOOL
CDlgWindowBasicInfo::FetchData(DWORD nParam)
{
    // 前置窗口
    m_hForeground = ::GetForegroundWindow();
    if(m_hForeground != NULL && ::IsWindow(m_hForeground)) {
        DWORD nForegroundTid = ::GetWindowThreadProcessId(m_hForeground, NULL);
        m_oForegroundThreadInfo.cbSize = sizeof(m_oForegroundThreadInfo);
        ::GetGUIThreadInfo(nForegroundTid, &m_oForegroundThreadInfo);
    }

    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return FALSE;
    }

    // 线程状态
    m_oThreadInfo.cbSize = sizeof(m_oThreadInfo);
    ::GetGUIThreadInfo(oWindow.GetWindowThreadID(), &m_oThreadInfo);

    if(REFRESH_PARAM_TRACK_MODE == nParam) {
        return TRUE;
    }

    // 进程
    m_nPid = oWindow.GetWindowProcessID();

    ProcessControl oProcess(m_nPid, PROCESS_VM_READ | PROCESS_QUERY_INFORMATION);
    oProcess.GetExeFileName(m_strExeName);

    // 线程
    m_nTid = oWindow.GetWindowThreadID();

    ThreadControl oThread(m_nTid, THREAD_QUERY_INFORMATION);
    oThread.GetThreadModule(m_strThreadModule);

    // ClassName
    TCHAR szClassName[1024] = {0};
    ::GetClassName(oWindow, szClassName, 1023);
    m_strClassName = szClassName;

    // IsUnicode
    m_bIsUnicode = oWindow.IsWindowUnicode();

    // Window Proc
    m_nWndProc = GetWindowProc(oWindow);
    if(NULL == m_nWndProc) {
        MLOG_DEBUG("WindowBasicInfo", "获取窗口过程失败。");
    }

    // UserData
    m_nUserData = oWindow.GetWindowLongPtr(GWLP_USERDATA);

    // Style
    m_nStyle = (DWORD)oWindow.GetWindowLongPtr(GWL_STYLE);
    TranslateWindowStyle(m_nStyle, m_strClassName, m_vStyles);

    // ExStyle
    m_nExStyle = (DWORD)oWindow.GetWindowLongPtr(GWL_EXSTYLE);
    TranslateWindowExStyle(m_nExStyle, m_strClassName, m_vExStyles);

    // 窗口位置
    oWindow.GetWindowRect(&m_oWindowRect);

    // 客户区域
    oWindow.GetClientRect(&m_oClientRect);

    // Parent
    m_hParent = (HWND)oWindow.GetWindowLongPtr(GWLP_HWNDPARENT);

    // Owner
    m_hOwner = oWindow.GetWindow(GW_OWNER);
    
    // Child
    m_hChild = oWindow.GetWindow(GW_CHILD);

    // Hwnd prev
    m_hHwndPrev = oWindow.GetWindow(GW_HWNDPREV);

    // Hwnd next
    m_hHwndNext = oWindow.GetWindow(GW_HWNDNEXT);

    // Layered Attribute
    ::GetLayeredWindowAttributes(oWindow, &m_nLayeredColor, &m_nLayeredAlpha, &m_nLayeredFlag);

    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateUI(DWORD nParam)
{
    UpdateWindowInfo(WBI_LIST_FOCUS_INDEX, m_oThreadInfo.hwndFocus);
    UpdateWindowInfo(WBI_LIST_ACTIVE_INDEX, m_oThreadInfo.hwndActive);
    UpdateWindowInfo(WBI_LIST_CAPTURE_INDEX, m_oThreadInfo.hwndCapture);
    UpdateWindowInfo(WBI_LIST_CARET_INDEX, m_oThreadInfo.hwndCaret);
    UpdateWindowInfo(WBI_LIST_FOREGROUND_INDEX, m_hForeground);
    UpdateWindowInfo(WBI_LIST_GLOBAL_FOCUS_INDEX, m_oForegroundThreadInfo.hwndFocus);
    UpdateWindowInfo(WBI_LIST_GLOBAL_ACTIVE_INDEX, m_oForegroundThreadInfo.hwndActive);
    UpdateWindowInfo(WBI_LIST_GLOBAL_CAPTURE_INDEX, m_oForegroundThreadInfo.hwndCapture);
    UpdateWindowInfo(WBI_LIST_GLOBAL_CARET_INDEX, m_oForegroundThreadInfo.hwndCaret);

    if(REFRESH_PARAM_TRACK_MODE == nParam) {
        return TRUE;
    }

    CString strPid;
    strPid.Format(_T("[%lu:%lu] %s / %s"), m_nPid, m_nTid, m_strExeName.GetBuffer(), m_strThreadModule);
    UpdateBasicInfo(WBI_LIST_PID_INDEX, strPid);

    UpdateRectInfo(WBI_LIST_RECT, &m_oWindowRect);

    CString strWindowProc;
    strWindowProc.Format(_T("%08x (%s)"), m_nWndProc, m_bIsUnicode ? _T("Unicode") : _T("Not Unicode"));
    UpdateBasicInfo(WBI_LIST_WINDOW_PROC_INDEX, strWindowProc);

    UpdateRectInfo(WBI_LIST_CLIENT_RECT, &m_oClientRect);

    CString strUserData;
    strUserData.Format(_T("%08x"), m_nUserData);
    UpdateBasicInfo(WBI_LIST_USER_DATA, strUserData);

    CString strStyle;
    strStyle.Format(_T("%08x"), m_nStyle);
    UpdateBasicInfo(WBI_LIST_STYLE_INDEX, strStyle);

    CString strExStyle;
    strExStyle.Format(_T("%08x"), m_nExStyle);
    UpdateBasicInfo(WBI_LIST_EXSTYLE_INDEX, strExStyle);

    UpdateWindowInfo(WBI_LIST_PARENT_INDEX, m_hParent);
    UpdateWindowInfo(WBI_LIST_OWNER_INDEX, m_hOwner);
    UpdateWindowInfo(WBI_LIST_CHILD_INDEX, m_hChild);
    UpdateWindowInfo(WBI_LIST_HWNDPREV_INDEX, m_hHwndPrev);
    UpdateWindowInfo(WBI_LIST_HWNDNEXT_INDEX, m_hHwndNext);

    CString strLayeredInfo, strLayeredAlpha, strLayeredColor;
    if(m_nLayeredFlag & LWA_ALPHA) {
        strLayeredAlpha.Format(_T("Alpha: %lu "), m_nLayeredAlpha);
    }
    if(m_nLayeredFlag & LWA_COLORKEY) {
        strLayeredColor.Format(_T("Color: %08x "), m_nLayeredColor);
    }
    strLayeredInfo = strLayeredColor + strLayeredAlpha;
    if(strLayeredInfo.GetLength() == 0) {
        strLayeredInfo = _T("无");
    }
    UpdateBasicInfo(WBI_LIST_OPACITY_INDEX, strLayeredInfo);

    // Styles
    m_oListStyle.ResetContent();
    CString strStyleName;
    size_t i = 0, nCount = m_vStyles.size();
    for(i = 0; i < nCount; ++i) {
        strStyleName.Format(_T("%s [%08x]"), m_vStyles[i].m_strName, m_vStyles[i].m_nCode);
        int nIndex = m_oListStyle.AddString(strStyleName);
        if(nIndex >= 0) {
            m_oListStyle.SetItemDataPtr(nIndex, &(m_vStyles[i]));
        }
    }

    // ExStyles
    m_oListExStyle.ResetContent();
    nCount = m_vExStyles.size();
    for(i = 0; i < nCount; ++i) {
        strStyleName.Format(_T("%s [%08x]"), m_vExStyles[i].m_strName, m_vExStyles[i].m_nCode);
        int nIndex = m_oListExStyle.AddString(strStyleName);
        if(nIndex >= 0) {
            m_oListExStyle.SetItemDataPtr(nIndex, &(m_vExStyles[i]));
        }
    }

    // TrackWindow
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_FOCUS);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_ACTIVE);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_CAPTURE);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_CARET);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_FOREGROUND);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_GLOBAL_FOCUS);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_GLOBAL_ACTIVE);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_GLOBAL_CAPTURE);
    UpdateTrackWindow(IDC_CHECK_WBI_TRACK_GLOBAL_CARET);

    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateWindowInfo(int nIndex, HWND hWindow)
{
    CString strOutput;
    OutputWindowToString(hWindow, strOutput);
    UpdateBasicInfo(nIndex, strOutput);
    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateRectInfo(int nIndex, WTL::CRect *pRect)
{
    CString strOutput;
    OutputRectToString(pRect, strOutput);
    UpdateBasicInfo(nIndex, strOutput);
    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateBasicInfo(int nIndex, CString &strInfo)
{
    if(nIndex >= WBI_LIST_NUM) {
        return FALSE;
    }

    m_vEditList[nIndex].SetWindowText(strInfo);

    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateWatchWindow(HWND hWindow)
{
    SetWatchWindowWithOffset(hWindow, 0, 0);
    RefreshAllWindowInfo();
    return TRUE;
}

BOOL
CDlgWindowBasicInfo::UpdateTrackWindow(UINT nCheckId)
{
    CString strTrackHint;
    HWND hTrackWindow = NULL;
    COLORREF nColor = 0;

    switch(nCheckId) {
    case IDC_CHECK_WBI_TRACK_FOCUS:
        strTrackHint = _T("焦点窗口");
        hTrackWindow = m_oThreadInfo.hwndFocus;
        nColor = RGB(255, 0, 0);
        break;
    case IDC_CHECK_WBI_TRACK_ACTIVE:
        strTrackHint = _T("激活窗口");
        hTrackWindow = m_oThreadInfo.hwndActive;
        nColor = RGB(255, 128, 0);
        break;
    case IDC_CHECK_WBI_TRACK_CAPTURE:
        strTrackHint = _T("鼠标捕获");
        hTrackWindow = m_oThreadInfo.hwndCapture;
        nColor = RGB(255, 255, 0);
        break;
    case IDC_CHECK_WBI_TRACK_CARET:
        strTrackHint = _T("输入窗口");
        hTrackWindow = m_oForegroundThreadInfo.hwndCaret;
        nColor = RGB(255, 128, 128);
        break;
    case IDC_CHECK_WBI_TRACK_FOREGROUND:
        strTrackHint = _T("前景窗口");
        hTrackWindow = m_hForeground;
        nColor = RGB(0, 0, 255);
        break;
    case IDC_CHECK_WBI_TRACK_GLOBAL_FOCUS:
        strTrackHint = _T("全局焦点");
        hTrackWindow = m_oForegroundThreadInfo.hwndFocus;
        nColor = RGB(0, 255, 255);
        break;
    case IDC_CHECK_WBI_TRACK_GLOBAL_ACTIVE:
        strTrackHint = _T("全局激活");
        hTrackWindow = m_oForegroundThreadInfo.hwndActive;
        nColor = RGB(0, 128, 255);
        break;
    case IDC_CHECK_WBI_TRACK_GLOBAL_CAPTURE:
        strTrackHint = _T("全局捕获");
        hTrackWindow = m_oForegroundThreadInfo.hwndCapture;
        nColor = RGB(0, 128, 0);
        break;
    case IDC_CHECK_WBI_TRACK_GLOBAL_CARET:
        strTrackHint = _T("全局输入");
        hTrackWindow = m_oForegroundThreadInfo.hwndCaret;
        nColor = RGB(128, 128, 255);
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

VOID
CDlgWindowBasicInfo::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_PARENT, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_PARENT, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_OWNER, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_OWNER, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_CHILD, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_CHILD, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_HWNDPREV, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_HWNDPREV, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_HWNDNEXT, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_HWNDNEXT, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_FOCUS, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_FOCUS, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_ACTIVE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_ACTIVE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_CAPTURE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_CAPTURE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_FOREGROUND, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_FOREGROUND, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_FOCUS, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_GLOBAL_FOCUS, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_ACTIVE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_GLOBAL_ACTIVE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_CAPTURE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_GOTO_GLOBAL_CAPTURE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_FOCUS, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_FOCUS, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_ACTIVE, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_ACTIVE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_CAPTURE, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_CAPTURE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_FOREGROUND, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_FOREGROUND, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_FOCUS, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_GLOBAL_FOCUS, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_ACTIVE, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_GLOBAL_ACTIVE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_CAPTURE, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_WBI_TRACK_GLOBAL_CAPTURE, 0 },
        { HOTKEY_SECTION_WINDOW_INFO, HOTKEY_NAME_WINDOW_INFO_SET_PARENT, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WBI_SET_PARENT, 0 },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}