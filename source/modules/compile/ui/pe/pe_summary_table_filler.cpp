#include "stdafx.h"
#include "pe_summary_table_filler.h"

struct PESummaryTableRow
{
    CString m_strElement;
    PEAddress m_nRVA;
    PEAddress m_nFOA;
    PEAddress m_nVA;
    PEAddress m_nSizeInMemory;
    PEAddress m_nSizeInFile;

    static LPCTSTR ProjectToElement(void *p) { return ((PESummaryTableRow*)p)->m_strElement; }
    static PEAddress ProjectToRVA(void *p) { return ((PESummaryTableRow*)p)->m_nRVA; }
    static PEAddress ProjectToFOA(void *p) { return ((PESummaryTableRow*)p)->m_nFOA; }
    static PEAddress ProjectToVA(void *p) { return ((PESummaryTableRow*)p)->m_nVA; }
    static PEAddress ProjectToSizeInMemory(void *p) { return ((PESummaryTableRow*)p)->m_nSizeInMemory; }
    static PEAddress ProjectToSizeInFile(void *p) { return ((PESummaryTableRow*)p)->m_nSizeInFile; }
};

static CTableColumn *s_vColumns [] {
    &((*new CTableColumnT<LPCTSTR>()).SetProjector(PESummaryTableRow::ProjectToElement).SetWidth(200).SetTitle(_T("元素"))),
    &((*new CTableColumnT<UINT64>()).SetProjector(PESummaryTableRow::ProjectToFOA).SetIsShowAsHex(true).SetWidth(100).SetTitle(_T("FOA"))),
    &((*new CTableColumnT<UINT64>()).SetProjector(PESummaryTableRow::ProjectToSizeInFile).SetWidth(120).SetTitle(_T("占用文件大小 (B)"))),
    &((*new CTableColumnT<UINT64>()).SetProjector(PESummaryTableRow::ProjectToRVA).SetIsShowAsHex(true).SetWidth(100).SetTitle(_T("RVA"))),
    &((*new CTableColumnT<UINT64>()).SetProjector(PESummaryTableRow::ProjectToVA).SetIsShowAsHex(true).SetWidth(100).SetTitle(_T("VA"))),
    &((*new CTableColumnT<UINT64>()).SetProjector(PESummaryTableRow::ProjectToSizeInMemory).SetWidth(120).SetTitle(_T("占用内存大小 (B)"))),
};
static int s_nColumnCount = sizeof(s_vColumns) / sizeof(s_vColumns[0]);

PESummaryTableFiller::PESummaryTableFiller(IPEFile *pPEFile)
    : m_pPEFile(pPEFile)
{
}

PESummaryTableFiller::~PESummaryTableFiller()
{
    if (NULL != m_pTableFillerWatcher) {
        delete m_pTableFillerWatcher;
    }

    if (NULL != m_pTableFiller) {
        delete m_pTableFiller;
    }
}

PESummaryTableFiller &
PESummaryTableFiller::SetTargetAsTreeList(CTreeListViewCtrl *pTreeListViewCtrl)
{
    m_pTableFiller = new CTableFillerTreeList(pTreeListViewCtrl, s_vColumns, s_nColumnCount);
    m_pTableFillerWatcher = new CTableFillerScopeWatcher(m_pTableFiller);
    return *this;
}

PESummaryTableFiller &
PESummaryTableFiller::ParseDosHeader()
{
    ASSERT_RETURN(NULL != m_pPEFile, *this);
    ASSERT_RETURN(NULL != m_pTableFiller, *this);

    LibPEPtr<IPEDosHeader> pDosHeader;
    ASSERT_RETURN(SUCCEEDED(m_pPEFile->GetDosHeader(&pDosHeader)), *this);
    ASSERT_RETURN(NULL != pDosHeader, *this);

    AddPEElementToTable(_T("DOS Header"), pDosHeader);

    return *this;
}

PESummaryTableFiller &
PESummaryTableFiller::ParseNtHeader()
{
    ASSERT_RETURN(NULL != m_pPEFile, *this);
    ASSERT_RETURN(NULL != m_pTableFiller, *this);

    LibPEPtr<IPENtHeaders> pNtHeaders;
    ASSERT_RETURN(SUCCEEDED(m_pPEFile->GetNtHeaders(&pNtHeaders)), *this);
    ASSERT_RETURN(NULL != pNtHeaders, *this);

    LibPEPtr<IPEFileHeader> pFileHeader;
    ASSERT_RETURN(SUCCEEDED(pNtHeaders->GetFileHeader(&pFileHeader)), *this);
    ASSERT_RETURN(NULL != pFileHeader, *this);

    LibPEPtr<IPEOptionalHeader> pOptionalHeader;
    ASSERT_RETURN(SUCCEEDED(pNtHeaders->GetOptionalHeader(&pOptionalHeader)), *this);
    ASSERT_RETURN(NULL != pOptionalHeader, *this);

    AddPEElementToTable(_T("NT Headers"), pNtHeaders);
    AddPEElementToTable(_T("NT Headers\\File Header"), pFileHeader);
    AddPEElementToTable(_T("NT Headers\\Optional Header"), pOptionalHeader);

    return *this;
}

PESummaryTableFiller &
PESummaryTableFiller::ParseSection()
{
    ASSERT_RETURN(NULL != m_pPEFile, *this);
    ASSERT_RETURN(NULL != m_pTableFiller, *this);

    UINT32 nSectionCount = m_pPEFile->GetSectionCount();
    for (UINT32 nSectionIndex = 0; nSectionIndex < nSectionCount; ++nSectionIndex) {
        LibPEPtr<IPESectionHeader> pSectionHeader;
        ASSERT_RETURN(SUCCEEDED(m_pPEFile->GetSectionHeader(nSectionIndex, &pSectionHeader)), *this);
        ASSERT_RETURN(NULL != pSectionHeader, *this);

        LibPEPtr<IPESection> pSection;
        ASSERT_RETURN(SUCCEEDED(pSectionHeader->GetSection(&pSection)), *this);
        ASSERT_RETURN(NULL != pSection, *this);

        CStringA strSectionNameA = pSection->GetName();
        CString strSectionName = CA2T(strSectionNameA);

        CString strSectionHeaderName;
        strSectionHeaderName.Format(_T("Section Headers\\%s"), strSectionName);
        AddPEElementToTable(strSectionHeaderName, pSectionHeader);

        CString strSectionContentName;
        strSectionContentName.Format(_T("Sections\\%s"), strSectionName);
        AddPEElementToTable(strSectionContentName, pSection);
    }

    return *this;
}

void
PESummaryTableFiller::AddPEElementToTable(LPCTSTR pElementName, IPEElement *pElement)
{
    ASSERT_RETURN_VOID(NULL != pElement);

    PESummaryTableRow oRow;
    oRow.m_strElement = pElementName;
    oRow.m_nRVA = pElement->GetRVA();
    oRow.m_nFOA = pElement->GetFOA();
    oRow.m_nVA = pElement->GetVA();
    oRow.m_nSizeInMemory = pElement->GetSizeInMemory();
    oRow.m_nSizeInFile = pElement->GetSizeInFile();

    m_pTableFiller->AddRow(&oRow);

    return;
}