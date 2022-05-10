#include "stdafx.h"
#include "modules/compile/pdb/pdb_module_detail.h"
#include "modules/compile/pdb/pdb_module_detail_list.h"
#include <algorithm>

static TCHAR *s_pIgnoreModule[] = {
    _T("* Linker *"),
    _T("* CIL *"),
    _T("Import:")
};

static DWORD s_nIgnoreModuleCount = sizeof(s_pIgnoreModule) / sizeof(TCHAR *);

PdbModuleDetail::PdbModuleDetail()
    : m_nSize(0)
{

}

PdbModuleDetail::~PdbModuleDetail()
{
    RelatedFileList::iterator oFileIt = m_vRelatedFiles.begin();
    while(oFileIt != m_vRelatedFiles.end()) {
        (*oFileIt)->Release();
        ++oFileIt;
    }
}

HRESULT
PdbModuleDetail::GetName(BSTR *pName)
{
    return m_strName.CopyTo(pName);
}

HRESULT
PdbModuleDetail::GetSize(ULONGLONG *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    *pSize = m_nSize;
    return S_OK;
}

HRESULT
PdbModuleDetail::Sort(DWORD nSortType)
{
    std::sort(m_vRelatedFiles.begin(), m_vRelatedFiles.end(), SortDetailByName);
    return S_OK;
}

DWORD
PdbModuleDetail::GetRelatedFileCount()
{
    return (DWORD)m_vRelatedFiles.size();
}

HRESULT
PdbModuleDetail::GetRelatedFiles(IPdbSourceDetail **ppFiles, DWORD nMaxFileCount)
{
    ASSERT_RETURN(NULL != ppFiles, E_POINTER);

    DWORD i = 0, nCount = GetRelatedFileCount();
    for(i = 0; i < nCount && i < nMaxFileCount; ++i) {
        ppFiles[i] = m_vRelatedFiles[i];
        ppFiles[i]->AddRef();
    }

    return S_OK;
}

HRESULT
PdbModuleDetail::SetModule(IDiaSession *pSession, IDiaSymbol *pSymbol)
{
    pSymbol->get_name(&m_strName);
    pSymbol->get_length(&m_nSize);

    if(ShouldReject()) {
        return E_FAIL;
    }

    // Get file list
    CComPtr<IDiaEnumSourceFiles> pEnumFile;
    HRESULT hr = pSession->findFile(pSymbol, NULL, nsNone, &pEnumFile);
    if(FAILED(hr) || NULL == pEnumFile) {
        MLOG_ERROR("Pdb", "±È¿˙Symbol ß∞‹£∫Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    // Do something with each IDiaTable.
    ULONG celt = 0;
    CComPtr<IDiaSourceFile> pSourceFile;
    PdbSourceDetail *pSourceDetail = NULL;
    while(SUCCEEDED(hr = pEnumFile->Next(1, &pSourceFile, &celt)) && celt == 1) {
        pSourceDetail = new PdbSourceDetail;
        if(NULL != pSourceDetail) {
            if(SUCCEEDED(pSourceDetail->SetSource(pSourceFile))) {
                m_vRelatedFiles.push_back(pSourceDetail);
            }
        }
        pSourceFile = NULL;
    }

    return S_OK;
}

BOOL
PdbModuleDetail::ShouldReject()
{
    DWORD i = 0;
    for(i = 0; i < s_nIgnoreModuleCount; ++i) {
        if(NULL != _tcsstr(m_strName, s_pIgnoreModule[i])) {
            return TRUE;
        }
    }

    return FALSE;
}

bool
PdbModuleDetail::SortDetailByName(PdbSourceDetail *lrs, PdbSourceDetail *rhs)
{
    return lrs->m_strName < rhs->m_strName;
}
