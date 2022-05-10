#include "stdafx.h"
#include "modules/compile/ui/pdb/dlg_pdb_compiland.h"
#include "base/string_number_convertions.h"

CDlgPdbCompiland::CDlgPdbCompiland()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgPdbCompiland::~CDlgPdbCompiland()
{

}

BOOL
CDlgPdbCompiland::SetModuleList(IPdbModuleDetailList *pModuleList)
{
    ASSERT_RETURN(NULL != pModuleList, FALSE);

    m_pModuleList = pModuleList;
    
    m_oListCodeSizeObject.SetRedraw(FALSE);
    m_oListCodeSizeObject.DeleteAllItems();
    DWORD nModuleCount = pModuleList->GetModuleCount();
    if(0 == nModuleCount) {
        m_oListCodeSizeObject.SetRedraw(TRUE);
        return TRUE;
    }

    std::vector<CComPtr<IPdbModuleDetail>> vModules;
    vModules.resize(nModuleCount);

    m_pModuleList->GetModules((IPdbModuleDetail **)(&vModules[0]), nModuleCount);

    CComBSTR strModuleName;
    for(DWORD i = 0; i < nModuleCount; ++i) {
        vModules[i]->GetName(&strModuleName);

        m_oListCodeSizeObject.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM, i, _T(""), 0, 0, -1, (LPARAM)(vModules[i].p));
        m_oListCodeSizeObject.SetItemText(i, 0, strModuleName);
    }

    if(nModuleCount > 0) {
        m_oListCodeSizeObject.SelectItem(0);
    }

    m_oListCodeSizeObject.SetRedraw(TRUE);

    UpdateModuleRelatedFile();

    return TRUE;
}

LRESULT
CDlgPdbCompiland::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oListCodeSizeObject.SubclassWindow(GetDlgItem(IDC_LIST_PDB_OBJECT));
    m_oListCodeSizeObject.SetExtendedListViewStyle(m_oListCodeSizeObject.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_oListCodeSizeObject.ModifyStyle(0, LVS_SINGLESEL, 0);

    LVCOLUMN oColumn = {0};
    oColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    oColumn.fmt = LVCFMT_LEFT;

    oColumn.pszText = _T("Object路径");
    oColumn.iSubItem = 0;
    oColumn.cx = 600;
    m_oListCodeSizeObject.InsertColumn(0, &oColumn);

    m_oListCodeSizeCompiland.SubclassWindow(GetDlgItem(IDC_LIST_PDB_COMPILAND));
    m_oListCodeSizeCompiland.SetExtendedListViewStyle(m_oListCodeSizeCompiland.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
    m_oListCodeSizeCompiland.ModifyStyle(0, LVS_SINGLESEL, 0);

    oColumn.pszText = _T("文件路径");
    oColumn.iSubItem = 0;
    oColumn.cx = 400;
    m_oListCodeSizeCompiland.InsertColumn(0, &oColumn);

    oColumn.pszText = _T("Checksum类型");
    oColumn.iSubItem = 0;
    oColumn.cx = 90;
    m_oListCodeSizeCompiland.InsertColumn(1, &oColumn);

    oColumn.pszText = _T("Checksum");
    oColumn.iSubItem = 0;
    oColumn.cx = 250;
    m_oListCodeSizeCompiland.InsertColumn(2, &oColumn);

    return TRUE;
}

VOID
CDlgPdbCompiland::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdbCompiland::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdbCompiland::OnClose()
{
    SetMsgHandled(FALSE);
    m_oListCodeSizeObject.UnsubclassWindow();
    m_oListCodeSizeCompiland.UnsubclassWindow();
    DestroyWindow();
    return;
}

VOID
CDlgPdbCompiland::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

LRESULT
CDlgPdbCompiland::OnSelectObject(LPNMHDR pNMHDR)
{
    if(pNMHDR->code != NM_CLICK) {
        return 0;
    }

    UpdateModuleRelatedFile();

    return 0;
}

VOID
CDlgPdbCompiland::UpdateModuleRelatedFile()
{
    m_oListCodeSizeCompiland.SetRedraw(FALSE);
    m_oListCodeSizeCompiland.DeleteAllItems();

    int nIndex = m_oListCodeSizeObject.GetSelectedIndex();
    if(nIndex < 0) {
        m_oListCodeSizeCompiland.SetRedraw(TRUE);
        return;
    }

    IPdbModuleDetail *pModule = (IPdbModuleDetail *)m_oListCodeSizeObject.GetItemData(nIndex);
    if(NULL == pModule) {
        m_oListCodeSizeCompiland.SetRedraw(TRUE);
        return;
    }

    DWORD nSourceCount = pModule->GetRelatedFileCount();
    if(0 == nSourceCount) {
        m_oListCodeSizeCompiland.SetRedraw(TRUE);
        return;
    }

    std::vector<CComPtr<IPdbSourceDetail>> vSources;
    vSources.resize(nSourceCount);

    pModule->GetRelatedFiles((IPdbSourceDetail **)(&vSources[0]), nSourceCount);

    CComBSTR strSourcePath;
    DWORD nChecksumType = 0;
    CComBSTR strChecksum;
    TCHAR *pChecksumType = NULL;
    for(DWORD i = 0; i < nSourceCount; ++i) {
        vSources[i]->GetPath(&strSourcePath);
        vSources[i]->GetChecksum(&strChecksum);

        switch(vSources[i]->GetChecksumType()) {
        case CHKSUM_TYPE_MD5:
            pChecksumType = _T("md5");
            break;
        case CHKSUM_TYPE_SHA1:
            pChecksumType = _T("sha1");
            break;
        default:
            pChecksumType = _T("none");
        }

        m_oListCodeSizeCompiland.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM, i, _T(""), 0, 0, -1, (LPARAM)(vSources[i].p));
        m_oListCodeSizeCompiland.SetItemText(i, 0, strSourcePath);
        m_oListCodeSizeCompiland.SetItemText(i, 1, pChecksumType);
        m_oListCodeSizeCompiland.SetItemText(i, 2, strChecksum);
    }

    m_oListCodeSizeCompiland.SetRedraw(TRUE);
}