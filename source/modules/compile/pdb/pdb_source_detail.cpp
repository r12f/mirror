#include "stdafx.h"
#include "modules/compile/pdb/pdb_source_detail.h"
#include "modules/compile/pdb/pdb_module_detail.h"

PdbSourceDetail::PdbSourceDetail()
    : m_nId(0)
    , m_nChecksumType(0)
{

}

PdbSourceDetail::~PdbSourceDetail()
{

}

DWORD
PdbSourceDetail::GetUniqueId()
{
    return m_nId;
}

HRESULT
PdbSourceDetail::GetPath(BSTR *pName)
{
    return m_strName.CopyTo(pName);
}

DWORD
PdbSourceDetail::GetChecksumType()
{
    return m_nChecksumType;
}

HRESULT
PdbSourceDetail::GetChecksum(BSTR *pChecksum)
{
    ASSERT_RETURN(NULL != pChecksum, E_POINTER);
    return m_strChecksum.CopyTo(pChecksum);
}

HRESULT
PdbSourceDetail::SetSource(IDiaSourceFile *pSource)
{
    ASSERT_RETURN(NULL != pSource, E_POINTER);

    pSource->get_uniqueId(&m_nId);
    pSource->get_checksumType(&m_nChecksumType);
    pSource->get_fileName(&m_strName);

    if(CHKSUM_TYPE_NONE == m_nChecksumType) {
        return S_OK;
    }

    BYTE buffer[MAX_PATH] = {0};
    DWORD nSize = sizeof(buffer);
    if(FAILED(pSource->get_checksum(nSize, &nSize, buffer))) {
        return E_FAIL;
    }

    TCHAR szCheckSum[41] = {0};
    static TCHAR szHexMap[16] = { _T('0'), _T('1'), _T('2'), _T('3'), _T('4'), _T('5'), _T('6'), _T('7'), _T('8'), _T('9'), _T('A'), _T('B'), _T('C'), _T('D'), _T('E'), _T('F') };
    for(DWORD j = 0; j < nSize; ++j) {
        szCheckSum[j * 2] = szHexMap[((buffer[j] >> 4) & 0xF)]; 
        szCheckSum[j * 2 + 1] = szHexMap[(buffer[j]) & 0x0F];
    }

    m_strChecksum = szCheckSum;

    return S_OK;
}