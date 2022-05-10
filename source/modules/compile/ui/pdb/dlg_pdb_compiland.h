#pragma once

#include "modules/compile/pdb_utils.h"

class CDlgPdbCompiland :
    public CDialogImpl<CDlgPdbCompiland>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PDB_COMPILAND };

    CDlgPdbCompiland();
    virtual ~CDlgPdbCompiland();

    BOOL SetModuleList(IPdbModuleDetailList *pModuleList);

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgPdbCompiland)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        NOTIFY_ID_HANDLER_EX(IDC_LIST_PDB_OBJECT, OnSelectObject)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    LRESULT OnSelectObject(LPNMHDR pNMHDR);

protected:
    VOID UpdateModuleRelatedFile();

private:
    CTabPanel m_oTabCodeSize;
    CEditableListCtrl m_oListCodeSizeObject;
    CEditableListCtrl m_oListCodeSizeCompiland;
    CComPtr<IPdbModuleDetailList> m_pModuleList;
};