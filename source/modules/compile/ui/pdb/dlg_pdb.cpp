#include "stdafx.h"
#include "modules/compile/ui/pdb/dlg_pdb.h"
#include "base/string_number_convertions.h"

CDlgPdb::CDlgPdb()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgPdb::~CDlgPdb()
{

}

LRESULT
CDlgPdb::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    return TRUE;
}

VOID
CDlgPdb::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdb::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdb::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgPdb::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdb::OnLoadSymbol(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CFileDialog oDlgLoadSymbol(TRUE, _T("pdb"), _T("*.pdb"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("*.pdb (Program database)"), m_hWnd);
    if(IDOK != oDlgLoadSymbol.DoModal()) {
        return;
    }

    LoadPdb(oDlgLoadSymbol.m_szFileName);

    return;
}

HWND
CDlgPdb::GetNativeWindowFromResId(LPCSTR strId)
{
    CStringA strResId = strId;

    if(0 == strResId.CompareNoCase("IDC_TAB_COMPILE_PDB")) {
        m_oTabCodeSize.Create(m_hWnd);

        m_oDlgSymbolWithTemplate.Create(m_oTabCodeSize.m_tab);
        m_oTabCodeSize.AddPage(m_oDlgSymbolWithTemplate, _T("Symbol（展开模版）"));

        m_oDlgSymbolWithoutTemplate.Create(m_oTabCodeSize.m_tab);
        m_oTabCodeSize.AddPage(m_oDlgSymbolWithoutTemplate, _T("Symbol（合并模版）"));

        m_oDlgCompilandView.Create(m_oTabCodeSize.m_tab);
        m_oTabCodeSize.AddPage(m_oDlgCompilandView, _T("Compiland"));

        m_oTabCodeSize.SetActivePage(0);

        m_oTabCodeSize.ShowWindow(SW_SHOW);

        return m_oTabCodeSize;
    }

    return NULL;
}

VOID
CDlgPdb::LoadPdb(LPCTSTR pFileName)
{
    CComPtr<IPdbFile> pPdbFile;
    MirrorCreateObject(MODULE_COMPILE, __uuidof(IPdbFile), (void **)&pPdbFile);
    ASSERT_RETURN_VOID(NULL != pPdbFile);

    if(FAILED(pPdbFile->LoadPdb(CComBSTR(pFileName)))) {
        return;
    }

    m_pCodeDetailWithTemplate = NULL;
    m_pCodeDetailWithoutTemplate = NULL;
    m_pModuleDetailList = NULL;
    GetCodeDetail(pPdbFile, TRUE, &m_pCodeDetailWithoutTemplate);
    GetCodeDetail(pPdbFile, FALSE, &m_pCodeDetailWithTemplate);
    GetModuleDetailList(pPdbFile, &m_pModuleDetailList);

    CStatic oStaFilePath = GetDlgItem(IDC_STATIC_PDB_PDB_FILE_PATH);
    oStaFilePath.SetWindowText(pFileName);
    m_strSymFilePath = pFileName;

    m_oDlgSymbolWithTemplate.SetCodeSizeTree(m_pCodeDetailWithTemplate);
    m_oDlgSymbolWithoutTemplate.SetCodeSizeTree(m_pCodeDetailWithoutTemplate);
    m_oDlgCompilandView.SetModuleList(m_pModuleDetailList);

    return;
}

VOID
CDlgPdb::GetCodeDetail(IPdbFile *pPdbFile, BOOL bMergeTemplate, IPdbCodeDetail **ppCodeDetail)
{
    CComPtr<IPdbCodeDetail> pCodeDetail;
    if(FAILED(pPdbFile->GetCodeDetail(bMergeTemplate, &pCodeDetail)) || NULL == pCodeDetail) {
        return;
    }

    pCodeDetail->Sort(PCD_SORT_BY_SIZE);

    *ppCodeDetail = pCodeDetail.Detach();

    return;
}

VOID
CDlgPdb::GetModuleDetailList(IPdbFile *pPdbFile, IPdbModuleDetailList **ppModuleDetailList)
{
    CComPtr<IPdbModuleDetailList> pModuleDetailList;
    if(FAILED(pPdbFile->GetModuleDetailList(&pModuleDetailList)) || NULL == pModuleDetailList) {
        return;
    }

    pModuleDetailList->Sort(PCD_SORT_BY_SIZE);

    *ppModuleDetailList = pModuleDetailList.Detach();

    return;
}