#pragma once

#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "modules/window/app/properties/dwm_op_menu.h"

class CDlgWindowDwmOp :
    public CDialogImpl<CDlgWindowDwmOp>,
    public CLayout,
    public CUIControl
{
    enum {
        WDS_LIST_DWMWA_NCRENDERING_ENABLED_INDEX    = 0,
        WDS_LIST_DWMWA_CAPTION_BUTTON_BOUNDS_INDEX,
        WDS_LIST_DWMWA_EXTENDED_FRAME_BOUNDS_INDEX,
    };
public:
    enum { IDD = IDD_DIALOG_WINDOW_DWM_OP };

    CDlgWindowDwmOp();
    virtual ~CDlgWindowDwmOp();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowDwmOp)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_DWM_EXTEND_FRAME, OnBtnDwmExtendFrame)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_DWM_INVALIDATE_ICONIC_BITMAPS, OnBtnDwmInvalidateIconicBitmaps)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnBtnDwmExtendFrame(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnBtnDwmInvalidateIconicBitmaps(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

    VOID ProvideHotkey();

private:
    CDwmOpMenu m_oBtnNCRenderPolicy;
    CDwmOpMenu m_oBtnTransitionDisabled;
    CDwmOpMenu m_oBtnAllowNCPaint;
    CDwmOpMenu m_oBtnNCRtlLayout;
    CDwmOpMenu m_oBtnForceIconicRpst;
    CDwmOpMenu m_oBtnFlip3DPolicy;
    CDwmOpMenu m_oBtnHasIconicBitmap;
    CDwmOpMenu m_oBtnDisallowPeek;
    CDwmOpMenu m_oBtnExcluedFromPeek;
    CListViewCtrl m_oListDwmStatus;
    BOOL m_bNCRenderingEnabled;
    WTL::CRect m_oCaptionButtonBounds;
    WTL::CRect m_oExtendedFrameBounds;
};