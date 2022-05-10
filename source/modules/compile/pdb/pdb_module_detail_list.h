#pragma once

#include <vector>
#include <map>
#include "modules/compile/pdb_utils.h"
#include "base/com/simple_com_class.h"
#include "modules/compile/pdb/pdb_module_detail.h"

class PdbModuleDetailList :
    public SimpleComClassT<PdbModuleDetailList, ComRef, IPdbModuleDetailList>
{
    typedef std::vector<PdbModuleDetail *> PdbModuleList;

public:
    PdbModuleDetailList();
    virtual ~PdbModuleDetailList();

    // Override IPdbSymbolDetail
    STDMETHODIMP GetName(BSTR *pName);
    STDMETHODIMP GetSize(ULONGLONG *pSize);
    STDMETHODIMP Sort(DWORD nSortType);

    // Override IPdbModuleDetailList
    STDMETHODIMP_(DWORD) GetModuleCount();
    STDMETHODIMP GetModules(IPdbModuleDetail **pDetail, DWORD nMaxDetailCount);

    // PdbModuleDetailList
    HRESULT AddModule(IDiaSession *pSession, IDiaSymbol *pSymbol);

protected:
    static bool SortDetailByName(PdbModuleDetail *lrs, PdbModuleDetail *rhs);
    static bool SortDetailBySize(PdbModuleDetail *lrs, PdbModuleDetail *rhs);

private:
    ULONGLONG m_nSize;
    PdbModuleList m_vModules;
};