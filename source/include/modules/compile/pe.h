#pragma once

// PE element type
enum {
    PEE_SECTION                 = 0,
    PEE_RESOURCE,
    PEE_IMPORT_ADDRESS,
    PEE_IMPORT_ADDRESS_TABLE,
    PEE_EXPORT_ADDRESS,
    PEE_EXPORT_ADDRESS_TABLE,
    PEE_PROPERTY,
};

// PE visitor type
enum {
    PEV_SECTION                 = 0,
    PEV_DATA_DIR,
    PEV_RESOURCE,
    PEV_IMPORT_ADDRESS,
    PEV_EXPORT_ADDRESS,
    PEV_PROPERTY,
};


interface IPeFile;
interface IPeElementBase;
interface IPeSection;
interface IPeDataDir;
interface IPeResource;
interface IPeImportFunction;
interface IPeImportDll;
interface IPeIAT;
interface IPeExportFunction;
interface IPeEAT;
interface IPeProperty;
interface IPeVisitor;

// {F5219FE4-E354-48dc-89D2-F6B65731829F}
MIDL_INTERFACE("F5219FE4-E354-48dc-89D2-F6B65731829F")
IPeFile : IUnknown
{
public:
    // Load & Rebuild
    virtual HRESULT STDMETHODCALLTYPE Load(BSTR strFile) = 0;
    virtual HRESULT STDMETHODCALLTYPE Rebuild(BSTR strFile) = 0;

    // Basic info
    virtual BOOL STDMETHODCALLTYPE IsX86File() = 0;
    virtual IMAGE_DOS_HEADER * STDMETHODCALLTYPE GetDosHeader() = 0;
    virtual IMAGE_FILE_HEADER * STDMETHODCALLTYPE GetFileHeader() = 0;
    virtual IMAGE_NT_HEADERS32 * STDMETHODCALLTYPE GetNtHeader32() = 0;
    virtual IMAGE_NT_HEADERS64 * STDMETHODCALLTYPE GetNtHeader64() = 0;
    virtual IMAGE_DATA_DIRECTORY * STDMETHODCALLTYPE GetDataDirEntry() = 0;

    // Section
    virtual DWORD STDMETHODCALLTYPE GetSectionNum() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSection(DWORD nIndex, IPeSection **ppSection) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetSectionVisitor(IPeVisitor **ppVisitor) = 0;

    // Data directory
    virtual HRESULT STDMETHODCALLTYPE GetDataDir(DWORD nIndex, IPeDataDir **ppDataDir) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetDataDirVisitor(IPeVisitor **ppVisitor) = 0;

    // Resource
    virtual HRESULT STDMETHODCALLTYPE GetResourceVisitor(IPeVisitor **ppVisitor) = 0;

    // Import address table
    virtual HRESULT STDMETHODCALLTYPE GetIAT(IPeIAT **ppIAT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetIATVisitor(IPeVisitor **ppVisitor) = 0;

    // Export address table
    virtual HRESULT STDMETHODCALLTYPE GetEAT(IPeEAT **ppEAT) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetEATVisitor(IPeVisitor **ppVisitor) = 0;

    // Property
    virtual HRESULT STDMETHODCALLTYPE GetPropertyVisitor(IPeVisitor **ppVisitor) = 0;

    // Address convert tools
    virtual ULONGLONG STDMETHODCALLTYPE GetRVAFromRaw(ULONGLONG nRaw) = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetRawFromRVA(ULONGLONG nRVA) = 0;
};

// {9B19D962-B80C-4380-A74A-BF8E584AB401}
MIDL_INTERFACE("9B19D962-B80C-4380-A74A-BF8E584AB401")
IPeElementBase : IUnknown
{
public:
    virtual DWORD STDMETHODCALLTYPE GetType() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetName(BSTR *pName) = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetRaw() = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetRawSize() = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetRVA() = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetVA() = 0;
    virtual ULONGLONG STDMETHODCALLTYPE GetVirtualSize() = 0;
    virtual HRESULT STDMETHODCALLTYPE DumpElement(BYTE *pData, DWORD nMaxDataSize) = 0;
};

// {4EBFA897-205D-46b5-86DD-2AD2EE46E773}
MIDL_INTERFACE("4EBFA897-205D-46b5-86DD-2AD2EE46E773")
IPeSection : IPeElementBase
{
public:
};

// {51C32B0E-58CC-4b8a-A4DB-54D39399E8F8}
MIDL_INTERFACE("51C32B0E-58CC-4b8a-A4DB-54D39399E8F8")
IPeDataDir : IPeElementBase
{
public:
};

// {E70AFFD0-1D4F-4f09-897A-ACBF12478C3F}
MIDL_INTERFACE("E70AFFD0-1D4F-4f09-897A-ACBF12478C3F")
IPeResource : IPeElementBase
{
public:
    virtual DWORD STDMETHODCALLTYPE GetResourceId() = 0;
};

// {D42FA6EE-D26E-4e6d-B79B-B63866CDF390}
MIDL_INTERFACE("D42FA6EE-D26E-4e6d-B79B-B63866CDF390")
IPeImportFunction : IPeElementBase
{
public:
    virtual BOOL STDMETHODCALLTYPE IsImportByName() = 0;
    virtual DWORD STDMETHODCALLTYPE GetIndex() = 0;
    virtual WORD STDMETHODCALLTYPE GetHint() = 0;
    virtual ULONGLONG GetImageThunkData() = 0;
    virtual IMAGE_THUNK_DATA32 * GetImageThunkData32() = 0;
    virtual IMAGE_THUNK_DATA64 * GetImageThunkData64() = 0;
};

// {CADCAD2C-CD5C-4109-A42D-F5FBDCFF1F7E}
MIDL_INTERFACE("CADCAD2C-CD5C-4109-A42D-F5FBDCFF1F7E")
IPeImportDll : IPeElementBase
{
public:
    virtual DWORD STDMETHODCALLTYPE GetDllIndex() = 0;
    virtual IMAGE_IMPORT_DESCRIPTOR * GetImportDescriptor() = 0;
    virtual DWORD STDMETHODCALLTYPE GetImportFunctionCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImportFunction(DWORD nIndex, IPeImportFunction **ppFunction) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImportFunctionByName(BSTR strFuncName, IPeImportFunction **ppFunction) = 0;
};

// {D923EE2C-70C1-491e-A600-63ADB832807A}
MIDL_INTERFACE("D923EE2C-70C1-491e-A600-63ADB832807A")
IPeIAT : IPeDataDir
{
public:
    virtual DWORD STDMETHODCALLTYPE GetImportDllCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImportDll(DWORD nIndex, IPeImportDll **ppDll) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetImportDllByName(BSTR strDllName, IPeImportDll **ppDll) = 0;
};

// {B7C698B4-A081-4368-A6BA-8DDF89004D2F}
MIDL_INTERFACE("B7C698B4-A081-4368-A6BA-8DDF89004D2F")
IPeExportFunction : IPeElementBase
{
public:
    virtual BOOL STDMETHODCALLTYPE IsExportByName() = 0;
    virtual DWORD STDMETHODCALLTYPE GetIndex() = 0;
};

// {C16D1B32-5634-4008-9372-D1681E2531EB}
MIDL_INTERFACE("C16D1B32-5634-4008-9372-D1681E2531EB")
IPeEAT : IPeDataDir
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetExportFunctionCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE GetExportFunction(DWORD nIndex, IPeExportFunction **ppFunction) = 0;
};

// {0BE004E2-3CAE-41df-8E40-DEF62C4159A9}
MIDL_INTERFACE("0BE004E2-3CAE-41df-8E40-DEF62C4159A9")
IPeProperty : IPeElementBase
{
public:
};

// {FF5B5CA6-3416-43c9-A1C5-EB471F7908D1}
MIDL_INTERFACE("FF5B5CA6-3416-43c9-A1C5-EB471F7908D1")
IPeVisitor : IUnknown
{
public:
    virtual DWORD STDMETHODCALLTYPE GetType() = 0;
    virtual HRESULT STDMETHODCALLTYPE Get(IPeElementBase **ppElement) = 0;
    virtual HRESULT STDMETHODCALLTYPE Next() = 0;
    virtual HRESULT STDMETHODCALLTYPE Reset() = 0;
};