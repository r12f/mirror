#pragma once

#include <vector>
#include <map>
#include "base/com/com_class.h"
#include "modules/builtin/component_hotkey.h"
#include "modules/builtin/component_config.h"

class HotkeyManager :
    public IHotkeyManager,
    public IHotkeyEventPumpManager
{
    typedef std::vector<CComPtr<IHotkey>> HotkeyList;
    typedef std::vector<CComPtr<IHotkeyNotifyHandler>> HotkeyNotifyList;
    
public:
    HotkeyManager();
    virtual ~HotkeyManager();

    COM_CLASS_FACTORY(HotkeyManager)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HotkeyManager)
            QI_MAPPING(IHotkeyManager)
            QI_MAPPING(IHotkeyEventPumpManager)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // IHotkeyManager
    STDMETHOD(RegisterHotkey)(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey **ppHotkey);
    STDMETHOD(UnregisterHotkey)(BSTR strSection, BSTR strName, IHotkey **ppHotkey);
    STDMETHOD(GetHotkey)(BSTR strSection, BSTR strName, IHotkey **ppHotkey);
    STDMETHOD(GetHotkeyCount)(DWORD *pCount);
    STDMETHOD(GetAllHotkey)(IHotkey **ppHotkey, DWORD nMaxCount);
    STDMETHOD(SetHotkeyHandler)(BSTR strSection, BSTR strName, HWND hNotifyWnd, DWORD nNotifyMessage, WPARAM nNotifyMessageWParam, LPARAM nNotifyMessageLParam);
    STDMETHOD(SetHotkeyHandler)(BSTR strSection, BSTR strName, IHotkeyHandler *pHandler);
    STDMETHOD(GetHotkeyHandler)(BSTR strSection, BSTR strName, IHotkeyHandler **ppHandler);
    STDMETHOD(AddHotkeyNotifyHandler)(IHotkeyNotifyHandler *pHandler);
    STDMETHOD(RemoveHotkeyNotifyHandler)(IHotkeyNotifyHandler *pHandler);

    // Override IHotkeyEventPumpManager
    STDMETHOD(GetEventPump)(DWORD nType, IHotkeyEventPump **pPump);
    STDMETHOD(OnHotkeyRegistered)(BSTR strSection, BSTR strName, UINT nModifiers, UINT nKey, BOOL bGlobal, IHotkey *pHotkey);
    STDMETHOD(OnHotkeyUnregistered)(BSTR strSection, BSTR strName, IHotkey *pHotkey);

protected:
    HotkeyList::iterator GetHotkeyIt(BSTR strSection, BSTR strName);

private:
    HotkeyList m_vHotkeys;
    CComPtr<IHotkeyEventPump> m_pGlobalPump;
    CComPtr<IHotkeyEventPump> m_pLocalPump;
    CComPtr<IConfig> m_pConfig;
    HotkeyNotifyList m_vNotifyList;
};