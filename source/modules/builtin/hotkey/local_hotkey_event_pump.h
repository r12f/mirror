#pragma once

#include <vector>
#include <map>
#include "base/com/com_class.h"
#include "modules/builtin/component_hotkey.h"

class LocalHotkeyEventPump :
    public IHotkeyEventPump
{
    typedef std::map<UINT64, CComPtr<IHotkey>> LocalHotkeyList;

public:
    LocalHotkeyEventPump();
    virtual ~LocalHotkeyEventPump();

    COM_CLASS_FACTORY(LocalHotkeyEventPump)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(LocalHotkeyEventPump)
            QI_MAPPING(IHotkeyEventPump)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(Init)();
    STDMETHOD(Uninit)();
    STDMETHOD(RegisterHotkey)(UINT nModifier, UINT nKey, IHotkey *pHotkey);
    STDMETHOD(UnregisterHotkey)(UINT nModifier, UINT nKey, IHotkey *pHotkey);

protected:
    UINT64 MakeSystemHotkeyIndex(UINT nModifier, UINT nKey);
    UINT GetKeyModifier(LPARAM lParam);
    BOOL OnKeyStroke(WPARAM wParam, LPARAM lParam);

    static LRESULT CALLBACK KeyboardProc(int code, WPARAM wParam, LPARAM lParam);

private:
    static HHOOK s_hHook;
    static LocalHotkeyEventPump *s_pThis;
    LocalHotkeyList m_vLocalHotkeys;
};