#pragma once

#include <vector>
#include <map>
#include "base/com/com_class.h"
#include "modules/builtin/component_hotkey.h"

class GlobalHotkeyEventPump :
    public IHotkeyEventPump,
    public CWindowImpl<GlobalHotkeyEventPump>
{
    typedef std::vector<CComPtr<IHotkey>> HotkeyList;

    struct SystemHotkeyInfo {
        INT m_nHotkeyId;
        HotkeyList m_vHotkeys;

        SystemHotkeyInfo();
        SystemHotkeyInfo(INT nHotkeyId);
        SystemHotkeyInfo(const SystemHotkeyInfo &rhs);
        HotkeyList::iterator GetHotkey(IHotkey *pHotkey);
    };
    typedef std::map<UINT64, SystemHotkeyInfo> SystemHotkeyList;

public:
    GlobalHotkeyEventPump();
    virtual ~GlobalHotkeyEventPump();

    COM_CLASS_FACTORY(GlobalHotkeyEventPump)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(GlobalHotkeyEventPump)
            QI_MAPPING(IHotkeyEventPump)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(Init)();
    STDMETHOD(Uninit)();
    STDMETHOD(RegisterHotkey)(UINT nModifier, UINT nKey, IHotkey *pHotkey);
    STDMETHOD(UnregisterHotkey)(UINT nModifier, UINT nKey, IHotkey *pHotkey);

    BEGIN_MSG_MAP_EX(GlobalHotkeyEventPump)
        MSG_WM_HOTKEY(OnHotkey)
    END_MSG_MAP()

protected:
    VOID OnHotkey(INT nId, UINT nModifier, UINT nKey);

protected:
    UINT64 MakeSystemHotkeyIndex(UINT nModifier, UINT nKey);

private:
    INT m_nNextHotkeyId;
    SystemHotkeyList m_vSysHotkeys;
};