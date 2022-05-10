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

    // Set Modifier��Ҫʹ��InputSimulateBase��ʵ��
    // ��Ϊ������WM_CHAR���¼�ʱ��ʹ�õ��ǵ�ǰ���̰�����״̬���������ж�֮ǰ��û��VK_SHIFT�İ�����Ϣ
    STDMETHOD(SetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(UnsetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(GetCurrentModifier)(UINT *pModifier);

protected:
    BOOL SendModiferEvent(UINT nKey, UINT nEvent);

protected:
    UINT m_nModifer;
};