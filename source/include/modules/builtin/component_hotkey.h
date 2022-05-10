#pragma once

#include "modules/builtin/component_hotkey_id.h"

enum {
    HOTKEY_EVENT_PUMP_LOCAL     = 0,
    HOTKEY_EVENT_PUMP_GLOBAL,
};

__interface IHotkeyHandler;
__interface IHotkey;
__interface IHotkeyEventPump;
__interface IHotkeyEventPumpManager;
__interface IHotkeyManager;

MIDL_INTERFACE("164B571F-1DE6-414f-9154-86750BC0B6F1")
IHotkeyHandler : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetNotifyHwnd(HWND hNotifyWnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetNotifyMessage(DWORD nMessage) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetNotifyMessageWParam(WPARAM wParam) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetNotifyMessageLParam(LPARAM lParam) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetNotifyHwnd(HWND *pNotifyWnd) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetNotifyMessage(DWORD *pMessage) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetNotifyMessageWParam(WPARAM *pParam) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetNotifyMessageLParam(LPARAM *pParam) = 0;

    virtual HRESULT STDMETHODCALLTYPE OnHotkey(UINT nModifier, UINT nKey) = 0;
};

MIDL_INTERFACE("8BD0DC50-1F42-4763-8414-C1238B62E129")
IHotkey : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetSection(BSTR strSection) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetName(BSTR strName) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetModifier(UINT nModifier) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetKey(UINT nKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetHandler(IHotkeyHandler *pHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetGlobal(BOOL bGlobal) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetEventPumpManager(IHotkeyEventPumpManager *pPump) = 0;

    virtual HRESULT STDMETHODCALLTYPE GetSection(BSTR *pSection) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetName(BSTR *pName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModifier(UINT *pModifier) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetKey(UINT *pKey) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHandler(IHotkeyHandler **ppHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetGlobal(BOOL *pGlobal) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEventPumpManager(IHotkeyEventPumpManager **ppPump) = 0;

    virtual HRESULT STDMETHODCALLTYPE RegisterHotkey() = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterHotkey() = 0;

    virtual HRESULT STDMETHODCALLTYPE OnHotkey(UINT nModifier, UINT nKey) = 0;
};

MIDL_INTERFACE("53BFD598-1800-4df6-A2CC-F295D78A8462")
IHotkeyEventPump : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetType(DWORD *pType) = 0;
    virtual HRESULT STDMETHODCALLTYPE Init() = 0;
    virtual HRESULT STDMETHODCALLTYPE Uninit() = 0;
    virtual HRESULT STDMETHODCALLTYPE RegisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterHotkey(UINT nModifier, UINT nKey, IHotkey *pHotkey) = 0;
};

MIDL_INTERFACE("1A5D72AC-E52F-4b34-B347-DC9636BD084A")
IHotkeyEventPumpManager : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetEventPump(DWORD nType, IHotkeyEventPump **pPump) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnHotkeyRegistered(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnHotkeyUnregistered(BSTR strSection, BSTR strName, IHotkey *pHotkey) = 0;
};

MIDL_INTERFACE("D35CD81B-A542-4906-9907-EDE6EFAF240F")
IHotkeyNotifyHandler : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE OnRegister(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE OnUnregister(BSTR strSection, BSTR strName, IHotkey *pHotkey) = 0;
};

MIDL_INTERFACE("29370465-312D-4244-824A-F77566C49D34")
IHotkeyManager : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE RegisterHotkey(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey **ppHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE UnregisterHotkey(BSTR strSection, BSTR strName, IHotkey **ppHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHotkey(BSTR strSection, BSTR strName, IHotkey **ppHotkey) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHotkeyCount(DWORD *pCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetAllHotkey(IHotkey **ppHotkey, DWORD nMaxCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetHotkeyHandler(BSTR strSection, BSTR strName, HWND hNotifyWnd, DWORD nNotifyMessage, WPARAM nNotifyMessageWParam, LPARAM nNotifyMessageLParam) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetHotkeyHandler(BSTR strSection, BSTR strName, IHotkeyHandler *pHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetHotkeyHandler(BSTR strSection, BSTR strName, IHotkeyHandler **ppHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE AddHotkeyNotifyHandler(IHotkeyNotifyHandler *pHandler) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveHotkeyNotifyHandler(IHotkeyNotifyHandler *pHandler) = 0;
};
