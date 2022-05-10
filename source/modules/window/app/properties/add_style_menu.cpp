#include "stdafx.h"
#include "modules/window/app/properties/add_style_menu.h"
#include "modules/window/app/window_app_utils.h"

AddStyleMenu::AddStyleMenu(UINT nMenuType)
    : CMenuButton(TRUE)
    , m_nMenuType(nMenuType)
{
    
}

AddStyleMenu::~AddStyleMenu()
{

}

BOOL
AddStyleMenu::SetClassName(CString &strClassName)
{
    m_strClassName = strClassName;
    return TRUE;
}

HMENU
AddStyleMenu::CreateContextMenu()
{
    m_vStyles.clear();

    switch(m_nMenuType) {
    case ADD_STYLE_MENU_WINDOW_STYLE:
        GetAllWindowStyles(m_strClassName, m_vStyles);
        break;
    case ADD_STYLE_MENU_WINDOW_EXSTYLE:
        GetAllWindowExStyles(m_strClassName, m_vStyles);
        break;
    }

    HMENU hMenu = ::CreatePopupMenu();
    if(NULL == hMenu) {
        return NULL;
    }

    CString strStyleName;
    size_t i = 0, nCount = m_vStyles.size();
    for(i = 0; i < nCount; ++i) {
        strStyleName.Format(_T("%s [%08x]"), m_vStyles[i].m_strName, m_vStyles[i].m_nCode);
        MENUITEMINFO oItem = {0};
        oItem.cbSize = sizeof(MENUITEMINFO);
        oItem.fMask = MIIM_ID | MIIM_STRING | MIIM_DATA;
        oItem.wID = (UINT)i;
        oItem.dwItemData = i;
        oItem.dwTypeData = strStyleName.GetBuffer();
        ::InsertMenuItem(hMenu, (UINT)i, FALSE, &oItem);
    }

    return hMenu;
}

BOOL
AddStyleMenu::OnMenuItemClick(UINT nId)
{
    size_t i = 0, nCount = m_vStyles.size();
    if(nId >= nCount) {
        return FALSE;
    }

    CWindow oWatchWindow = GetWatchWindow();
    if(!oWatchWindow.IsWindow()) {
        return FALSE;
    }

    int nGWLIndex = 0;
    switch(m_nMenuType) {
    case ADD_STYLE_MENU_WINDOW_STYLE:
        nGWLIndex = GWL_STYLE;
        break;
    case ADD_STYLE_MENU_WINDOW_EXSTYLE:
        nGWLIndex = GWL_EXSTYLE;
        break;
    }

    DWORD nStyle = oWatchWindow.GetWindowLong(nGWLIndex);
    nStyle &= ~(m_vStyles[nId].m_nMask);
    nStyle |= m_vStyles[nId].m_nCode;

    oWatchWindow.SetWindowLong(nGWLIndex, nStyle);

    RefreshAllWindowInfo();

    return TRUE;
}