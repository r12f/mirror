#pragma once

#include "base/com/com_class.h"
#include "modules/window/utils/input/input_simulator.h"

class InputSimulatorBase :
    public IInputSimulator
{
public:
    InputSimulatorBase();
    virtual ~InputSimulatorBase();

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(InputSimulatorBase)
            QI_MAPPING(IInputSimulator)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Set Modifier需要使用InputSimulateBase的实现
    // 因为在生成WM_CHAR等事件时，使用的是当前键盘按键的状态，而不是判断之前有没有VK_SHIFT的按下消息
    STDMETHOD(SetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(UnsetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(GetCurrentModifier)(UINT *pModifier);

protected:
    BOOL SendModiferEvent(UINT nKey, UINT nEvent);

protected:
    UINT m_nModifer;
};