#pragma once

#include "ui/ui_engine/controls/menu_button.h"

class CDwmOpMenu :
    public CMenuButton
{
public:
    enum {
        BMI_DWMWA_NCRENDERING_POLICY,           // [set] Non-client rendering policy
        BMI_DWMWA_TRANSITIONS_FORCEDISABLED,    // [set] Potentially enable/forcibly disable transitions
        BMI_DWMWA_ALLOW_NCPAINT,                // [set] Allow contents rendered in the non-client area to be visible on the DWM-drawn frame.
        BMI_DWMWA_NONCLIENT_RTL_LAYOUT,         // [set] Is non-client content RTL mirrored
        BMI_DWMWA_FORCE_ICONIC_REPRESENTATION,  // [set] Force this window to display iconic thumbnails.
        BMI_DWMWA_FLIP3D_POLICY,                // [set] Designates how Flip3D will treat the window.
        BMI_DWMWA_HAS_ICONIC_BITMAP,            // [set] Indicates an available bitmap when there is no better thumbnail representation.
        BMI_DWMWA_DISALLOW_PEEK,                // [set] Don't invoke Peek on the window.
        BMI_DWMWA_EXCLUDED_FROM_PEEK,           // [set] LivePreview exclusion information
    };

    enum {
        BMII_DWMNCRP_USEWINDOWSTYLE,
        BMII_DWMNCRP_DISABLED,
        BMII_DWMNCRP_ENABLED,
        BMII_DWMWA_TRANSITIONS_FORCEDISABLED_ON,
        BMII_DWMWA_TRANSITIONS_FORCEDISABLED_OFF,
        BMII_DWMWA_ALLOW_NCPAINT_ON,
        BMII_DWMWA_ALLOW_NCPAINT_OFF,
        BMII_DWMWA_NONCLIENT_RTL_LAYOUT_ON,
        BMII_DWMWA_NONCLIENT_RTL_LAYOUT_OFF,
        BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_ON,
        BMII_DWMWA_FORCE_ICONIC_REPRESENTATION_OFF,
        BMII_DWMFLIP3D_DEFAULT,
        BMII_DWMFLIP3D_EXCLUDEBELOW,
        BMII_DWMFLIP3D_EXCLUDEABOVE,
        BMII_DWMWA_HAS_ICONIC_BITMAP_ON,
        BMII_DWMWA_HAS_ICONIC_BITMAP_OFF,
        BMII_DWMWA_DISALLOW_PEEK_ON,
        BMII_DWMWA_DISALLOW_PEEK_OFF,
        BMII_DWMWA_EXCLUDED_FROM_PEEK_ON,
        BMII_DWMWA_EXCLUDED_FROM_PEEK_OFF,
    };

public:
    CDwmOpMenu(UINT nMenuType);
    virtual ~CDwmOpMenu();

protected:
    virtual HMENU CreateContextMenu();
    virtual BOOL OnMenuItemClick(UINT nId);

private:
    UINT m_nMenuType;
};