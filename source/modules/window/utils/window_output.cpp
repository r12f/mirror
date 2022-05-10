#include "stdafx.h"
#include "modules/window/utils/window_output.h"

VOID
OutputWindowToString(HWND hWindow, CString &strOutput)
{
    CWindow oWindow(hWindow);
    if(NULL == oWindow.m_hWnd) {
        strOutput = _T("00000000 \"\"");
        return;
    }

    if(!oWindow.IsWindow()) {
        strOutput.Format(_T("%08x [Invalid Window Handle]"), hWindow);
        return;
    }

    TCHAR szClassName[1024] = {0};
    ::GetClassName(oWindow.m_hWnd, szClassName, 1023);

    CString strTitleName;
    oWindow.GetWindowText(strTitleName);

    strOutput.Format(_T("%08x \"%s\" %s"), hWindow, strTitleName, szClassName);

    return;
}

VOID
OutputRectToString(WTL::CRect *pRect, CString &strOutput)
{
    if(NULL == pRect) {
        strOutput = _T("Invalid rect.");
        return;
    }

    strOutput.Format(_T("(%d, %d) - (%d, %d) %dx%d"), pRect->left, pRect->top, pRect->right, pRect->bottom, pRect->Width(), pRect->Height());

    return;
}