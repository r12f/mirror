#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/menu_button.h"
#include <dwmapi.h>

class CDlgWindowSetLayeredWindow:
    public CDialogImpl<CDlgWindowSetLayeredWindow>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_SET_LAYERED_WINDOW };

    CDlgWindowSetLayeredWindow();
    virtual ~CDlgWindowSetLayeredWindow();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    VOID SetColor(COLORREF nColor);
    COLORREF GetColor();

    VOID SetAlpha(BYTE nAlpha);
    BYTE GetAlpha();

    VOID SetFlag(DWORD nFlag);
    DWORD GetFlag();

    BEGIN_MSG_MAP_EX(CDlgWindowSetLayeredWindow)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
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
    CEdit m_oEditColorR;
    CEdit m_oEditColorG;
    CEdit m_oEditColorB;
    CEdit m_oEditAlpha;
    CButton m_oCheckUseColor;
    CButton m_oCheckUseAlpha;

    COLORREF m_nColor;
    BYTE m_nAlpha;
    DWORD m_nFlag;
};