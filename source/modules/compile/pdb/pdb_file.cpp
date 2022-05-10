#include "stdafx.h"
#include "modules/compile/pdb/pdb_file.h"
#include "modules/compile/pdb/pdb_code_detail.h"
#include "modules/compile/pdb/pdb_module_detail_list.h"

PdbFile::PdbFile()
{
    HRESULT hr = CoInitialize(NULL);
    ATLASSERT(SUCCEEDED(hr));
}

PdbFile::~PdbFile()
{
    ::CoUninitialize();
}

HRESULT
PdbFile::LoadPdb(BSTR pFile)
{
    ASSERT_RETURN(NULL != pFile, E_POINTER);

    HRESULT hr = CoCreateInstance(__uuidof(DiaSource), 
        NULL, CLSCTX_INPROC_SERVER, 
        __uuidof(IDiaDataSource), 
        (void**)&m_pDiaDataSource);

    if(FAILED(hr) || NULL == m_pDiaDataSource) {
        MLOG_ERROR("Pdb", "创建DiaDataSource失败：Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    hr = m_pDiaDataSource->loadDataFromPdb(pFile);
    if(FAILED(hr) || NULL == m_pDiaDataSource) {
        MLOG_ERROR("Pdb", "加载PDB文件失败：Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    hr = m_pDiaDataSource->openSession(&m_pDiaSession);
    if(FAILED(hr) || NULL == m_pDiaDataSource) {
        MLOG_ERROR("Pdb", "创建PDB分析Session失败：Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    return S_OK;
}

HRESULT
PdbFile::GetCodeDetail(BOOL bMergeTemplate, IPdbCodeDetail **ppCodeDetail)
{
    ASSERT_RETURN(NULL != ppCodeDetail, E_POINTER);

    CComPtr<IDiaEnumSymbols> pEnumSymbols;
    HRESULT hr = GetGlobalSymbolEnum(SymTagFunction, &pEnumSymbols);
    if(FAILED(hr) || NULL == pEnumSymbols) {
        return hr;
    }

    PdbCodeDetail *pGlobalScope = new PdbCodeDetail;
    if(NULL == pGlobalScope) {
        return E_OUTOFMEMORY;
    }

    pGlobalScope->SetCodeBlockName(_T("<GLOBAL>"));

    // Do something with each IDiaTable.
    ULONG celt = 0;
    CComPtr<IDiaSymbol> pSymbol;
    while(SUCCEEDED(hr = pEnumSymbols->Next(1, &pSymbol, &celt)) && celt == 1) {
        pGlobalScope->AppendChildCode(pSymbol, bMergeTemplate);
        pSymbol = NULL;
    }

    *ppCodeDetail = pGlobalScope;

    return S_OK;
}

HRESULT
PdbFile::GetModuleDetailList(IPdbModuleDetailList **ppList)
{
    ASSERT_RETURN(NULL != ppList, E_POINTER);

    CComPtr<IDiaEnumSymbols> pEnumSymbols;
    HRESULT hr = GetGlobalSymbolEnum(SymTagCompiland, &pEnumSymbols);
    if(FAILED(hr) || NULL == pEnumSymbols) {
        return hr;
    }

    PdbModuleDetailList *pModuleDetailList = new PdbModuleDetailList;
    if(NULL == pModuleDetailList) {
        return E_OUTOFMEMORY;
    }

    ULONG celt = 0;
    CComPtr<IDiaSymbol> pSymbol;
    while(SUCCEEDED(hr = pEnumSymbols->Next(1, &pSymbol, &celt)) && celt == 1) {
        pModuleDetailList->AddModule(m_pDiaSession, pSymbol);
        pSymbol = NULL;
    }

    *ppList = pModuleDetailList;

    return S_OK;
}

HRESULT
PdbFile::GetGlobalSymbolEnum(enum SymTagEnum nSymTag, IDiaEnumSymbols **ppEnumSymbol)
{
    HRESULT hr = S_OK;
    CComPtr<IDiaSymbol> pGlobal;
    hr = m_pDiaSession->get_globalScope(&pGlobal);
    if(FAILED(hr) || NULL == pGlobal) {
        MLOG_ERROR("Pdb", "获取GlobalScope失败：Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    CComPtr<IDiaEnumSymbols> pEnumSymbols;
    hr = pGlobal->findChildren(nSymTag, NULL, nsNone, &pEnumSymbols);
    if(FAILED(hr) || NULL == pEnumSymbols) {
        MLOG_ERROR("Pdb", "遍历Symbol失败：Ret=%lu, ErrCode=%lu", hr, ::GetLastError());
        return hr;
    }

    *ppEnumSymbol = pEnumSymbols.Detach();

    return S_OK;
}
