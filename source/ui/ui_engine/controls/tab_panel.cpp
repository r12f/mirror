#include "stdafx.h"
#include "ui/ui_engine/controls/tab_panel.h"

CTabPanel::CTabPanel()
{

}

CTabPanel::~CTabPanel()
{

}

void
CTabPanel::UpdateLayout()
{
    RECT rect;
    GetClientRect(&rect);

    if(m_tab.IsWindow() && ((m_tab.GetStyle() & WS_VISIBLE) != 0)) {
        m_tab.SetWindowPos(NULL,
            0, 0, rect.right - rect.left, rect.bottom - rect.top,
            SWP_NOZORDER | SWP_ASYNCWINDOWPOS | SWP_DEFERERASE);
    }

    if(m_nActivePage != -1) {
        ::SetWindowPos(GetPageHWND(m_nActivePage), NULL,
            2, m_cyTabHeight, rect.right - rect.left - 5, rect.bottom - rect.top - m_cyTabHeight - 3,
            SWP_NOZORDER | SWP_ASYNCWINDOWPOS | SWP_DEFERERASE);
    }
}