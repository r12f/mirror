#pragma once

#include "ui/layout.h"

class CLayout
{
public:
    CLayout(DWORD nResourceId);
    virtual ~CLayout();

    BOOL AddChildBox(ILayoutBox *pBox);
    BOOL RemoveChildBox(ILayoutBox *pBox);

    virtual BOOL DoCustomLayout() { return FALSE; }
    virtual HWND GetNativeWindowFromResId(LPCSTR strId) { return NULL; }

protected:
    BOOL Layout(BOOL bForce);

public:
    // Message Chain
    BEGIN_MSG_MAP(CLayout)
        if(uMsg == WM_INITDIALOG) {
            bHandled = TRUE;
            lResult = OnInitDialog(hWnd, uMsg, wParam, lParam, bHandled);
            if(bHandled)
                return TRUE;
        }
        MESSAGE_HANDLER(WM_NCDESTROY, OnNCDestroy)
        MESSAGE_HANDLER(WM_SHOWWINDOW, OnShowWindow)
        MESSAGE_HANDLER(WM_SIZE, OnSize)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnNCDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

private:
    DWORD m_nResourceId;
    HWND m_hWindow;
    CComPtr<ILayoutBox> m_pRootLayoutBox;
};
