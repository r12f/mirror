#pragma once

#include <vector>
#include <map>
#include "modules/compile/pdb_utils.h"
#include "base/com/simple_com_class.h"

class PdbCodeDetail :
    public SimpleComClassT<PdbCodeDetail, ComRef, IPdbCodeDetail>
{
    struct CodeDetail {
        ULONGLONG m_nCodeSize;
    };

    typedef std::vector<PdbCodeDetail *> OwnedCodeList;

public:
    PdbCodeDetail();
    virtual ~PdbCodeDetail();

    // Override IPdbSymbolDetail
    STDMETHODIMP GetName(BSTR *pName);
    STDMETHODIMP GetSize(ULONGLONG *pSize);
    STDMETHODIMP Sort(DWORD nSortType);

    // Override IPdbCodeDetail
    STDMETHODIMP_(DWORD) GetOwnedCodeCount();
    STDMETHODIMP GetOwnedCodes(IPdbCodeDetail **ppDetails, DWORD nMaxCodeCount);

    // PdbCodeDetail
    BOOL SetCodeBlockName(LPCTSTR pName);
    HRESULT AppendChildCode(IDiaSymbol *pSymbol, BOOL bMergeTemplate);

protected:
    HRESULT AppendChildCodeInteral(CString &strName, CodeDetail &oDetail, BOOL bMergeTemplate);
    VOID GetCodeAndChildCodeName(CString &strName, CString &strMyName, CString &strChildName);
    VOID StripTemplateInfo(CString &strCodeName);
    OwnedCodeList::iterator GetCodeIt(CString &strName);

    static bool SortDetailByName(PdbCodeDetail *lrs, PdbCodeDetail *rhs);
    static bool SortDetailBySize(PdbCodeDetail *lrs, PdbCodeDetail *rhs);

private:
    CComBSTR m_strCodeBlockName;
    ULONGLONG m_nCodeBlockSize;
    OwnedCodeList m_vOwnedCodes;
};