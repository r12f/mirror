#pragma once

#include "modules/compile/pdb_utils.h"

class CDlgPdbSymbol :
    public CDialogImpl<CDlgPdbSymbol>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PDB_SYMBOL };

    CDlgPdbSymbol();
    virtual ~CDlgPdbSymbol();

    BOOL SetCodeSizeTree(IPdbCodeDetail *pCodeDetail);

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgPdbSymbol)
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

protected:
    VOID UpdateCodeSizeTree(IPdbCodeDetail *pCodeDetail, HTREEITEM hParentTreeItem);

private:
    CTabPanel m_oTabCodeSize;
    CTreeViewCtrlEx m_oTreeCodeSize;
    CComPtr<IPdbCodeDetail> m_pCodeDetail;
};