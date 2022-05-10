#include "stdafx.h"
#include "modules/window/app/properties/dwm_op_menu.h"
#include "modules/window/app/window_app_utils.h"
#include "modules/window/utils/dwmapi_adapter.h"

struct DWMMenuItem {
    UINT m_nType;
    UINT m_nId;
    TCHAR *m_strTitle;
    DWORD m_nAttrId;
    ULONGLONG m_nAttrValue;
    DWORD m_nAttrValueSize;
};

static DWMMenuItem s_vDwmOpMenuItems[] = {
    { CDwmOpMenu::BMI_DWMWA_NCRENDERING_POLICY, CDwmOpMenu::BMII_DWMNCRP_USEWINDOWSTYLE, _T("DWMNCRP_USEWINDOWSTYLE"), DWMWA_NCRENDERING_POLICY, DWMNCRP_USEWINDOWSTYLE, sizeof(DWMNCRENDERINGPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_NCRENDERING_POLICY, CDwmOpMenu::BMII_DWMNCRP_DISABLED, _T("DWMNCRP_DISABLED"), DWMWA_NCRENDERING_POLICY, DWMNCRP_DISABLED, sizeof(DWMNCRENDERINGPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_NCRENDERING_POLICY, CDwmOpMenu::BMII_DWMNCRP_ENABLED, _T("DWMNCRP_ENABLED"), DWMWA_NCRENDERING_POLICY, DWMNCRP_ENABLED, sizeof(DWMNCRENDERINGPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_TRANSITIONS_FORCEDISABLED, CDwmOpMenu::BMII_DWMWA_TRANSITIONS_FORCEDISABLED_ON, _T("TRUE"), DWMWA_TRANSITIONS_FORCEDISABLED, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_TRANSITIONS_FORCEDISABLED, CDwmOpMenu::BMII_DWMWA_TRANSITIONS_FORCEDISABLED_OFF, _T("FALSE"), DWMWA_TRANSITIONS_FORCEDISABLED, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_ALLOW_NCPAINT, CDwmOpMenu::BMII_DWMWA_ALLOW_NCPAINT_ON, _T("TRUE"), DWMWA_ALLOW_NCPAINT, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_ALLOW_NCPAINT, CDwmOpMenu::BMII_DWMWA_ALLOW_NCPAINT_OFF, _T("FALSE"), DWMWA_ALLOW_NCPAINT, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_NONCLIENT_RTL_LAYOUT, CDwmOpMenu::BMII_DWMWA_NONCLIENT_RTL_LAYOUT_ON, _T("TRUE"), DWMWA_NONCLIENT_RTL_LAYOUT, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_NONCLIENT_RTL_LAYOUT, CDwmOpMenu::BMII_DWMWA_NONCLIENT_RTL_LAYOUT_OFF, _T("FALSE"), DWMWA_NONCLIENT_RTL_LAYOUT, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_FORCE_ICONIC_REPRESENTATION, CDwmOpMenu::BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_ON, _T("TRUE"), DWMWA_FORCE_ICONIC_REPRESENTATION, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_FORCE_ICONIC_REPRESENTATION, CDwmOpMenu::BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_OFF, _T("FALSE"), DWMWA_FORCE_ICONIC_REPRESENTATION, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_FLIP3D_POLICY, CDwmOpMenu::BMII_DWMFLIP3D_DEFAULT, _T("DWMFLIP3D_DEFAULT"), DWMWA_FLIP3D_POLICY, DWMFLIP3D_DEFAULT, sizeof(DWMFLIP3DWINDOWPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_FLIP3D_POLICY, CDwmOpMenu::BMII_DWMFLIP3D_EXCLUDEBELOW, _T("DWMFLIP3D_EXCLUDEBELOW"), DWMWA_FLIP3D_POLICY, DWMFLIP3D_EXCLUDEBELOW, sizeof(DWMFLIP3DWINDOWPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_FLIP3D_POLICY, CDwmOpMenu::BMII_DWMFLIP3D_EXCLUDEABOVE, _T("DWMFLIP3D_EXCLUDEABOVE"), DWMWA_FLIP3D_POLICY, DWMFLIP3D_EXCLUDEABOVE, sizeof(DWMFLIP3DWINDOWPOLICY) },
    { CDwmOpMenu::BMI_DWMWA_HAS_ICONIC_BITMAP, CDwmOpMenu::BMII_DWMWA_HAS_ICONIC_BITMAP_ON, _T("TRUE"), DWMWA_HAS_ICONIC_BITMAP, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_HAS_ICONIC_BITMAP, CDwmOpMenu::BMII_DWMWA_HAS_ICONIC_BITMAP_OFF, _T("FALSE"), DWMWA_HAS_ICONIC_BITMAP, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_DISALLOW_PEEK, CDwmOpMenu::BMII_DWMWA_DISALLOW_PEEK_ON, _T("TRUE"), DWMWA_DISALLOW_PEEK, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_DISALLOW_PEEK, CDwmOpMenu::BMII_DWMWA_DISALLOW_PEEK_OFF, _T("FALSE"), DWMWA_DISALLOW_PEEK, FALSE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_EXCLUDED_FROM_PEEK, CDwmOpMenu::BMII_DWMWA_EXCLUDED_FROM_PEEK_ON, _T("TRUE"), DWMWA_EXCLUDED_FROM_PEEK, TRUE, sizeof(BOOL) },
    { CDwmOpMenu::BMI_DWMWA_EXCLUDED_FROM_PEEK, CDwmOpMenu::BMII_DWMWA_EXCLUDED_FROM_PEEK_OFF, _T("FALSE"), DWMWA_EXCLUDED_FROM_PEEK, FALSE, sizeof(BOOL) },
};

static DWORD s_nDwmOpMenuItemCount = sizeof(s_vDwmOpMenuItems) / sizeof(DWMMenuItem);

CDwmOpMenu::CDwmOpMenu(UINT nMenuType)
    : CMenuButton(TRUE)
    , m_nMenuType(nMenuType)
{

}

CDwmOpMenu::~CDwmOpMenu()
{

}

HMENU
CDwmOpMenu::CreateContextMenu()
{
    HMENU hMenu = ::CreatePopupMenu();
    if(NULL == hMenu) {
        return NULL;
    }

    DWORD i = 0;
    for(i = 0; i < s_nDwmOpMenuItemCount; ++i) {
        if(s_vDwmOpMenuItems[i].m_nType == m_nMenuType) {
            ::AppendMenu(hMenu, MF_STRING | MF_ENABLED | MF_BYCOMMAND, s_vDwmOpMenuItems[i].m_nId, s_vDwmOpMenuItems[i].m_strTitle);
        }
    }

    return hMenu;
}

BOOL
CDwmOpMenu::OnMenuItemClick(UINT nId)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return FALSE;
    }

    DWORD i = 0;
    for(i = 0; i < s_nDwmOpMenuItemCount; ++i) {
        if(s_vDwmOpMenuItems[i].m_nId == nId) {
            AdaDwmSetWindowAttribute(oWindow.m_hWnd, s_vDwmOpMenuItems[i].m_nAttrId, &(s_vDwmOpMenuItems[i].m_nAttrValue), s_vDwmOpMenuItems[i].m_nAttrValueSize);
            return TRUE;
        }
    }

    RefreshAllWindowInfo();

    return FALSE;
}