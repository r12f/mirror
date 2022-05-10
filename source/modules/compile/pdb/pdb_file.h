#pragma once

#include "modules/compile/pdb_utils.h"
#include "base/com/simple_com_class.h"

class PdbFile :
    public SimpleComClassT<PdbFile, ComRef, IPdbFile>
{
public:
    PdbFile();
    virtual ~PdbFile();

    STDMETHODIMP LoadPdb(BSTR pFile);
    STDMETHODIMP GetCodeDetail(BOOL bMergeTemplate, IPdbCodeDetail **ppCodeDetail);
    STDMETHODIMP GetModuleDetailList(IPdbModuleDetailList **ppList);

protected:
    HRESULT GetGlobalSymbolEnum(enum SymTagEnum nSymTag, IDiaEnumSymbols **ppEnumSymbol);

private:
    CComPtr<IDiaDataSource> m_pDiaDataSource;
    CComPtr<IDiaSession> m_pDiaSession;
};