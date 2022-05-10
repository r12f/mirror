#include "stdafx.h"
#include "modules/window/utils/input/window_input_simulator.h"

static int s_vKeyMsgs[WindowInputSimulator::WINDOW_KEY_OP_NUM][4] = {
    { WM_KEYDOWN, -1 },
    { WM_KEYUP, -1 },
    { WM_CHAR, -1 },
    { WM_KEYDOWN, WM_KEYUP, -1 },
};

static int s_vMouseMsgs[WindowInputSimulator::WINDOW_MOUSE_OP_NUM][6] = {
    { 0, -1 },
    { 1, -1 },
    { 2, -1 },
    { 0, 1, -1 },
    { 0, 1, 0, 1, 2, -1 },
    { WM_MOUSEHOVER, -1 },
    { WM_MOUSELEAVE, -1 },
    { WM_MOUSEMOVE, -1 },
    { 0, -1 },
    { 1, -1 },
    { 2, -1 },
    { 0, 1, -1 },
    { 0, 1, 0, 1, 2, -1 },
    { WM_NCMOUSEHOVER, -1 },
    { WM_NCMOUSELEAVE, -1 },
    { WM_NCMOUSEMOVE, -1 },
};

WindowInputSimulator::WindowInputSimulator()
    : m_hWindow(NULL)
    , m_nMouseX(0)
    , m_nMouseY(0)
{

}

WindowInputSimulator::~WindowInputSimulator()
{

}

HRESULT
WindowInputSimulator::SetHWND(HWND hWindow)
{
    m_hWindow = hWindow;

    return S_OK;
}

HRESULT
WindowInputSimulator::GetHWND(HWND *pWindow)
{
    ASSERT_RETURN(NULL != pWindow, E_INVALIDARG);

    *pWindow = m_hWindow;

    return S_OK;
}

HRESULT
WindowInputSimulator::SetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    return InputSimulatorBase::SetModifier(nModifier, bSetBySimulate);
}

HRESULT
WindowInputSimulator::UnsetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    return InputSimulatorBase::UnsetModifier(nModifier, bSetBySimulate);
}

HRESULT
WindowInputSimulator::GetCurrentModifier(UINT *pModifier)
{
    return InputSimulatorBase::GetCurrentModifier(pModifier);
}

HRESULT
WindowInputSimulator::KeyDown(UINT nKey)
{
    return SimulateKeyboardOp(WINDOW_KEY_OP_KEYDOWN, nKey);
}

HRESULT
WindowInputSimulator::KeyUp(UINT nKey)
{
    return SimulateKeyboardOp(WINDOW_KEY_OP_KEYUP, nKey);
}

HRESULT
WindowInputSimulator::KeyChar(UINT nChar)
{
    return SimulateKeyboardOp(WINDOW_KEY_OP_KEYCHAR, nChar);
}

HRESULT
WindowInputSimulator::KeyStroke(UINT nKey)
{
    return SimulateKeyboardOp(WINDOW_KEY_OP_STROKE, nKey);
}

HRESULT
WindowInputSimulator::SetMousePos(INT nX, INT nY)
{
    m_nMouseX = nX;
    m_nMouseY = nY;

    return S_OK;
}

HRESULT
WindowInputSimulator::MouseButtonDown(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_BUTTONDOWN);
}

HRESULT
WindowInputSimulator::MouseButtonUp(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_BUTTONUP);
}

HRESULT
WindowInputSimulator::MouseButtonDoubleClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_BUTTONDBLCLICK);
}

HRESULT
WindowInputSimulator::MouseClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_CLICK);
}

HRESULT
WindowInputSimulator::MouseDoubleClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_DBLCLICK);
}

HRESULT
WindowInputSimulator::MouseHover()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_HOVER);
}

HRESULT
WindowInputSimulator::MouseLeave()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_LEAVE);
}

HRESULT
WindowInputSimulator::MouseMove()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_MOVE);
}

HRESULT
WindowInputSimulator::NCMouseButtonDown(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_NCBUTTONDOWN);
}

HRESULT
WindowInputSimulator::NCMouseButtonUp(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_NCBUTTONUP);
}

HRESULT
WindowInputSimulator::NCMouseButtonDoubleClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_NCBUTTONDBLCLICK);
}

HRESULT
WindowInputSimulator::NCMouseClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_NCCLICK);
}

HRESULT
WindowInputSimulator::NCMouseDoubleClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, WINDOW_MOUSE_OP_NCDBLCLICK);
}

HRESULT
WindowInputSimulator::NCMouseHover()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_NCHOVER);
}

HRESULT
WindowInputSimulator::NCMouseLeave()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_NCLEAVE);
}

HRESULT
WindowInputSimulator::NCMouseMove()
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, WINDOW_MOUSE_OP_NCMOVE);
}

HRESULT
WindowInputSimulator::SimulateKeyboardOp(DWORD nKeyboardOp, UINT nKey)
{
    int *pMsgList = GetKeyboardMessageList(nKeyboardOp);
    if(NULL == pMsgList) {
        return E_FAIL;
    }

    int i = 0;
    while(pMsgList[i] != -1) {
        PostKeyMessageToWindow(nKeyboardOp, pMsgList[i], nKey);
        ++i;
    }

    return S_OK;
}

int *
WindowInputSimulator::GetKeyboardMessageList(DWORD nKeyboardOp)
{
    ASSERT_RETURN(nKeyboardOp < WINDOW_KEY_OP_NUM, NULL);
    return s_vKeyMsgs[nKeyboardOp];
}

WPARAM
WindowInputSimulator::GetKeyMessageWParam(DWORD nMsg, UINT nKey)
{
    return nKey;
}

LPARAM
WindowInputSimulator::GetKeyMessageLParam(DWORD nMsg, UINT nKey)
{
    unsigned short nFlag = 0;
    UINT nScanKey = ::MapVirtualKey(nKey, 0);
    unsigned short nRepeatCount = 1;

    switch(nMsg) {
    case WM_KEYUP:
        nFlag |= 0xC000;
        break;
    }

    return ((int)nFlag << 16) | (nScanKey << 16) | nRepeatCount;
}

BOOL
WindowInputSimulator::PostKeyMessageToWindow(DWORD nKeyType, DWORD nMsg, UINT nKey)
{
    if(NULL == m_hWindow) {
        return FALSE;
    }

    DWORD nRealMsg = nMsg;
    if((m_nModifer & MOD_ALT) != 0) {
        switch(nMsg) {
        case WM_KEYDOWN:
            nRealMsg = WM_SYSKEYDOWN;
            break;
        case WM_KEYUP:
            nRealMsg = WM_SYSKEYUP;
            break;
        case WM_CHAR:
            nRealMsg = WM_SYSCHAR;
            break;
        }
    }

    WPARAM wParam = GetKeyMessageWParam(nMsg, nKey);
    LPARAM lParam = GetKeyMessageLParam(nMsg, nKey);

    if(!::PostMessage(m_hWindow, nMsg, wParam, lParam)) {
        return FALSE;
    }

    ::Sleep(50);

    return TRUE;
}

HRESULT
WindowInputSimulator::SimulateMouseOp(DWORD nMouseButtonType, DWORD nMouseOp)
{
    DWORD nMsgBase = GetMouseMessageBase(nMouseButtonType, nMouseOp);
    int *pMsgList = GetMouseMessageFixList(nMouseOp);
    if(NULL == pMsgList || (DWORD)(-1) == nMsgBase) {
        return E_FAIL;
    }

    int i = 0;
    while(pMsgList[i] != -1) {
        PostMessageToWindow(nMouseButtonType, nMsgBase + pMsgList[i]);
        ++i;
    }

    return S_OK;
}

int *
WindowInputSimulator::GetMouseMessageFixList(DWORD nMouseOp)
{
    ASSERT_RETURN(nMouseOp < WINDOW_MOUSE_OP_NUM, NULL);
    return s_vMouseMsgs[nMouseOp];
}

DWORD
WindowInputSimulator::GetMouseMessageBase(DWORD nMouseButtonType, DWORD nMouseOp)
{
    BOOL bIsNCOp = FALSE;
    switch(nMouseOp) {
    case WINDOW_MOUSE_OP_HOVER:
    case WINDOW_MOUSE_OP_LEAVE:
    case WINDOW_MOUSE_OP_MOVE:
    case WINDOW_MOUSE_OP_NCHOVER:
    case WINDOW_MOUSE_OP_NCLEAVE:
    case WINDOW_MOUSE_OP_NCMOVE:
        return 0;
    }

    switch(nMouseButtonType) {
    case ISF_MOUSE_LBUTTON:
        return !bIsNCOp ? WM_LBUTTONDOWN : WM_NCLBUTTONDOWN;
    case ISF_MOUSE_MBUTTON:
        return !bIsNCOp ? WM_MBUTTONDOWN : WM_NCMBUTTONDOWN;
    case ISF_MOUSE_RBUTTON:
        return !bIsNCOp ? WM_RBUTTONDOWN : WM_NCRBUTTONDOWN;
    case ISF_MOUSE_XBUTTON1:
        return !bIsNCOp ? WM_XBUTTONDOWN : WM_NCXBUTTONDOWN;
    case ISF_MOUSE_XBUTTON2:
        return !bIsNCOp ? WM_XBUTTONDOWN : WM_NCXBUTTONDOWN;
    }

    return (DWORD)-1;
}

DWORD
WindowInputSimulator::GetMouseMessageWParam(DWORD nMouseButtonType, DWORD nMsg)
{
    switch(nMouseButtonType) {
    case ISF_MOUSE_LBUTTON:
        if(nMsg == WM_LBUTTONDOWN || nMsg == WM_NCLBUTTONDOWN) {
            return MK_LBUTTON;
        }
        break;
    case ISF_MOUSE_MBUTTON:
        if(nMsg == WM_MBUTTONDOWN || nMsg == WM_NCMBUTTONDOWN) {
            return MK_MBUTTON;
        }
        break;
    case ISF_MOUSE_RBUTTON:
        if(nMsg == WM_RBUTTONDOWN || nMsg == WM_NCRBUTTONDOWN) {
            return MK_RBUTTON;
        }
        break;
    case ISF_MOUSE_XBUTTON1:
        if(nMsg == WM_XBUTTONDOWN || nMsg == WM_NCXBUTTONDOWN) {
            return MK_XBUTTON1;
        }
        break;
    case ISF_MOUSE_XBUTTON2:
        if(nMsg == WM_XBUTTONDOWN || nMsg == WM_NCXBUTTONDOWN) {
            return MK_XBUTTON2;
        }
        break;
    }

    return 0;
}

DWORD
WindowInputSimulator::GetMouseMessageLParam(DWORD nMouseButtonType, DWORD nMsg, INT nMouseX, INT nMouseY)
{
    return ((nMsg == WM_MOUSELEAVE || nMsg == WM_NCMOUSELEAVE) ? 0 : MAKELPARAM(nMouseX, nMouseY));
}

BOOL
WindowInputSimulator::PostMessageToWindow(DWORD nMouseButtonType, DWORD nMsg)
{
    if(NULL == m_hWindow) {
        return FALSE;
    }

    WPARAM wParam = GetMouseMessageWParam(nMouseButtonType, nMsg);
    LPARAM lParam = GetMouseMessageLParam(nMouseButtonType, nMsg, m_nMouseX, m_nMouseY);

    DWORD nRet = (DWORD)::PostMessage(m_hWindow, nMsg, wParam, lParam);
    DWORD nErrCode = ::GetLastError();

    MLOG_INFO("Window", "发送消息结束：Message = %lu(0x%x)，wParam = %lu(0x%x)，lParam = %lu(0x%x)，Ret = %lu，ErrCode = %lu。",
        nMsg, nMsg, wParam, wParam, lParam, lParam, nRet, nErrCode);

    return (BOOL)nRet;
}
