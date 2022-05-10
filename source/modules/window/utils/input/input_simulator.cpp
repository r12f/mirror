#include "stdafx.h"
#include "modules/window/utils/input/input_simulator.h"
#include "modules/window/utils/input/window_input_simulator.h"
#include "modules/window/utils/input/real_input_simulator.h"

CComPtr<IWindowInputSimulator>
CreateWindowInputSimulator(HWND hWindow)
{
    CComPtr<IWindowInputSimulator> pInput;
    WindowInputSimulator::CreateObject(__uuidof(IWindowInputSimulator), (void **)&pInput);
    if(NULL == pInput) {
        return NULL;
    }

    pInput->SetHWND(hWindow);

    return pInput;
}

CComPtr<IRealInputSimulator>
CreateRealInputSimulator()
{
    CComPtr<IRealInputSimulator> pInput;
    RealInputSimulator::CreateObject(__uuidof(IRealInputSimulator), (void **)&pInput);
    if(NULL == pInput) {
        return NULL;
    }

    return pInput;
}