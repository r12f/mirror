#pragma once

#include "modules/compile/ui/dlg_pe.h"
#include "modules/compile/ui/pdb/dlg_pdb.h"
#include "ui/ui_engine/utils/simple_droptarget.h"

class CDlgCompile :
    public CDialogImpl<CDlgCompile>,
    public CLayout,
    public CUIControl,
    public CSimpleDropTarget
{
public:
    enum { IDD = IDD_DIALOG_COMPILE };

    CDlgCompile();
    virtual ~CDlgCompile();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgCompile)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        CHAIN_MSG_MAP(CSimpleDropTarget)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

private:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

    virtual VOID OnFileDropped(LPCTSTR pFilePath);
    VOID AnalyzeFile(LPCTSTR pFilePath);

private:
    std::vector<HWND> m_vAnalyses;
};