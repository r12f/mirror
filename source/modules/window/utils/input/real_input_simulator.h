#pragma once

#include "base/com/com_class.h"
#include "modules/window/utils/input/input_simulator_base.h"

class RealInputSimulator :
    public InputSimulatorBase,
    public IRealInputSimulator
{
public:
    enum {
        REAL_KEY_OP_STROKE              = 0,
        REAL_KEY_OP_NUM,
    };

    enum {
        REAL_MOUSE_OP_MOVE              = 0,
        REAL_MOUSE_OP_CLICK,
        REAL_MOUSE_OP_DBLCLICK,
        REAL_MOUSE_OP_NUM,
    };

public:
    RealInputSimulator();
    virtual ~RealInputSimulator();

    COM_CLASS_FACTORY(RealInputSimulator)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(InputSimulatorBase)
        BEGIN_QI_MAP(RealInputSimulator)
            QI_MAPPING(IRealInputSimulator)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(SetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(UnsetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(GetCurrentModifier)(UINT *pModifier);

    // Keyboard actions
    STDMETHOD(KeyStroke)(UINT nKey);

    // Mouse actions
    STDMETHOD(MouseClick)(DWORD nMouseButtonType);
    STDMETHOD(MouseDoubleClick)(DWORD nMouseButtonType);
    STDMETHOD(MouseMove)(INT nX, INT nY);

protected:
    // Keyboard
    HRESULT SimulateKeyboardOp(DWORD nKeyboardOp, UINT nKey);
    int * GetKeyInputList(DWORD nKeyboardOp);
    BOOL SendKeyInput(DWORD nInput, UINT nKey);

    // Mouse
    HRESULT SimulateMouseOp(DWORD nMouseButtonType, DWORD nMouseOp, DWORD nX = 0, DWORD nY = 0);
    int * GetMouseInputFixList(DWORD nMouseOp);
    DWORD GetMouseInputBase(DWORD nMouseButtonType, DWORD nMouseOp);
    BOOL SendMouseInput(DWORD nInput, DWORD nX, DWORD nY);
};