#include "stdafx.h"
#include "table_filler.h"
#include "../base/comm_ctrl_utils.h"

CTableFillerWithHeaderCtrl::CTableFillerWithHeaderCtrl(HWND hHeaderCtrl, CTableColumn **pColumns, int nColumnCount)
    : CTableFiller(pColumns, nColumnCount)
{
    m_oHeaderCtrl.Attach(hHeaderCtrl);
}

void
CTableFillerWithHeaderCtrl::Begin()
{
    ASSERT_RETURN_VOID(m_oHeaderCtrl.IsWindow());

    for (int nColumnIndex = 0; nColumnIndex < GetColumnCount(); ++nColumnIndex) {
        CTableColumn *pColumn = GetColumn(nColumnIndex);
        CDItem oHeaderItem;
        oHeaderItem.Width(pColumn->m_nWidth).SetAlign(HDF_LEFT).Title(pColumn->m_strTitle.GetBuffer());
        m_oHeaderCtrl.AddItem(&oHeaderItem);
    }

    return;
}

CTableFillerTreeList::CTableFillerTreeList(CTreeListViewCtrl *pTreeListCtrl, CTableColumn **pColumns, int nColumnCount)
    : CTableFillerWithHeaderCtrl(pTreeListCtrl->GetHeaderControl().m_hWnd, pColumns, nColumnCount)
    , m_pTreeListCtrl(pTreeListCtrl)
{
    m_oTreeCtrl = pTreeListCtrl->GetTreeControl();
}

void
CTableFillerTreeList::Begin()
{
    ASSERT_RETURN_VOID(NULL != m_pTreeListCtrl);
    m_pTreeListCtrl->SetRedraw(FALSE);
    __super::Begin();
    return;
}

void
CTableFillerTreeList::AddRow(void *pData)
{
    ASSERT_RETURN_VOID(NULL != pData);
    ASSERT_RETURN_VOID(NULL != m_pTreeListCtrl);

    HTREEITEM hItem = NULL;
    for (int nColumnIndex = 0; nColumnIndex < GetColumnCount(); ++nColumnIndex) {
        CTableColumn *pColumn = GetColumn(nColumnIndex);

        CTableCell *pCell = pColumn->ProcessData(pData);
        ASSERT_RETURN_VOID(NULL != pCell);

        LPCTSTR strData = pCell->ToString();

        // In tree list, if column index is 0, it is a tree node, otherwise it is a list column
        if (nColumnIndex == 0) {
            hItem = MakeTreeNode(strData);
        }
        else {
            m_pTreeListCtrl->SetSubItemText(hItem, nColumnIndex, strData);
        }
    }

    return;
}

void 
CTableFillerTreeList::End()
{
    ASSERT_RETURN_VOID(NULL != m_pTreeListCtrl);
    m_pTreeListCtrl->SetRedraw(TRUE);
    return;
}

HTREEITEM
CTableFillerTreeList::MakeTreeNode(LPCTSTR pPath)
{
    std::vector<CString> vPendingTreePaths;

    CString strTreeNodePath = pPath;

    auto itTreeNodePath = m_vPathToTreeNode.find(strTreeNodePath);
    while (itTreeNodePath == m_vPathToTreeNode.end()) {
        vPendingTreePaths.push_back(strTreeNodePath);
        int nLastPath = strTreeNodePath.ReverseFind(_T('\\'));
        if (nLastPath < 0) {
            break;
        }
        strTreeNodePath = strTreeNodePath.Left(nLastPath);
        itTreeNodePath = m_vPathToTreeNode.find(strTreeNodePath);
    }

    // If there is no pending tree path, the itTreeNodePath must point to an existing node.
    if (vPendingTreePaths.size() == 0) {
        return itTreeNodePath->second;
    }

    HTREEITEM hTreeItem = NULL;
    HTREEITEM hParentTreeItem = NULL;
    if (itTreeNodePath != m_vPathToTreeNode.end()) {
        hParentTreeItem = itTreeNodePath->second;
    }

    CString strTreeNodeName;
    for (auto itPendingTreeNodePath = vPendingTreePaths.rbegin();
        itPendingTreeNodePath != vPendingTreePaths.rend();
        ++itPendingTreeNodePath)
    {
        int nLastPath = (*itPendingTreeNodePath).ReverseFind(_T('\\'));
        if (nLastPath < 0) {
            strTreeNodeName = *itPendingTreeNodePath;
        }
        else {
            strTreeNodeName = (*itPendingTreeNodePath).Right((*itPendingTreeNodePath).GetLength() - nLastPath - 1);
        }

        hTreeItem = m_oTreeCtrl.InsertItem(strTreeNodeName, hParentTreeItem, NULL);
        hParentTreeItem = hTreeItem;
        m_vPathToTreeNode[*itPendingTreeNodePath] = hTreeItem;
    }

    return hTreeItem;
}
