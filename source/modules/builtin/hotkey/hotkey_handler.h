#pragma once

#include "base/com/com_class.h"
#include "modules/builtin/component_hotkey.h"

class HotkeyHandler :
    public IHotkeyHandler
{
public:
    HotkeyHandler();
    virtual ~HotkeyHandler();

    COM_CLASS_FACTORY(HotkeyHandler)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HotkeyHandler)
            QI_MAPPING(IHotkeyHandler)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(SetNotifyHwnd)(HWND hNotifyWnd);
    STDMETHOD(SetNotifyMessage)(DWORD nMessage);
    STDMETHOD(SetNotifyMessageWParam)(WPARAM wParam);
    STDMETHOD(SetNotifyMessageLParam)(LPARAM lParam);

    STDMETHOD(GetNotifyHwnd)(HWND *pNotifyWnd);
    STDMETHOD(GetNotifyMessage)(DWORD *pMessage);
    STDMETHOD(GetNotifyMessageWParam)(WPARAM *pParam);
    STDMETHOD(GetNotifyMessageLParam)(LPARAM *pParam);

    STDMETHOD(OnHotkey)(UINT nModifier, UINT nKey);

private:
    HWND m_hNotifyWnd;
    UINT m_nNotifyMessage;
    WPARAM m_nNotifyMessageWParam;
    LPARAM m_nNotifyMessageLParam;
};