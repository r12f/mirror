#include "stdafx.h"
#include "modules/window/app/window_tree_ctrl.h"
#include "modules/component_id.h"
#include "modules/builtin/component_config.h"
#include "modules/framework.h"

CWindowTreeCtrl::CWindowTreeCtrl()
{

}

CWindowTreeCtrl::~CWindowTreeCtrl()
{

}

VOID
CWindowTreeCtrl::BeforeContextMenuPopup(CMenuHandle &oMenu)
{
    BOOL bAutoRefresh = FALSE;
    UINT nAutoRefreshInterval = 0;
    BOOL bAutoExpand = FALSE;
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL != pConfig) {
        pConfig->GetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_LIST_AUTO_REFRESH), &bAutoRefresh);
        pConfig->GetUInt(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_LIST_AUTO_REFRESH_INTERVAL), &nAutoRefreshInterval);
        pConfig->GetBool(CComBSTR(CONFIG_SECTION_WINDOW), CComBSTR(CONFIG_KEY_WINDOW_LIST_AUTO_EXPAND), &bAutoExpand);
    }

    // 自动刷新
    oMenu.CheckMenuItem(ID_WINDOW_TREE_NO_AUTO_REFRESH, MF_BYCOMMAND | (!bAutoRefresh ? MF_CHECKED : MF_UNCHECKED));
    if(bAutoRefresh) {
        switch(nAutoRefreshInterval) {
        case 1000:
            oMenu.CheckMenuItem(ID_WINDOW_TREE_AUTO_REFRESH_PER_SECOND, MF_BYCOMMAND | MF_CHECKED);
            break;
        case 3000:
            oMenu.CheckMenuItem(ID_WINDOW_TREE_AUTO_REFRESH_PER_THREE_SECONDS, MF_BYCOMMAND | MF_CHECKED);
            break;
        case 5000:
            oMenu.CheckMenuItem(ID_WINDOW_TREE_AUTO_REFRESH_PER_FIVE_SECONDS, MF_BYCOMMAND | MF_CHECKED);
            break;
        }
    }

    // 自动展开
    oMenu.CheckMenuItem(ID_WINDOW_TREE_AUTO_EXPAND, MF_BYCOMMAND | (bAutoExpand ? MF_CHECKED : MF_UNCHECKED));

    return;
}

VOID
CWindowTreeCtrl::AfterContextMenuPopup(CMenuHandle &oMenu)
{
    return;
}