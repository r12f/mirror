#pragma once

#include "base/com/com_class.h"
#include "modules/window/utils/input/input_simulator_base.h"

class WindowInputSimulator :
    public InputSimulatorBase,
    public IWindowInputSimulator
{
public:
    enum {
        WINDOW_KEY_OP_KEYDOWN               = 0,
        WINDOW_KEY_OP_KEYUP,
        WINDOW_KEY_OP_KEYCHAR,
        WINDOW_KEY_OP_STROKE,
        WINDOW_KEY_OP_NUM,
    };

    enum {
        WINDOW_MOUSE_OP_BUTTONDOWN          = 0,
        WINDOW_MOUSE_OP_BUTTONUP,
        WINDOW_MOUSE_OP_BUTTONDBLCLICK,
        WINDOW_MOUSE_OP_CLICK,
        WINDOW_MOUSE_OP_DBLCLICK,
        WINDOW_MOUSE_OP_HOVER,
        WINDOW_MOUSE_OP_LEAVE,
        WINDOW_MOUSE_OP_MOVE,
        WINDOW_MOUSE_OP_NCBUTTONDOWN,
        WINDOW_MOUSE_OP_NCBUTTONUP,
        WINDOW_MOUSE_OP_NCBUTTONDBLCLICK,
        WINDOW_MOUSE_OP_NCCLICK,
        WINDOW_MOUSE_OP_NCDBLCLICK,
        WINDOW_MOUSE_OP_NCHOVER,
        WINDOW_MOUSE_OP_NCLEAVE,
        WINDOW_MOUSE_OP_NCMOVE,
        WINDOW_MOUSE_OP_NUM,
    };

public:
    WindowInputSimulator();
    virtual ~WindowInputSimulator();

    COM_CLASS_FACTORY(WindowInputSimulator)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(InputSimulatorBase)
        BEGIN_QI_MAP(WindowInputSimulator)
            QI_MAPPING(IWindowInputSimulator)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(SetHWND)(HWND hWindow);
    STDMETHOD(GetHWND)(HWND *pWindow);

    STDMETHOD(SetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(UnsetModifier)(UINT nModifier, BOOL bSetBySimulate);
    STDMETHOD(GetCurrentModifier)(UINT *pModifier);

    // Keyboard actions
    STDMETHOD(KeyDown)(UINT nKey);
    STDMETHOD(KeyUp)(UINT nKey);
    STDMETHOD(KeyChar)(UINT nKey);
    STDMETHOD(KeyStroke)(UINT nKey);

    // Mouse actions
    STDMETHOD(SetMousePos)(INT nX, INT nY);
    STDMETHOD(MouseButtonDown)(DWORD nMouseButtonType);
    STDMETHOD(MouseButtonUp)(DWORD nMouseButtonType);
    STDMETHOD(MouseButtonDoubleClick)(DWORD nMouseButtonType);
    STDMETHOD(MouseClick)(DWORD nMouseButtonType);
    STDMETHOD(MouseDoubleClick)(DWORD nMouseButtonType);
    STDMETHOD(MouseHover)();
    STDMETHOD(MouseLeave)();
    STDMETHOD(MouseMove)();
    STDMETHOD(NCMouseButtonDown)(DWORD nMouseButtonType);
    STDMETHOD(NCMouseButtonUp)(DWORD nMouseButtonType);
    STDMETHOD(NCMouseButtonDoubleClick)(DWORD nMouseButtonType);
    STDMETHOD(NCMouseClick)(DWORD nMouseButtonType);
    STDMETHOD(NCMouseDoubleClick)(DWORD nMouseButtonType);
    STDMETHOD(NCMouseHover)();
    STDMETHOD(NCMouseLeave)();
    STDMETHOD(NCMouseMove)();

protected:
    // Key
    HRESULT SimulateKeyboardOp(DWORD nKeyboardOp, UINT nKey);
    int * GetKeyboardMessageList(DWORD nKeyboardOp);
    WPARAM GetKeyMessageWParam(DWORD nMsg, UINT nKey);
    LPARAM GetKeyMessageLParam(DWORD nMsg, UINT nKey);
    BOOL PostKeyMessageToWindow(DWORD nKeyType, DWORD nMsg, UINT nKey);

    // Mouse
    HRESULT SimulateMouseOp(DWORD nMouseButtonType, DWORD nMouseOp);
    int * GetMouseMessageFixList(DWORD nMouseOp);
    DWORD GetMouseMessageBase(DWORD nMouseButtonType, DWORD nMouseOp);
    DWORD GetMouseMessageWParam(DWORD nMouseButtonType, DWORD nMsg);
    DWORD GetMouseMessageLParam(DWORD nMouseButtonType, DWORD nMsg, INT nMouseX = 0, INT nMouseY = 0);
    BOOL PostMessageToWindow(DWORD nMouseButtonType, DWORD nMsg);

private:
    HWND m_hWindow;
    INT m_nMouseX;
    INT m_nMouseY;
};