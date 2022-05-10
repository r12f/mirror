#include "stdafx.h"
#include "modules/compile/ui/pdb/dlg_pdb_symbol.h"
#include "base/string_number_convertions.h"

CDlgPdbSymbol::CDlgPdbSymbol()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgPdbSymbol::~CDlgPdbSymbol()
{

}

BOOL
CDlgPdbSymbol::SetCodeSizeTree(IPdbCodeDetail *pCodeDetail)
{
    m_oTreeCodeSize.SetRedraw(FALSE);
    m_oTreeCodeSize.DeleteAllItems();
    m_pCodeDetail = pCodeDetail;
    UpdateCodeSizeTree(m_pCodeDetail, TVI_ROOT);
    m_oTreeCodeSize.SetRedraw(TRUE);
    return TRUE;
}


LRESULT
CDlgPdbSymbol::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oTreeCodeSize = GetDlgItem(IDC_TREE_PDB_SYMBOL);
    return TRUE;
}

VOID
CDlgPdbSymbol::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdbSymbol::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdbSymbol::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgPdbSymbol::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPdbSymbol::UpdateCodeSizeTree(IPdbCodeDetail *pCodeDetail, HTREEITEM hParentTreeItem)
{
    CComBSTR strCodeName;
    ULONGLONG nCodeSize = 0;
    pCodeDetail->GetName(&strCodeName);
    pCodeDetail->GetSize(&nCodeSize);

    CString strCodeDetail;
    strCodeDetail.Format(_T("%s (%s)"), strCodeName, FormatSizeToString(nCodeSize));

    HTREEITEM hTreeItem = NULL;
    TVINSERTSTRUCT oItem = {0};
    oItem.hParent = hParentTreeItem;
    oItem.hInsertAfter = TVI_LAST;
    oItem.item.mask = TVIF_TEXT | TVIF_SELECTEDIMAGE | TVIF_PARAM;
    oItem.item.pszText = strCodeDetail.GetBuffer();
    oItem.item.lParam = (LPARAM)pCodeDetail;
    hTreeItem = m_oTreeCodeSize.InsertItem(&oItem);
    if(NULL == hTreeItem) {
        return;
    }

    DWORD nCodeCount = pCodeDetail->GetOwnedCodeCount();
    if(0 == nCodeCount) {
        return;
    }

    std::vector<CComPtr<IPdbCodeDetail>> vCodes;
    vCodes.resize(nCodeCount);
    pCodeDetail->GetOwnedCodes((IPdbCodeDetail **)&(vCodes[0]), nCodeCount);

    DWORD i = 0;
    for(i = 0; i < nCodeCount; ++i) {
        UpdateCodeSizeTree(vCodes[i], hTreeItem);
    }

    if(hParentTreeItem == TVI_ROOT) {
        m_oTreeCodeSize.Expand(hTreeItem);
    }

    return;
}
