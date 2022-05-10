#pragma once

#include "ui/ui_engine/layout/layout.h"

class CDlgFloatEdit :
    public CDialogImpl<CDlgFloatEdit>,
    public CMessageFilter,
    public CLayout
{
public:
    enum { IDD = IDD_DIALOG_FLOAT_EDIT };

    CDlgFloatEdit();
    virtual ~CDlgFloatEdit();

    virtual BOOL PreTranslateMessage(MSG* pMsg);

    BEGIN_MSG_MAP_EX(CDlgFloatEdit)
        CHAIN_MSG_MAP(CLayout)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_COMMAND(OnCommand)
        MSG_WM_KILLFOCUS(OnKillFocus)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnCommand(UINT nNotify, UINT nId, HWND hWndCtrl);
    VOID OnKillFocus(HWND hFocus);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

    virtual void OnFinalMessage(HWND);

protected:
    virtual BOOL DoCustomLayout();

private:
    CEdit m_oEditInput;
};