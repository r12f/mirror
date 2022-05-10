#pragma once

#include <map>
#include "base/kv/kv.h"
#include "base/com/com_class.h"
#include "ui/ui_engine/controls/menu_button.h"
#include <dwmapi.h>

class CDlgWindowGetRect:
    public CDialogImpl<CDlgWindowGetRect>
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_GET_RECT };

    CDlgWindowGetRect();
    virtual ~CDlgWindowGetRect();

    VOID SetWindowTitle(LPCTSTR strTitle);
    VOID SetRect(RECT oRect);
    VOID GetRect(RECT *pRect);
    VOID SetMargin(MARGINS oMargin);
    VOID GetMargin(MARGINS *pMargin);

    BEGIN_MSG_MAP_EX(CDlgWindowGetRect)
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
    CStatic m_vStaLable[4];
    CEdit m_vEditInputOffset[4];
    BOOL m_bIsGetRectDialog;
    int m_vOffset[4];
};