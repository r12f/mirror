#pragma once

#include <vector>

// Style translate functions
struct StyleInfo {
    CString m_strName;
    DWORD m_nCode;
    DWORD m_nMask;
};

typedef std::vector<StyleInfo> StyleInfoList;

BOOL TranslateWindowStyle(DWORD nStyle, CString &strClassName, StyleInfoList &vStyleNames);
BOOL GetAllWindowStyles(CString &strClassName, StyleInfoList &vStyleNames);

BOOL TranslateWindowExStyle(DWORD nExStyle, CString &strClassName, StyleInfoList &vExStyleNames);
BOOL GetAllWindowExStyles(CString &strClassName, StyleInfoList &vExStyleNames);

BOOL TranslateClassStyle(DWORD nStyle, StyleInfoList &vStyleNames);
BOOL GetAllClassStyles(StyleInfoList &vStyleNames);