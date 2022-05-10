#pragma once

#include "third_party/LibPE/Include/LibPE.h"
using namespace LibPE;

class CDlgPEBasic :
    public CDialogImpl<CDlgPEBasic>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PE_BASIC};

    CDlgPEBasic();
    virtual ~CDlgPEBasic();

    VOID SetPEFile(IPEFile *pPEFile) { m_pPEFile = pPEFile; }

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgPEBasic)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

private:
    LibPEPtr<IPEFile> m_pPEFile;
    CTreeListViewCtrl m_oPEBreakdown;
};