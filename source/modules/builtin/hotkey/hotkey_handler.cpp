#include "stdafx.h"
#include "modules/builtin/hotkey/hotkey_handler.h"

HotkeyHandler::HotkeyHandler()
    : m_hNotifyWnd(0)
    , m_nNotifyMessage(0)
    , m_nNotifyMessageWParam(0)
    , m_nNotifyMessageLParam(0)
{

}

HotkeyHandler::~HotkeyHandler()
{

}

HRESULT
HotkeyHandler::SetNotifyHwnd(HWND hNotifyWnd)
{
    ASSERT_RETURN(NULL != hNotifyWnd, E_INVALIDARG);

    m_hNotifyWnd = hNotifyWnd;

    return S_OK;
}

HRESULT
HotkeyHandler::SetNotifyMessage(DWORD nMessage)
{
    ASSERT_RETURN(0 != nMessage, E_INVALIDARG);

    m_nNotifyMessage = nMessage;

    return S_OK;
}

HRESULT
HotkeyHandler::SetNotifyMessageWParam(WPARAM wParam)
{
    m_nNotifyMessageWParam = wParam;

    return S_OK;
}

HRESULT
HotkeyHandler::SetNotifyMessageLParam(LPARAM lParam)
{
    m_nNotifyMessageLParam = lParam;

    return S_OK;
}

HRESULT
HotkeyHandler::GetNotifyHwnd(HWND *pNotifyWnd)
{
    ASSERT_RETURN(NULL != pNotifyWnd, E_INVALIDARG);

    *pNotifyWnd = m_hNotifyWnd;

    return S_OK;
}

HRESULT
HotkeyHandler::GetNotifyMessage(DWORD *pMessage)
{
    ASSERT_RETURN(NULL != pMessage, E_INVALIDARG);

    *pMessage = m_nNotifyMessage;

    return S_OK;
}

HRESULT
HotkeyHandler::GetNotifyMessageWParam(WPARAM *pParam)
{
    ASSERT_RETURN(NULL != pParam, E_INVALIDARG);

    *pParam = m_nNotifyMessageWParam;

    return S_OK;
}

HRESULT
HotkeyHandler::GetNotifyMessageLParam(LPARAM *pParam)
{
    ASSERT_RETURN(NULL != pParam, E_INVALIDARG);

    *pParam = m_nNotifyMessageLParam;

    return S_OK;
}

HRESULT
HotkeyHandler::OnHotkey(UINT nModifier, UINT nKey)
{
    if(NULL == m_hNotifyWnd || 0 == m_nNotifyMessage) {
        return E_FAIL;
    }

    ::PostMessage(m_hNotifyWnd, m_nNotifyMessage, m_nNotifyMessageWParam, m_nNotifyMessageLParam);

    return S_OK;
}