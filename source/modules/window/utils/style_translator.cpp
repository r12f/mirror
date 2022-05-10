#include "stdafx.h"
#include "modules/window/utils/style_translator.h"

struct StyleTranslateInfo {
    LPCTSTR m_pName;
    DWORD m_nCode;
    DWORD m_nMask;
};

#define DEFINE_STYLE_LIST_BEGIN(l)                              \
    static StyleTranslateInfo l[] = {                           \

#define DEFINE_STYLE(style)                                     \
        { _T(#style), style, style },

#define DEFINE_STYLE_WITH_MASK(style, mask)                     \
        { _T(#style), style, mask },

#define DEFINE_STYLE_LIST_END(l, num)                           \
    };                                                          \
                                                                \
    static DWORD num = sizeof(l) / sizeof(StyleTranslateInfo);

DEFINE_STYLE_LIST_BEGIN(s_vStyles)
    DEFINE_STYLE_WITH_MASK(WS_OVERLAPPED, 0xC0000000)
    DEFINE_STYLE(WS_POPUP)
    DEFINE_STYLE(WS_CHILD)
    DEFINE_STYLE(WS_MINIMIZE)
    DEFINE_STYLE(WS_VISIBLE)
    DEFINE_STYLE(WS_DISABLED)
    DEFINE_STYLE(WS_CLIPSIBLINGS)
    DEFINE_STYLE(WS_CLIPCHILDREN)
    DEFINE_STYLE(WS_MAXIMIZE)
    DEFINE_STYLE(WS_CAPTION)
    DEFINE_STYLE(WS_BORDER)
    DEFINE_STYLE(WS_DLGFRAME)
    DEFINE_STYLE(WS_VSCROLL)
    DEFINE_STYLE(WS_HSCROLL)
    DEFINE_STYLE(WS_SYSMENU)
    DEFINE_STYLE(WS_THICKFRAME)
    DEFINE_STYLE(WS_GROUP)
    DEFINE_STYLE(WS_TABSTOP)
    DEFINE_STYLE(WS_MINIMIZEBOX)
    DEFINE_STYLE(WS_MAXIMIZEBOX)
    DEFINE_STYLE_WITH_MASK(WS_TILED, 0xC0000000)
    DEFINE_STYLE(WS_ICONIC)
    DEFINE_STYLE(WS_SIZEBOX)
    DEFINE_STYLE_WITH_MASK(WS_TILEDWINDOW, 0xC0000000 | WS_TILEDWINDOW)
    DEFINE_STYLE_WITH_MASK(WS_OVERLAPPEDWINDOW, 0xC0000000 | WS_OVERLAPPEDWINDOW)
    DEFINE_STYLE(WS_POPUPWINDOW)
    DEFINE_STYLE(WS_CHILDWINDOW)
DEFINE_STYLE_LIST_END(s_vStyles, s_nStyleNum)

DEFINE_STYLE_LIST_BEGIN(s_vExStyles)
    DEFINE_STYLE(WS_EX_DLGMODALFRAME)
    DEFINE_STYLE(WS_EX_NOPARENTNOTIFY)
    DEFINE_STYLE(WS_EX_TOPMOST)
    DEFINE_STYLE(WS_EX_ACCEPTFILES)
    DEFINE_STYLE(WS_EX_TRANSPARENT)
    DEFINE_STYLE(WS_EX_MDICHILD)
    DEFINE_STYLE(WS_EX_TOOLWINDOW)
    DEFINE_STYLE(WS_EX_WINDOWEDGE)
    DEFINE_STYLE(WS_EX_CLIENTEDGE)
    DEFINE_STYLE(WS_EX_CONTEXTHELP)
    DEFINE_STYLE(WS_EX_RIGHT)
    DEFINE_STYLE(WS_EX_LEFT)
    DEFINE_STYLE(WS_EX_RTLREADING)
    DEFINE_STYLE(WS_EX_LTRREADING)
    DEFINE_STYLE(WS_EX_LEFTSCROLLBAR)
    DEFINE_STYLE(WS_EX_RIGHTSCROLLBAR)
    DEFINE_STYLE(WS_EX_CONTROLPARENT)
    DEFINE_STYLE(WS_EX_STATICEDGE)
    DEFINE_STYLE(WS_EX_APPWINDOW)
    DEFINE_STYLE(WS_EX_OVERLAPPEDWINDOW)
    DEFINE_STYLE(WS_EX_PALETTEWINDOW)
    DEFINE_STYLE(WS_EX_LAYERED)
    DEFINE_STYLE(WS_EX_NOINHERITLAYOUT)
    DEFINE_STYLE(WS_EX_LAYOUTRTL)
    DEFINE_STYLE(WS_EX_COMPOSITED)
    DEFINE_STYLE(WS_EX_NOACTIVATE)
DEFINE_STYLE_LIST_END(s_vExStyles, s_nExStyleNum)

DEFINE_STYLE_LIST_BEGIN(s_vClassStyles)
    DEFINE_STYLE(CS_VREDRAW)
    DEFINE_STYLE(CS_HREDRAW)
    DEFINE_STYLE(CS_DBLCLKS)
    DEFINE_STYLE(CS_OWNDC)
    DEFINE_STYLE(CS_CLASSDC)
    DEFINE_STYLE(CS_PARENTDC)
    DEFINE_STYLE(CS_NOCLOSE)
    DEFINE_STYLE(CS_SAVEBITS)
    DEFINE_STYLE(CS_BYTEALIGNCLIENT)
    DEFINE_STYLE(CS_BYTEALIGNWINDOW)
    DEFINE_STYLE(CS_GLOBALCLASS)
    DEFINE_STYLE(CS_IME)
    DEFINE_STYLE(CS_DROPSHADOW)
DEFINE_STYLE_LIST_END(s_vClassStyles, s_nClassStyleNum)

BOOL
TranslateWindowStyle(DWORD nStyle, CString &strClassName, StyleInfoList &vStyleNames)
{
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nStyleNum; ++i) {
        if(s_vStyles[i].m_nCode == 0) {
            nTestStyle = nStyle & s_vStyles[i].m_nMask;
        } else {
            nTestStyle = (nStyle & s_vStyles[i].m_nMask) & s_vStyles[i].m_nCode;
        }

        if(nTestStyle == s_vStyles[i].m_nCode) {
            StyleInfo oInfo;
            oInfo.m_strName = s_vStyles[i].m_pName;
            oInfo.m_nCode = s_vStyles[i].m_nCode;
            oInfo.m_nMask = s_vStyles[i].m_nMask;
            vStyleNames.push_back(oInfo);
        }
    }

    return TRUE;
}

BOOL
GetAllWindowStyles(CString &strClassName, StyleInfoList &vStyleNames)
{
    StyleInfo oInfo;
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nStyleNum; ++i) {
        oInfo.m_strName = s_vStyles[i].m_pName;
        oInfo.m_nCode = s_vStyles[i].m_nCode;
        oInfo.m_nMask = s_vStyles[i].m_nMask;
        vStyleNames.push_back(oInfo);
    }

    return TRUE;
}

BOOL
TranslateWindowExStyle(DWORD nExStyle, CString &strClassName, StyleInfoList &vExStyleNames)
{
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nExStyleNum; ++i) {
        if(s_vExStyles[i].m_nCode == 0) {
            nTestStyle = nExStyle & s_vExStyles[i].m_nMask;
        } else {
            nTestStyle = (nExStyle & s_vExStyles[i].m_nMask) & s_vExStyles[i].m_nCode;
        }

        if(nTestStyle == s_vExStyles[i].m_nCode) {
            StyleInfo oInfo;
            oInfo.m_strName = s_vExStyles[i].m_pName;
            oInfo.m_nCode = s_vExStyles[i].m_nCode;
            oInfo.m_nMask = s_vExStyles[i].m_nMask;
            vExStyleNames.push_back(oInfo);
        }
    }

    return TRUE;
}

BOOL
GetAllWindowExStyles(CString &strClassName, StyleInfoList &vExStyleNames)
{
    StyleInfo oInfo;
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nExStyleNum; ++i) {
        oInfo.m_strName = s_vExStyles[i].m_pName;
        oInfo.m_nCode = s_vExStyles[i].m_nCode;
        oInfo.m_nMask = s_vExStyles[i].m_nMask;
        vExStyleNames.push_back(oInfo);
    }

    return TRUE;
}

BOOL
TranslateClassStyle(DWORD nStyle, StyleInfoList &vStyleNames)
{
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nClassStyleNum; ++i) {
        if(s_vClassStyles[i].m_nCode == 0) {
            nTestStyle = nStyle & s_vClassStyles[i].m_nMask;
        } else {
            nTestStyle = (nStyle & s_vClassStyles[i].m_nMask) & s_vClassStyles[i].m_nCode;
        }

        if(nTestStyle == s_vClassStyles[i].m_nCode) {
            StyleInfo oInfo;
            oInfo.m_strName = s_vClassStyles[i].m_pName;
            oInfo.m_nCode = s_vClassStyles[i].m_nCode;
            oInfo.m_nMask = s_vClassStyles[i].m_nMask;
            vStyleNames.push_back(oInfo);
        }
    }

    return TRUE;
}

BOOL
GetAllClassStyles(StyleInfoList &vStyleNames)
{
    StyleInfo oInfo;
    DWORD i = 0, nTestStyle = 0;
    for(i = 0; i < s_nClassStyleNum; ++i) {
        oInfo.m_strName = s_vClassStyles[i].m_pName;
        oInfo.m_nCode = s_vClassStyles[i].m_nCode;
        oInfo.m_nMask = s_vClassStyles[i].m_nMask;
        vStyleNames.push_back(oInfo);
    }

    return TRUE;
}