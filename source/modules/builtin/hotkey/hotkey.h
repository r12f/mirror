#pragma once

#include "base/com/com_class.h"
#include "modules/builtin/component_hotkey.h"
#include "modules/builtin/component_config.h"

class HotkeyManager;

class Hotkey :
    public IHotkey
{
    friend class HotkeyManager;

public:
    Hotkey();
    virtual ~Hotkey();

    COM_CLASS_FACTORY(Hotkey)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(Hotkey)
            QI_MAPPING(IHotkey)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(SetSection)(BSTR strSection);
    STDMETHOD(SetName)(BSTR strName);
    STDMETHOD(SetModifier)(UINT nModifier);
    STDMETHOD(SetKey)(UINT nKey);
    STDMETHOD(SetHandler)(IHotkeyHandler *pHandler);
    STDMETHOD(SetGlobal)(BOOL bGlobal);
    STDMETHOD(SetEventPumpManager)(IHotkeyEventPumpManager *pPumpMgr);

    STDMETHOD(GetSection)(BSTR *pSection);
    STDMETHOD(GetName)(BSTR *pName);
    STDMETHOD(GetModifier)(UINT *pModifier);
    STDMETHOD(GetKey)(UINT *pKey);
    STDMETHOD(GetHandler)(IHotkeyHandler **ppHandler);
    STDMETHOD(GetGlobal)(BOOL *pGlobal);
    STDMETHOD(GetEventPumpManager)(IHotkeyEventPumpManager **ppPumpMgr);

    STDMETHOD(RegisterHotkey)();
    STDMETHOD(UnregisterHotkey)();

    STDMETHOD(OnHotkey)(UINT nModifier, UINT nKey);

public:
    BOOL FixHotkeyByConfig();
    BOOL ParseHotkeyFromConfig(IConfig *pConfig, BSTR strHotkeyName, UINT &nModifier, UINT &nKey, BOOL &bGlobal);
    BOOL SaveHotkeyToConfig();

private:
    CComBSTR m_strSection;
    CComBSTR m_strName;
    UINT m_nModifier;
    UINT m_nKey;
    BOOL m_bGlobal;
    BOOL m_bHasChanged;
    CComPtr<IHotkeyHandler> m_pHandler;
    CComPtr<IHotkeyEventPump> m_pEventPump;
    CComPtr<IHotkeyEventPumpManager> m_pEventPumpMgr;
};