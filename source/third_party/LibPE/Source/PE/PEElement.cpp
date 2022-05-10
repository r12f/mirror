#include "stdafx.h"
#include "PE/PEElement.h"

LIBPE_NAMESPACE_BEGIN

template <class T>
void *
PEElementT<T>::GetRawMemory()
{
    if(NULL == m_pRawBuffer) {
        LIBPE_CHK(NULL != m_pParser, NULL);
        m_pRawBuffer = m_pParser->GetRawMemory(GetRawOffset(), GetRawSize());
    }

    return m_pRawBuffer;
}

template <class T>
PEAddress
PEElementT<T>::GetRawOffset()
{
    LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
    return m_pParser->IsRawAddressVA() ? GetRVA() : GetFOA();
}

template <class T>
PEAddress
PEElementT<T>::GetRawSize()
{
    LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_SIZE);
    return m_pParser->IsRawAddressVA() ? GetSizeInMemory() : GetSizeInFile();
}

template <class T>
PEAddress
PEElementT<T>::GetRVA()
{
    if(LIBPE_INVALID_ADDRESS == m_nRVA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nRVA = m_pParser->GetRVAFromFOA(m_nFOA);
    }
    return m_nRVA;
}

template <class T>
PEAddress
PEElementT<T>::GetVA()
{
    if(LIBPE_INVALID_ADDRESS == m_nVA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nVA = m_pParser->GetVAFromRVA(m_nRVA);
    }
    return m_nVA;
}

template <class T>
PEAddress
PEElementT<T>::GetFOA()
{
    if(LIBPE_INVALID_ADDRESS == m_nFOA) {
        LIBPE_CHK(NULL != m_pParser, LIBPE_INVALID_ADDRESS);
        m_nFOA = m_pParser->GetFOAFromRVA(m_nRVA);
    }
    return m_nFOA;
}

LIBPE_FORCE_TEMPLATE_REDUCTION_CLASS(PEElementT);


LIBPE_NAMESPACE_END