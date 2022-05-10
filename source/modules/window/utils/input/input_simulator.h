#pragma once

// Input simulate flags
enum {
    ISF_MOUSE_NOBUTTON      = 0,
    ISF_MOUSE_LBUTTON,
    ISF_MOUSE_MBUTTON,
    ISF_MOUSE_RBUTTON,
    ISF_MOUSE_XBUTTON1,
    ISF_MOUSE_XBUTTON2,
    ISF_MOUSE_BUTTONNUM,
};

MIDL_INTERFACE("A1DA252B-950C-4a28-A698-0C7E4D7F8A06")
IInputSimulator : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetModifier(UINT nModifier, BOOL bSetBySimulate) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnsetModifier(UINT nModifier, BOOL bSetBySimulate) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentModifier(UINT *pModifier) = 0;
};

MIDL_INTERFACE("E70F47F1-0072-4b49-855F-7C134E7DDA51")
IWindowInputSimulator : IInputSimulator
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetHWND(HWND hWindow) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHWND(HWND *pWindow) = 0;

    // Keyboard actions
    virtual HRESULT STDMETHODCALLTYPE KeyDown(UINT nKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE KeyUp(UINT nKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE KeyChar(UINT nChar) = 0;
    virtual HRESULT STDMETHODCALLTYPE KeyStroke(UINT nKey) = 0;

    // Mouse actions
    virtual HRESULT STDMETHODCALLTYPE SetMousePos(INT nX, INT nY) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseButtonDown(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseButtonUp(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseButtonDoubleClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseDoubleClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseHover() = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseLeave() = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseMove() = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseButtonDown(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseButtonUp(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseButtonDoubleClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseDoubleClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseHover() = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseLeave() = 0;
    virtual HRESULT STDMETHODCALLTYPE NCMouseMove() = 0;
};

MIDL_INTERFACE("8BE43B78-CE0A-400a-801C-615672A96FC0")
IRealInputSimulator : IInputSimulator
{
    // Keyboard actions
    virtual HRESULT STDMETHODCALLTYPE KeyStroke(UINT nKey) = 0;

    // Mouse actions
    virtual HRESULT STDMETHODCALLTYPE MouseClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseDoubleClick(DWORD nMouseButtonType) = 0;
    virtual HRESULT STDMETHODCALLTYPE MouseMove(INT nX, INT nY) = 0;
};

CComPtr<IWindowInputSimulator> CreateWindowInputSimulator(HWND hWindow);
CComPtr<IRealInputSimulator> CreateRealInputSimulator();