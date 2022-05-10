#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_input.h"
#include "modules/builtin/component_config.h"
#include "modules/component_id.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindowInput::CDlgWindowInput()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT, this);
}

CDlgWindowInput::~CDlgWindowInput()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT);
}

LRESULT
CDlgWindowInput::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    ProvideHotkey();

    return TRUE;
}

VOID
CDlgWindowInput::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowInput::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowInput::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
} 

HWND
CDlgWindowInput::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;
    if(0 == strResId.CompareNoCase("IDC_TAB_WINDOW_INPUT")) {
        m_oTabInput.Create(m_hWnd);

        m_oDlgInputKeyMouse.Create(m_oTabInput.m_tab);
        m_oTabInput.AddPage(m_oDlgInputKeyMouse, _T("鼠标键盘"));

        //m_oDlgInputBatch.Create(m_oTabInput.m_tab);
        //m_oTabInput.AddPage(m_oDlgInputBatch, _T("批量输入"));

        m_oTabInput.SetActivePage(0);

        return m_oTabInput;
    }

    return NULL;
}

BOOL
CDlgWindowInput::DoOperation(UINT nOpId, LPARAM lParam)
{
    switch(nOpId) {
    case UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND:
        SwitchToMe(COMPONENT_UI_DLG_WINDOW_PROPERTIES);
        SwitchToTabByHWND(m_oTabInput, (HWND)lParam);
        break;
    }
    return TRUE;
}

VOID
CDlgWindowInput::ProvideHotkey()
{
    static HotkeyHandlerData s_vHandlers[] = {
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT_KEYMOUSE, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgInputKeyMouse.m_hWnd },
        { HOTKEY_SECTION_BASIC_SWITCH_TO_TAB, HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT_BATCH, m_hWnd, WM_UI_CONTROL_DO_OPERATION, (WPARAM)UIOP_COMMMON_SWITCH_TO_TAB_BY_HWND, (LPARAM)m_oDlgInputBatch.m_hWnd },
    };

    static DWORD s_nHandlerNum = sizeof(s_vHandlers) / sizeof(HotkeyHandlerData);

    ProvideHotkeyHandler(s_vHandlers, s_nHandlerNum);

    return;
}