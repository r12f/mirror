#include "stdafx.h"
#include "modules/window/utils/input/real_input_simulator.h"

static int s_vKeyInputs[RealInputSimulator::REAL_KEY_OP_NUM][4] = {
    { 0, KEYEVENTF_KEYUP, -1 }
};

static int s_vMouseInputs[RealInputSimulator::REAL_MOUSE_OP_NUM][6] = {
    { MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, -1 },
    { 0, 1, -1 },
    { 0, 1, 0, 1, -1 },
};

RealInputSimulator::RealInputSimulator()
{

}

RealInputSimulator::~RealInputSimulator()
{

}

HRESULT
RealInputSimulator::SetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    return InputSimulatorBase::SetModifier(nModifier, bSetBySimulate);
}

HRESULT
RealInputSimulator::UnsetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    return InputSimulatorBase::UnsetModifier(nModifier, bSetBySimulate);
}

HRESULT
RealInputSimulator::GetCurrentModifier(UINT *pModifier)
{
    return InputSimulatorBase::GetCurrentModifier(pModifier);
}

HRESULT
RealInputSimulator::KeyStroke(UINT nKey)
{
    return SimulateKeyboardOp(REAL_KEY_OP_STROKE, nKey);
}

HRESULT
RealInputSimulator::MouseClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, REAL_MOUSE_OP_CLICK);
}

HRESULT
RealInputSimulator::MouseDoubleClick(DWORD nMouseButtonType)
{
    return SimulateMouseOp(nMouseButtonType, REAL_MOUSE_OP_DBLCLICK);
}


HRESULT
RealInputSimulator::MouseMove(INT nX, INT nY)
{
    return SimulateMouseOp(ISF_MOUSE_NOBUTTON, REAL_MOUSE_OP_MOVE, nX, nY);
}

HRESULT
RealInputSimulator::SimulateKeyboardOp(DWORD nKeyboardOp, UINT nKey)
{
    int *pMsgList = GetKeyInputList(nKeyboardOp);
    if(NULL == pMsgList) {
        return E_FAIL;
    }

    int i = 0;
    while(pMsgList[i] != -1) {
        SendKeyInput(pMsgList[i], nKey);
        ++i;
    }

    return S_OK;
}

int *
RealInputSimulator::GetKeyInputList(DWORD nKeyboardOp)
{
    ASSERT_RETURN(nKeyboardOp < REAL_KEY_OP_NUM, NULL);

    return s_vKeyInputs[nKeyboardOp];
}

BOOL
RealInputSimulator::SendKeyInput(DWORD nInput, UINT nKey)
{
    INPUT oInput = {0};
    oInput.type = INPUT_KEYBOARD;
    oInput.ki.time = 0;
    oInput.ki.dwFlags = nInput;
    oInput.ki.wVk = ::VkKeyScan(nKey);
    return ::SendInput(1, &oInput, sizeof(INPUT));
}

HRESULT
RealInputSimulator::SimulateMouseOp(DWORD nMouseButtonType, DWORD nMouseOp, DWORD nX, DWORD nY)
{
    int *pMsgList = GetMouseInputFixList(nMouseOp);
    DWORD nInputBase = GetMouseInputBase(nMouseButtonType, nMouseOp);
    if(NULL == pMsgList || (DWORD)(-1) == nInputBase) {
        return E_FAIL;
    }

    int i = 0;
    DWORD nInput = 0;
    while(pMsgList[i] != -1) {
        if(nInputBase == 0) {
            nInput = pMsgList[i];
        } else {
            nInput = nInputBase << pMsgList[i];
        }

        SendMouseInput(nInput, nX, nY);

        ++i;
    }

    return S_OK;
}

int *
RealInputSimulator::GetMouseInputFixList(DWORD nMouseOp)
{
    ASSERT_RETURN(nMouseOp < REAL_MOUSE_OP_NUM, NULL);

    return s_vMouseInputs[nMouseOp];
}

DWORD
RealInputSimulator::GetMouseInputBase(DWORD nMouseButtonType, DWORD nMouseOp)
{
    switch(nMouseButtonType) {
    case ISF_MOUSE_NOBUTTON:
        return 0;
    case ISF_MOUSE_LBUTTON:
        return MOUSEEVENTF_LEFTDOWN;
    case ISF_MOUSE_MBUTTON:
        return MOUSEEVENTF_MIDDLEDOWN;
    case ISF_MOUSE_RBUTTON:
        return MOUSEEVENTF_RIGHTDOWN;
    case ISF_MOUSE_XBUTTON1:
        return MOUSEEVENTF_XDOWN;
    case ISF_MOUSE_XBUTTON2:
        return MOUSEEVENTF_XDOWN;
    }

    return (DWORD)(-1);
}

BOOL
RealInputSimulator::SendMouseInput(DWORD nInput, DWORD nX, DWORD nY)
{
    INPUT oInput = {0};
    oInput.type = INPUT_MOUSE;
    oInput.mi.time = 0;
    oInput.mi.dwFlags = nInput;
    oInput.mi.dx = ((nX << 16) - 1) / ::GetSystemMetrics(SM_CXSCREEN);
    oInput.mi.dy = ((nY << 16) - 1) / ::GetSystemMetrics(SM_CYSCREEN);
    return ::SendInput(1, &oInput, sizeof(INPUT));
}