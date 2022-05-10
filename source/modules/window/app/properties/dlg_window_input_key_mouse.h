#pragma once

#include <map>
#include "base/kv/kv.h"
#include "modules/window/utils/input/input_simulator.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"

class CDlgWindowInputKeyMouse :
    public CDialogImpl<CDlgWindowInputKeyMouse>,
    public CLayout,
    public CUIControl
{
    typedef std::map<CString, DWORD> WMList;

public:
    enum { IDD = IDD_DIALOG_WINDOW_INPUT_KEYMOUSE };

    enum {
        MSG_LIST_WM_KEYDOWN     = 0,
        MSG_LIST_WM_KEYUP,
        MSG_LIST_WM_CHAR,
        MSG_LIST_WM_CLICK,
        MSG_LIST_INPUT,
        MSG_LIST_NUM,
    };

    CDlgWindowInputKeyMouse();
    virtual ~CDlgWindowInputKeyMouse();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowInputKeyMouse)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDOWN, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSEUP, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSEDBLCLK, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDOWN, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEUP, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEDBLCLK, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_CLICK, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_MOUSE_MSG_DBLCLK, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSEHOVER, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSELEAVE, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSEMOVE, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEHOVER, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSELEAVE, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_NCMOUSEMOVE, OnBtnMouseMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_CLICK, OnBtnMouseRealInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_MOUSE_REAL_DBLCLK, OnBtnMouseRealInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_KEYDOWN, OnBtnKeyMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_KEYUP, OnBtnKeyMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_CHAR, OnBtnKeyMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_CLICK, OnBtnKeyMsgInput)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_INPUT_WM_INPUT, OnBtnKeyRealInput)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnBtnKeyMsgInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnKeyRealInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnMouseMsgInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnMouseRealInput(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    BOOL BeginInput(IInputSimulator *pInput);
    BOOL EndInput(IInputSimulator *pInput);

    BOOL BeginSingleKeyInput(IInputSimulator *pInput, UINT nId, TCHAR nChar, TCHAR &nInputKey);
    BOOL EndSingleKeyInput(IInputSimulator *pInput, UINT nId, TCHAR nChar);

    BOOL GetMouseInputParam(HWND &hWindow, INT &nX, INT &nY, DWORD &nMouseButtonType);

    VOID ProvideHotkey();

private:
    CButton m_oCheckMouseButton[ISF_MOUSE_BUTTONNUM];
    CEdit m_oEditInputString;
    CButton m_oCheckCtrlKeyState;
    CButton m_oCheckAltKeyState;
    CButton m_oCheckShiftKeyState;
};