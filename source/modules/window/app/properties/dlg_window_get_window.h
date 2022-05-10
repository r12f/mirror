#pragma once

#include <map>
#include "base/kv/kv.h"
#include "base/com/com_class.h"
#include "ui/ui_engine/controls/menu_button.h"

class CDlgWindowGetWindow:
    public CDialogImpl<CDlgWindowGetWindow>
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_GET_WINDOW };

    CDlgWindowGetWindow();
    virtual ~CDlgWindowGetWindow();

    VOID SetWindowTitle(LPCTSTR strTitle);
    VOID SetWindowHWND(HWND hWindow);
    HWND GetWindowHWND();

    BEGIN_MSG_MAP_EX(CDlgWindowGetWindow)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

private:
    CString m_strWindowTitle;
    HWND m_hWindow;
    CEdit m_oEditInputWindow;
};