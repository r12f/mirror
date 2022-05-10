#pragma once

#include <vector>
#include "modules/compile/pdb_utils.h"
#include "base/com/simple_com_class.h"
#include "modules/compile/pdb/pdb_source_detail.h"

class PdbModuleDetailList;

class PdbModuleDetail :
    public SimpleComClassT<PdbModuleDetail, ComRef, IPdbModuleDetail>
{
    friend class PdbModuleDetailList;

    typedef std::vector<PdbSourceDetail *> RelatedFileList;

public:
    PdbModuleDetail();
    virtual ~PdbModuleDetail();

    // Override IPdbSymbolDetail
    STDMETHODIMP GetName(BSTR *pName);
    STDMETHODIMP GetSize(ULONGLONG *pSize);
    STDMETHODIMP Sort(DWORD nSortType);

    // Override IPdbModuleDetail
    STDMETHODIMP_(DWORD) GetRelatedFileCount();
    STDMETHODIMP GetRelatedFiles(IPdbSourceDetail **ppFiles, DWORD nMaxFileCount);

    // PdbModuleDetail
    HRESULT SetModule(IDiaSession *pSession, IDiaSymbol *pSymbol);

protected:
    BOOL ShouldReject();

protected:
    static bool SortDetailByName(PdbSourceDetail *lrs, PdbSourceDetail *rhs);

private:
    CComBSTR m_strName;
    ULONGLONG m_nSize;
    RelatedFileList m_vRelatedFiles;
};