#pragma once

#include "modules/window/utils/style_translator.h"
#include "ui/ui_engine/controls/menu_button.h"

class AddStyleMenu :
    public CMenuButton
{
public:
    enum {
        ADD_STYLE_MENU_WINDOW_STYLE     = 0,
        ADD_STYLE_MENU_WINDOW_EXSTYLE,
    };

public:
    AddStyleMenu(UINT nMenuType);
    virtual ~AddStyleMenu();

    BOOL SetClassName(CString &strClassName);

protected:
    virtual HMENU CreateContextMenu();
    virtual BOOL OnMenuItemClick(UINT nId);

private:
    DWORD m_nMenuType;
    CString m_strClassName;
    StyleInfoList m_vStyles;
};