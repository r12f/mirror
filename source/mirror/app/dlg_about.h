#pragma once

class CDlgAbout :
    public CDialogImpl<CDlgAbout>
{
public:
    enum { IDD = IDD_DIALOG_ABOUT };

    CDlgAbout();
    virtual ~CDlgAbout();

    BEGIN_MSG_MAP_EX(CDlgAbout)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_ABOUT_CHECK_UPDATE, OnAboutCheckUpdate)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_ABOUT_DONATE, OnVisitWebsite)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_ABOUT_GOTO_HOME, OnVisitWebsite)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_ABOUT_GOTO_BLOG, OnVisitWebsite)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnAboutCheckUpdate(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnVisitWebsite(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
};