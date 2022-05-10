#pragma once

#include "ui/ui_engine/datatable/table_column.h"
#include "ui/ui_engine/controls/tree_list_ctrl.h"

class CTableFiller
{
public:
    CTableFiller(CTableColumn **pColumns, int nColumnCount)
        : m_pColumns(pColumns)
        , m_nColumnCount(nColumnCount)
    {}

    virtual ~CTableFiller() {}

    virtual void Begin() {}
    virtual void AddRow(void *pData) {}
    virtual void End() {}

protected:
    CTableColumn * GetColumn(int nIndex) const { return m_pColumns[nIndex]; }
    int GetColumnCount() const { return m_nColumnCount; }

private:
    CTableColumn **m_pColumns;
    int m_nColumnCount;
};

class CTableFillerScopeWatcher
{
public:
    CTableFillerScopeWatcher(CTableFiller *pTableFiller)
        : m_pTableFiller(pTableFiller)
    {
        if (NULL != m_pTableFiller) {
            m_pTableFiller->Begin();
        }
    }

    ~CTableFillerScopeWatcher()
    {
        if (NULL != m_pTableFiller) {
            m_pTableFiller->End();
        }
    }

private:
    CTableFiller *m_pTableFiller;
};

class CTableFillerWithHeaderCtrl :
    public CTableFiller
{
public:
    CTableFillerWithHeaderCtrl(HWND hHeaderCtrl, CTableColumn **pColumns, int nColumnCount);

    virtual void Begin();

private:
    CHeaderCtrl m_oHeaderCtrl;
};

class CTableFillerTreeList :
    public CTableFillerWithHeaderCtrl
{
public:
    CTableFillerTreeList(CTreeListViewCtrl *pTreeListCtrl, CTableColumn **pColumns, int nColumnCount);

    virtual void Begin();
    virtual void AddRow(void *pData);
    virtual void End();

protected:
    HTREEITEM MakeTreeNode(LPCTSTR pPath);

private:
    CTreeListViewCtrl *m_pTreeListCtrl;
    CTreeViewCtrl m_oTreeCtrl;
    std::map<CString, HTREEITEM> m_vPathToTreeNode;
};