#include "stdafx.h"
#include "modules/compile/pdb/pdb_code_detail.h"
#include <algorithm>

PdbCodeDetail::PdbCodeDetail()
    : m_nCodeBlockSize(0)
{

}

PdbCodeDetail::~PdbCodeDetail()
{
    OwnedCodeList::iterator oCodeIt = m_vOwnedCodes.begin();
    while(oCodeIt != m_vOwnedCodes.end()) {
        (*oCodeIt)->Release();
        ++oCodeIt;
    }
}

HRESULT
PdbCodeDetail::GetName(BSTR *pName)
{
    m_strCodeBlockName.CopyTo(pName);
    return S_OK;
}

HRESULT
PdbCodeDetail::GetSize(ULONGLONG *pCodeSize)
{
    ASSERT_RETURN(NULL != pCodeSize, E_POINTER);
    *pCodeSize = m_nCodeBlockSize;
    return S_OK;
}

HRESULT
PdbCodeDetail::Sort(DWORD nSortType)
{
    switch(nSortType) {
    case PCD_SORT_BY_NAME:
        std::sort(m_vOwnedCodes.begin(), m_vOwnedCodes.end(), SortDetailByName);
        break;
    case PCD_SORT_BY_SIZE:
        std::sort(m_vOwnedCodes.begin(), m_vOwnedCodes.end(), SortDetailBySize);
        break;
    }

    OwnedCodeList::iterator oCodeIt = m_vOwnedCodes.begin();
    while(oCodeIt != m_vOwnedCodes.end()) {
        (*oCodeIt)->Sort(nSortType);
        ++oCodeIt;
    }

    return S_OK;
}

DWORD
PdbCodeDetail::GetOwnedCodeCount()
{
    return m_vOwnedCodes.size();
}

HRESULT
PdbCodeDetail::GetOwnedCodes(IPdbCodeDetail **ppDetails, DWORD nMaxCodeCount)
{
    ASSERT_RETURN(NULL != ppDetails, E_POINTER);

    DWORD nCopiedCount = 0;
    OwnedCodeList::iterator oCodeIt = m_vOwnedCodes.begin();
    while(nCopiedCount < nMaxCodeCount && oCodeIt != m_vOwnedCodes.end()) {
        (*oCodeIt)->QueryInterface(__uuidof(IPdbCodeDetail), (void **)&(ppDetails[nCopiedCount]));
        ++nCopiedCount;
        ++oCodeIt;
    }

    return S_OK;
}

BOOL
PdbCodeDetail::SetCodeBlockName(LPCTSTR pName)
{
    m_strCodeBlockName = pName;
    return TRUE;
}

HRESULT
PdbCodeDetail::AppendChildCode(IDiaSymbol *pSymbol, BOOL bMergeTemplate)
{
    ASSERT_RETURN(NULL != pSymbol, E_POINTER);

    CComBSTR strName;
    pSymbol->get_name(&strName);

    CodeDetail oCodeDetail;
    pSymbol->get_length(&(oCodeDetail.m_nCodeSize));
    m_nCodeBlockSize += oCodeDetail.m_nCodeSize;

    CString strInternalName = strName;
    return AppendChildCodeInteral(strInternalName, oCodeDetail, bMergeTemplate);
}

HRESULT
PdbCodeDetail::AppendChildCodeInteral(CString &strName, CodeDetail &oDetail, BOOL bMergeTemplate)
{
    if(strName.GetLength() == 0) {
        return S_OK;
    }

    CString strMyName, strChildName;
    GetCodeAndChildCodeName(strName, strMyName, strChildName);

    if(bMergeTemplate) {
        StripTemplateInfo(strMyName);
    }

    PdbCodeDetail *pCodeDetail = NULL;
    OwnedCodeList::iterator oCodeIt = GetCodeIt(strMyName);
    if(oCodeIt == m_vOwnedCodes.end()) {
        pCodeDetail = new PdbCodeDetail;
        if(NULL == pCodeDetail) {
            return E_OUTOFMEMORY;
        }
        pCodeDetail->m_strCodeBlockName = strMyName;
        m_vOwnedCodes.push_back(pCodeDetail);
    } else {
        pCodeDetail = (*oCodeIt);
    }

    pCodeDetail->m_nCodeBlockSize += oDetail.m_nCodeSize;

    return pCodeDetail->AppendChildCodeInteral(strChildName, oDetail, bMergeTemplate);
}

VOID
PdbCodeDetail::GetCodeAndChildCodeName(CString &strName, CString &strMyName, CString &strChildName)
{
    DWORD nScopeLevel = 0, nColonNum = 0;
    int i = 0, nNameLength = strName.GetLength();
    for(i = 0; i < nNameLength; ++i) {
        if(strName[i] == _T('<')) {
            ++nScopeLevel;
        } else if(strName[i] == _T('>')) {
            --nScopeLevel;
        } else if(strName[i] == _T(':')) {
            if(nScopeLevel == 0) {
                ++nColonNum;
                if(2 == nColonNum && i + 1 < nNameLength) {
                    strMyName = strName.Left(i - 1);
                    strChildName = strName.Mid(i + 1);
                    return;
                }
            }
        }
    }

    strMyName = strName;

    return;
}

VOID
PdbCodeDetail::StripTemplateInfo(CString &strCodeName)
{
    int nIndex = strCodeName.Find(_T('<'));
    if(nIndex >= 0) {
        strCodeName = strCodeName.Left(nIndex);
    }
}

PdbCodeDetail::OwnedCodeList::iterator
PdbCodeDetail::GetCodeIt(CString &strName)
{
    OwnedCodeList::iterator oCodeIt = m_vOwnedCodes.begin();
    while(oCodeIt != m_vOwnedCodes.end()) {
        if(strName == (*oCodeIt)->m_strCodeBlockName) {
            break;
        }
        ++oCodeIt;
    }

    return oCodeIt;
}

bool
PdbCodeDetail::SortDetailByName(PdbCodeDetail *lrs, PdbCodeDetail *rhs)
{
    return lrs->m_strCodeBlockName > rhs->m_strCodeBlockName;
}

bool
PdbCodeDetail::SortDetailBySize(PdbCodeDetail *lrs, PdbCodeDetail *rhs)
{
    return lrs->m_nCodeBlockSize > rhs->m_nCodeBlockSize;
}