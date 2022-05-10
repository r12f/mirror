#include "stdafx.h"
#include "modules/compile/pdb/pdb_module_detail_list.h"
#include <algorithm>

PdbModuleDetailList::PdbModuleDetailList()
    : m_nSize(0)
{

}

PdbModuleDetailList::~PdbModuleDetailList()
{
    PdbModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        (*oModuleIt)->Release();
        ++oModuleIt;
    }
}

HRESULT
PdbModuleDetailList::GetName(BSTR *pName)
{
    return E_NOTIMPL;
}

HRESULT
PdbModuleDetailList::GetSize(ULONGLONG *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    *pSize = m_nSize;
    return S_OK;
}

HRESULT
PdbModuleDetailList::Sort(DWORD nSortType)
{
    switch(nSortType) {
    case PCD_SORT_BY_NAME:
        std::sort(m_vModules.begin(), m_vModules.end(), SortDetailByName);
        break;
    case PCD_SORT_BY_SIZE:
        std::sort(m_vModules.begin(), m_vModules.end(), SortDetailBySize);
        break;
    }

    PdbModuleList::iterator oModuleIt = m_vModules.begin();
    while(oModuleIt != m_vModules.end()) {
        (*oModuleIt)->Sort(nSortType);
        ++oModuleIt;
    }

    return S_OK;
}

DWORD
PdbModuleDetailList::GetModuleCount()
{
    return (DWORD)m_vModules.size();
}

HRESULT
PdbModuleDetailList::GetModules(IPdbModuleDetail **pDetail, DWORD nMaxDetailCount)
{
    DWORD i = 0, nCount = GetModuleCount();
    for(i = 0; i < nCount && i < nMaxDetailCount; ++i) {
        pDetail[i] = m_vModules[i];
        pDetail[i]->AddRef();
    }

    return S_OK;
}

HRESULT
PdbModuleDetailList::AddModule(IDiaSession *pSession, IDiaSymbol *pSymbol)
{
    PdbModuleDetail *pDetail = new PdbModuleDetail;
    if(NULL == pDetail) {
        return E_OUTOFMEMORY;
    }

    HRESULT hr = pDetail->SetModule(pSession, pSymbol);
    if(FAILED(hr)) {
        return hr;
    }

    m_vModules.push_back(pDetail);
    m_nSize += pDetail->m_nSize;

    return S_OK;
}

bool
PdbModuleDetailList::SortDetailByName(PdbModuleDetail *lrs, PdbModuleDetail *rhs)
{
    return lrs->m_strName < rhs->m_strName;
}

bool
PdbModuleDetailList::SortDetailBySize(PdbModuleDetail *lrs, PdbModuleDetail *rhs)
{
    return lrs->m_nSize < rhs->m_nSize;
}