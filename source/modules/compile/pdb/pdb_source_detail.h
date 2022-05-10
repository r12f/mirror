#pragma once

#include <vector>
#include "modules/compile/pdb_utils.h"
#include "base/com/simple_com_class.h"

class PdbModuleDetail;

class PdbSourceDetail :
    public SimpleComClassT<PdbSourceDetail, ComRef, IPdbSourceDetail>
{
    friend class PdbModuleDetail;

public:
    PdbSourceDetail();
    virtual ~PdbSourceDetail();

    // Override IPdbSourceDetail
    STDMETHODIMP_(DWORD) GetUniqueId();
    STDMETHODIMP GetPath(BSTR *pName);
    STDMETHODIMP_(DWORD) GetChecksumType();
    STDMETHODIMP GetChecksum(BSTR *pChecksum);

    // PdbSourceDetail
    HRESULT SetSource(IDiaSourceFile *pSource);

private:
    CComBSTR m_strName;
    DWORD m_nId;
    DWORD m_nChecksumType;
    CComBSTR m_strChecksum;
};