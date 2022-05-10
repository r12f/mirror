#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_input_key_mouse.h"
#include "modules/builtin/component_config.h"
#include "modules/component_id.h"
#include "modules/builtin/component_hotkey_id.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/utils/input/input_simulator.h"

CDlgWindowInputKeyMouse::CDlgWindowInputKeyMouse()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT_KEY_MOUSE, this);
}

CDlgWindowInputKeyMouse::~CDlgWindowInputKeyMouse()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT_KEY_MOUSE);
}

LRESULT
CDlgWindowInputKeyMouse::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oCheckMouseButton[ISF_MOUSE_LBUTTON] = GetDlgItem(IDC_RADIO_WINDOW_INPUT_MOUSE_LBUTTON);
    m_oCheckMouseButton[ISF_MOUSE_MBUTTON] = GetDlgItem(IDC_RADIO_WINDOW_INPUT_MOUSE_MBUTTON);
    m_oCheckMouseButton[ISF_MOUSE_RBUTTON] = GetDlgItem(IDC_RADIO_WINDOW_INPUT_MOUSE_RBUTTON);
    m_oCheckMouseButton[ISF_MOUSE_XBUTTON1] = GetDlgItem(IDC_RADIO_WINDOW_INPUT_MOUSE_XBUTTON1);
    m_oCheckMouseButton[ISF_MOUSE_XBUTTON2] = GetDlgItem(IDC_RADIO_WINDOW_INPUT_MOUSE_XBUTTON2);

    m_oCheckMouseButton[ISF_MOUSE_LBUTTON].SetCheck(TRUE);

    m_oEditInputString = GetDlgItem(IDC_EDIT_WINDOW_INPUT_INPUT_STRING);

    m_oCheckCtrlKeyState = GetDlgItem(IDC_CHECK_WINDOW_INPUT_CTRL_STATE);
    m_oCheckAltKeyState = GetDlgItem(IDC_CHECK_WINDOW_INPUT_ALT_STATE);
    m_oCheckShiftKeyState = GetDlgItem(IDC_CHECK_WINDOW_INPUT_SHIFT_STATE);

    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowInputKeyMouse::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowInputKeyMouse::OnClose()
{
    SetMsgHandled(FALSE);

    DestroyWindow();

    return;
}

VOID
CDlgWindowInputKeyMouse::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgWindowInputKeyMouse::OnBtnKeyMsgInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    CString strInputString;
    m_oEditInputString.GetWindowText(strInputString);

    CComPtr<IWindowInputSimulator> pInput = CreateWindowInputSimulator(oWatchWindow);
    if(NULL == pInput) {
        return;
    }

    BeginInput(pInput);

    int nCharIndex = 0, nCharNum = strInputString.GetLength();
    for(nCharIndex = 0; nCharIndex < nCharNum; ++nCharIndex) {
        TCHAR nInputKey = 0;
        TCHAR nChar = strInputString.GetAt(nCharIndex);
        BeginSingleKeyInput(pInput, nId, nChar, nInputKey);
        switch(nId) {
        case IDC_BUTTON_WINDOW_INPUT_WM_KEYDOWN:
            pInput->KeyDown(nInputKey);
            break;
        case IDC_BUTTON_WINDOW_INPUT_WM_KEYUP:
            pInput->KeyUp(nInputKey);
            break;
        case IDC_BUTTON_WINDOW_INPUT_WM_CHAR:
            pInput->KeyChar(nInputKey);
            break;
        case IDC_BUTTON_WINDOW_INPUT_WM_CLICK:
            pInput->KeyStroke(nInputKey);
            break;
        }
        EndSingleKeyInput(pInput, nId, nChar);
    }

    EndInput(pInput);

    RefreshAllWindowInfo();

    return;
}

VOID
CDlgWindowInputKeyMouse::OnBtnKeyRealInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return;
    }

    CString strInputString;
    m_oEditInputString.GetWindowText(strInputString);

    CComPtr<IRealInputSimulator> pInput = CreateRealInputSimulator();
    if(NULL == pInput) {
        return;
    }

    BeginInput(pInput);

    int nCharIndex = 0, nCharNum = strInputString.GetLength();
    for(nCharIndex = 0; nCharIndex < nCharNum; ++nCharIndex) {
        TCHAR nInputKey = 0;
        TCHAR nChar = strInputString.GetAt(nCharIndex);
        BeginSingleKeyInput(pInput, nId, nChar, nInputKey);
        switch(nId) {
        case IDC_BUTTON_WINDOW_INPUT_WM_INPUT:
            pInput->KeyStroke(nInputKey);
            break;
        }
        EndSingleKeyInput(pInput, nId, nChar);
    }
    
    EndInput(pInput);

    RefreshAllWindowInfo();

    return;
}

VOID
CDlgWindowInputKeyMouse::OnBtnMouseMsgInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    HWND hWindow = NULL;
    INT nMouseX = 0, nMouseY = 0;
    DWORD nMouseButtonType = 0;
    if(!GetMouseInputParam(hWindow, nMouseX, nMouseY, nMouseButtonType) || NULL == hWindow) {
        return;
    }

    CComPtr<IWindowInputSimulator> pInput = CreateWindowInputSimulator(hWindow);
    if(NULL == pInput) {
        return;
    }

    BeginInput(pInput);

    pInput->SetMousePos(nMouseX, nMouseY);
    switch(nId) {
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDOWN:
        pInput->MouseButtonDown(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSEUP:
        pInput->MouseButtonUp(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDBLCLK:
        pInput->MouseButtonDoubleClick(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDOWN:
        pInput->NCMouseButtonDown(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEUP:
        pInput->NCMouseButtonDown(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDBLCLK:
        pInput->NCMouseButtonDoubleClick(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_CLICK:
        pInput->MouseClick(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_DBLCLK:
        pInput->MouseDoubleClick(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSEHOVER:
        pInput->MouseHover();
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSELEAVE:
        pInput->MouseLeave();
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSEMOVE:
        pInput->MouseMove();
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEHOVER:
        pInput->NCMouseHover();
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSELEAVE:
        pInput->NCMouseLeave();
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEMOVE:
        pInput->NCMouseMove();
        break;
    }

    EndInput(pInput);

    RefreshAllWindowInfo();

    return;
}

VOID
CDlgWindowInputKeyMouse::OnBtnMouseRealInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    HWND hWindow = NULL;
    INT nMouseX = 0, nMouseY = 0;
    DWORD nMouseButtonType = 0;
    if(!GetMouseInputParam(hWindow, nMouseX, nMouseY, nMouseButtonType) || NULL == hWindow) {
        return;
    }

    CComPtr<IRealInputSimulator> pInput = CreateRealInputSimulator();
    if(NULL == pInput) {
        return;
    }

    BeginInput(pInput);

    POINT oRealPt = { nMouseX, nMouseY };
    ::ClientToScreen(hWindow, &oRealPt);

    pInput->MouseMove(oRealPt.x, oRealPt.y);

    switch(nId) {
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_CLICK:
        pInput->MouseClick(nMouseButtonType);
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_DBLCLK:
        pInput->MouseDoubleClick(nMouseButtonType);
        break;
    }

    EndInput(pInput);

    RefreshAllWindowInfo();

    return;
}

BOOL
CDlgWindowInputKeyMouse::BeginInput(IInputSimulator *pInput)
{
    UINT nModifier = 0;
    if(m_oCheckCtrlKeyState.GetCheck()) {
        nModifier |= MOD_CONTROL;
    }

    if(m_oCheckAltKeyState.GetCheck()) {
        nModifier |= MOD_ALT;
    }

    if(m_oCheckShiftKeyState.GetCheck()) {
        nModifier |= MOD_SHIFT;
    }

    pInput->SetModifier(nModifier, TRUE);

    return TRUE;
}

BOOL
CDlgWindowInputKeyMouse::EndInput(IInputSimulator *pInput)
{
    UINT nModifier = 0;
    if(m_oCheckCtrlKeyState.GetCheck()) {
        nModifier |= MOD_CONTROL;
    }

    if(m_oCheckAltKeyState.GetCheck()) {
        nModifier |= MOD_ALT;
    }

    if(m_oCheckShiftKeyState.GetCheck()) {
        nModifier |= MOD_SHIFT;
    }

    pInput->UnsetModifier(nModifier, TRUE);

    return TRUE;
}

BOOL
CDlgWindowInputKeyMouse::BeginSingleKeyInput(IInputSimulator *pInput, UINT nId, TCHAR nChar, TCHAR &nInputKey)
{
    nInputKey = 0;

    switch(nId) {
    case IDC_BUTTON_WINDOW_INPUT_WM_KEYDOWN:
    case IDC_BUTTON_WINDOW_INPUT_WM_KEYUP:
    case IDC_BUTTON_WINDOW_INPUT_WM_CLICK:
    case IDC_BUTTON_WINDOW_INPUT_WM_INPUT:
        nInputKey = (nChar >= _T('a') && nChar <= _T('z')) ? (nChar - _T('a') + _T('A')) : nChar;
        break;
    case IDC_BUTTON_WINDOW_INPUT_WM_CHAR:
        nInputKey = nChar;
        break;
    }

    if(nChar >= _T('A') && nChar <= _T('Z')) {
        pInput->SetModifier(MOD_SHIFT, TRUE);
    }

    return TRUE;
}

BOOL
CDlgWindowInputKeyMouse::EndSingleKeyInput(IInputSimulator *pInput, UINT nId, TCHAR nChar)
{
    if(nChar >= _T('A') && nChar <= _T('Z')) {
        pInput->UnsetModifier(MOD_SHIFT, TRUE);
    }

    return TRUE;
}

BOOL
CDlgWindowInputKeyMouse::GetMouseInputParam(HWND &hWindow, INT &nX, INT &nY, DWORD &nMouseButtonType)
{
    hWindow = NULL;
    nX = 0;
    nY = 0;
    nMouseButtonType = ISF_MOUSE_NOBUTTON;

    if(!GetWatchWindowWithOffset(&hWindow, &nX, &nY)) {
        return FALSE;
    }

    DWORD i = 0;
    for(i = 0; i < ISF_MOUSE_BUTTONNUM; ++i) {
        if(m_oCheckMouseButton[i].IsWindow() && m_oCheckMouseButton[i].GetCheck()) {
            break;
        }
    }

    if(i < ISF_MOUSE_BUTTONNUM) {
        nMouseButtonType = i;
    }

    return TRUE;
}

VOID
CDlgWindowInputKeyMouse::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_DOWN, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDOWN, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_UP, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_MOUSEUP, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_DBLCLK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDBLCLK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCDOWN, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDOWN, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCUP, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEUP, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCDBLCLK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDBLCLK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_CLICK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_CLICK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_DBLCLK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_DBLCLK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_REAL_CLICK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_CLICK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_MOUSE, HOTKEY_NAME_WINDOW_INPUT_MOUSE_REAL_DBLCLK, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_DBLCLK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_KEY, HOTKEY_NAME_WINDOW_INPUT_KEY_WM_DOWN, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_KEYDOWN, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_KEY, HOTKEY_NAME_WINDOW_INPUT_KEY_WM_UP, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_KEYUP, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_KEY, HOTKEY_NAME_WINDOW_INPUT_KEY_WM_CHAR, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_CHAR, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_KEY, HOTKEY_NAME_WINDOW_INPUT_KEY_STROKE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_CLICK, 0},
        { HOTKEY_SECTION_WINDOW_INPUT_KEY, HOTKEY_NAME_WINDOW_INPUT_KEY_REAL_STROKE, m_hWnd, WM_COMMAND, (WPARAM)IDC_BUTTON_WINDOW_INPUT_WM_INPUT, 0},
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}