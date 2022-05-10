#pragma once

#include "modules/window/utils/dwmapi_adapter.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"

class CDlgWindowDwmTiming :
    public CDialogImpl<CDlgWindowDwmTiming>,
    public CLayout,
    public CUIControl,
    public CMessageFilter
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_DWM_TIMING };

    CDlgWindowDwmTiming();
    virtual ~CDlgWindowDwmTiming();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowDwmTiming)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
    virtual void OnFinalMessage(HWND);

protected:
    virtual BOOL DoCustomLayout();
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

private:
    CStatic *m_pDwmLabels;
    CStatic *m_pDwmTimingInfos;
    CFont m_oFont;
    CToolTipCtrl m_oTooltip;

    DWM_TIMING_INFO m_oDwmTimingInfo;
};