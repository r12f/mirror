#pragma once

// Debug Interface Access (DIA) specific
#include "cvconst.h"
#include "dia2.h"

interface IPdbFile;
interface IPdbCodeAnalyzer;
interface IPdbCodeDetail;
interface IPdbModuleDetail;
interface IPdbModuleDetailList;

// {D4445698-BDD1-4553-9512-AECD56ED4EC4}
MIDL_INTERFACE("D4445698-BDD1-4553-9512-AECD56ED4EC4")
IPdbFile : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE LoadPdb(BSTR pFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCodeDetail(BOOL bMergeTemplate, IPdbCodeDetail **ppCodeDetail) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModuleDetailList(IPdbModuleDetailList **ppList) = 0;
};

#define PCD_SORT_BY_NAME        1
#define PCD_SORT_BY_SIZE        2

// {9AA5F812-14E9-46b1-9E85-49A83BDFF97B}
MIDL_INTERFACE("09CD26A3-E007-47ed-9B04-F11AF693AB9E")
IPdbSymbolDetail : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetName(BSTR *pName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSize(ULONGLONG *pSize) = 0;
    virtual HRESULT STDMETHODCALLTYPE Sort(DWORD nSortType) = 0;
};

// {09CD26A3-E007-47ed-9B04-F11AF693AB9E}
MIDL_INTERFACE("09CD26A3-E007-47ed-9B04-F11AF693AB9E")
IPdbCodeDetail : IPdbSymbolDetail
{
public:
    virtual DWORD STDMETHODCALLTYPE GetOwnedCodeCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetOwnedCodes(IPdbCodeDetail **ppDetails, DWORD nMaxCodeCount) = 0;
};

// {11E47185-702B-4b94-9CA4-A373C47DC799}
MIDL_INTERFACE("11E47185-702B-4b94-9CA4-A373C47DC799")
IPdbSourceDetail : IUnknown
{
public:
    // Override IPdbSourceDetail
    virtual DWORD STDMETHODCALLTYPE GetUniqueId() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetPath(BSTR *pName) = 0;
    virtual DWORD STDMETHODCALLTYPE GetChecksumType() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetChecksum(BSTR *pChecksum) = 0;
};

// {D1578D01-CDC7-40fa-ABC3-754A6F3CFF3F}
MIDL_INTERFACE("D1578D01-CDC7-40fa-ABC3-754A6F3CFF3F")
IPdbModuleDetail : IPdbSymbolDetail
{
public:
    virtual DWORD STDMETHODCALLTYPE GetRelatedFileCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetRelatedFiles(IPdbSourceDetail **ppFiles, DWORD nMaxFileCount) = 0;
};

// {143496DE-C284-4a74-9F32-D85A027403B0}
MIDL_INTERFACE("143496DE-C284-4a74-9F32-D85A027403B0")
IPdbModuleDetailList : IPdbSymbolDetail
{
public:
    virtual DWORD STDMETHODCALLTYPE GetModuleCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetModules(IPdbModuleDetail **pDetail, DWORD nMaxDetailCount) = 0;
};