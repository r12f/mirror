#include "stdafx.h"
#include "modules/window/utils/input/input_simulator_base.h"

InputSimulatorBase::InputSimulatorBase()
    : m_nModifer(0)
{

}

InputSimulatorBase::~InputSimulatorBase()
{

}

HRESULT
InputSimulatorBase::SetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    m_nModifer |= nModifier;

    if(!bSetBySimulate) {
        return S_OK;
    }

    if(nModifier & MOD_ALT) {
        SendModiferEvent(VK_MENU, 0);
    }

    if(nModifier & MOD_CONTROL) {
        SendModiferEvent(VK_CONTROL, 0);
    }

    if(nModifier & MOD_SHIFT) {
        SendModiferEvent(VK_SHIFT, 0);
    }

    return S_OK;
}

HRESULT
InputSimulatorBase::UnsetModifier(UINT nModifier, BOOL bSetBySimulate)
{
    m_nModifer &= ~nModifier;

    if(!bSetBySimulate) {
        return S_OK;
    }

    if(nModifier & MOD_ALT) {
        SendModiferEvent(VK_MENU, KEYEVENTF_KEYUP);
    }

    if(nModifier & MOD_CONTROL) {
        SendModiferEvent(VK_CONTROL, KEYEVENTF_KEYUP);
    }

    if(nModifier & MOD_SHIFT) {
        SendModiferEvent(VK_SHIFT, KEYEVENTF_KEYUP);
    }

    return S_OK;
}

HRESULT
InputSimulatorBase::GetCurrentModifier(UINT *pModifier)
{
    ASSERT_RETURN(NULL != pModifier, E_INVALIDARG);
    *pModifier = m_nModifer;
    return S_OK;
}

BOOL
InputSimulatorBase::SendModiferEvent(UINT nKey, UINT nEvent)
{
    INPUT oInput = {0};
    oInput.type = INPUT_KEYBOARD;
    oInput.ki.time = 0;
    oInput.ki.wVk = nKey;
    oInput.ki.wScan = ::MapVirtualKey(nKey, 0);
    oInput.ki.dwFlags = nEvent;
    oInput.ki.dwExtraInfo = 0;

    ::SendInput(1, &oInput, sizeof(INPUT));

    return TRUE;
}