#pragma once

#include "modules/compile/pdb_utils.h"
#include "modules/compile/ui/pdb/dlg_pdb_symbol.h"
#include "modules/compile/ui/pdb/dlg_pdb_compiland.h"

class CDlgPdb :
    public CDialogImpl<CDlgPdb>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PDB };

    CDlgPdb();
    virtual ~CDlgPdb();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgPdb)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_PDB_LOAD_SYMBOL, OnLoadSymbol)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnLoadSymbol(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    VOID LoadPdb(LPCTSTR pFileName);
    VOID GetCodeDetail(IPdbFile *pPdbFile, BOOL bMergeTemplate, IPdbCodeDetail **ppCodeDetail);
    VOID GetModuleDetailList(IPdbFile *pPdbFile, IPdbModuleDetailList **ppCodeDetailList);

private:
    CTabPanel m_oTabCodeSize;
    CDlgPdbSymbol m_oDlgSymbolWithTemplate;
    CDlgPdbSymbol m_oDlgSymbolWithoutTemplate;
    CDlgPdbCompiland m_oDlgCompilandView;
    CComPtr<IPdbCodeDetail> m_pCodeDetailWithTemplate;
    CComPtr<IPdbCodeDetail> m_pCodeDetailWithoutTemplate;
    CComPtr<IPdbModuleDetailList> m_pModuleDetailList;
    CString m_strSymFilePath;
};