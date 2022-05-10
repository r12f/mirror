#include "stdafx.h"
#include "modules/compile/module_compile.h"
#include "modules/framework.h"
#include "modules/compile/pdb/pdb_file.h"

MirModuleCompile::MirModuleCompile()
{

}

MirModuleCompile::~MirModuleCompile()
{

}

STDMETHODIMP
MirModuleCompile::Create()
{
    return S_OK;
}

STDMETHODIMP
MirModuleCompile::Destroy()
{
    return S_OK;
}

STDMETHODIMP
MirModuleCompile::Init()
{
    return S_OK;
}

STDMETHODIMP
MirModuleCompile::Uninit()
{
    return S_OK;
}

STDMETHODIMP
MirModuleCompile::RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI)
{
    ASSERT_RETURN(NULL != pTitle, E_INVALIDARG);
    ASSERT_RETURN(NULL != pUI, E_INVALIDARG);

    // ��������
    m_oDlgCompile.Create(hMainWnd);
    ASSERT_RETURN(m_oDlgCompile.IsWindow(), E_FAIL);

    *pTitle = CComBSTR(L"�������").Detach();
    *pUI = m_oDlgCompile.m_hWnd;

    return S_OK;
}

HRESULT
MirModuleCompile::CreateInstance(REFGUID pGUID, void **pObject)
{
    ASSERT_RETURN(NULL != pObject, E_POINTER);

    if(IsEqualGUID(pGUID, __uuidof(IPdbFile))) {
        return PdbFile::CreateObject(__uuidof(IPdbFile), pObject);
    }

    return E_NOINTERFACE;
}