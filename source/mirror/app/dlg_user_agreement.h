#pragma once

class CDlgUserAgreement :
    public CDialogImpl<CDlgUserAgreement>
{
public:
    enum { IDD = IDD_DIALOG_USER_AGREEMENT };

    CDlgUserAgreement();
    virtual ~CDlgUserAgreement();

    BOOL IsAgreed();

    BEGIN_MSG_MAP_EX(CDlgUserAgreement)
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
    BOOL m_bIsAgreed;
};