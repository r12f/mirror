#pragma once

class PESummaryTableFiller
{
public:
    PESummaryTableFiller(IPEFile *pPEFile);
    ~PESummaryTableFiller();

    PESummaryTableFiller & SetTargetAsTreeList(CTreeListViewCtrl *pTreeListViewCtrl);
    PESummaryTableFiller & ParseDosHeader();
    PESummaryTableFiller & ParseNtHeader();
    PESummaryTableFiller & ParseSection();

protected:
    void AddPEElementToTable(LPCTSTR pElementName, IPEElement *pElement);

private:
    CTableFiller *m_pTableFiller;
    CTableFillerScopeWatcher *m_pTableFillerWatcher;
    LibPEPtr<IPEFile> m_pPEFile;
};
