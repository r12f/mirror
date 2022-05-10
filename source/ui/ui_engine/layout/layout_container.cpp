#include "stdafx.h"
#include "ui/ui_engine/layout/layout_container.h"

#define DEFAULT_BOX_SLOT_SIZE   50

BoxSlotInfo::BoxSlotInfo()
    : m_bUseSlotSize(TRUE)
    , m_nSlotSize(0)
    , m_nSlotWeight(0)
    , m_nCalcSlotSize(0)
{

}

BoxSlotInfo::BoxSlotInfo(INT32 nSlotSize)
    : m_bUseSlotSize(TRUE)
    , m_nSlotSize(nSlotSize)
    , m_nSlotWeight(0)
    , m_nCalcSlotSize(0)
{

}

LayoutContainer::LayoutContainer()
{

}

LayoutContainer::~LayoutContainer()
{

}

HRESULT
LayoutContainer::SetSlotNum(DWORD nSlotNum)
{
    m_vBoxList.resize(nSlotNum);
    return S_OK;
}

HRESULT
LayoutContainer::SetSlotSize(DWORD nSlotId, INT32 nSlotSize)
{
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);
    m_vBoxList[nSlotId].m_bUseSlotSize = TRUE;
    m_vBoxList[nSlotId].m_nSlotSize = nSlotSize;
    m_vBoxList[nSlotId].m_nSlotWeight = 0;
    return S_OK;
}

HRESULT
LayoutContainer::SetSlotWeight(DWORD nSlotId, DWORD nSlotWeight)
{
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);
    m_vBoxList[nSlotId].m_bUseSlotSize = FALSE;
    m_vBoxList[nSlotId].m_nSlotSize = 0;
    m_vBoxList[nSlotId].m_nSlotWeight = nSlotWeight;
    return S_OK;
}

HRESULT
LayoutContainer::SetSlotBox(DWORD nSlotId, ILayoutBox *pBox)
{
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);

    CComQIPtr<ILayoutBox> pChild = m_vBoxList[nSlotId].m_pBox;
    if(NULL != pChild) {
        LayoutBoxBase::RemoveChild(pChild);
    }

    if(NULL == pBox) {
        m_vBoxList[nSlotId].m_pBox = NULL;
    } else {
        pBox->QueryInterface(__uuidof(ILayoutBoxInternal), (void **)&(m_vBoxList[nSlotId].m_pBox));
    }

    if(NULL != pBox) {
        LayoutBoxBase::AddChild(pBox);
    }

    return S_OK;
}

HRESULT
LayoutContainer::GetSlotSize(DWORD *pSlotNum)
{
    ASSERT_RETURN(NULL != pSlotNum, E_POINTER);
    *pSlotNum = (DWORD)m_vBoxList.size();
    return S_OK;
}

HRESULT
LayoutContainer::GetSlotSize(DWORD nSlotId, INT32 *pSlotSize)
{
    ASSERT_RETURN(NULL != pSlotSize, E_POINTER);
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);

    if(!m_vBoxList[nSlotId].m_bUseSlotSize) {
        *pSlotSize = 0;
        return E_FAIL;
    }

    *pSlotSize = m_vBoxList[nSlotId].m_nSlotSize;

    return S_OK;
}

HRESULT
LayoutContainer::GetSlotWeight(DWORD nSlotId, DWORD *pSlotWeight)
{
    ASSERT_RETURN(NULL != pSlotWeight, E_POINTER);
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);

    if(m_vBoxList[nSlotId].m_bUseSlotSize) {
        *pSlotWeight = 0;
        return E_FAIL;
    }

    *pSlotWeight = m_vBoxList[nSlotId].m_nSlotWeight;

    return S_OK;
}

HRESULT
LayoutContainer::GetSlotBox(DWORD nSlotId, ILayoutBox **ppBox)
{
    ASSERT_RETURN(NULL != ppBox, E_POINTER);
    ASSERT_RETURN(nSlotId < m_vBoxList.size(), E_FAIL);

    *ppBox = NULL;
    if(NULL == m_vBoxList[nSlotId].m_pBox) {
        return S_OK;
    }

    return m_vBoxList[nSlotId].m_pBox->QueryInterface(__uuidof(ILayoutBox), (void **)ppBox);
}

HRESULT
LayoutContainer::RecaculateSlotSize(INT32 nTotalSize)
{
    // Calculate fixed slot size and the total weight first
    DWORD nTotalWeight = 0;
    INT32 nUsedSize = 0;

    LayoutBoxList::iterator oBoxIt = m_vBoxList.begin();
    while(oBoxIt != m_vBoxList.end()) {
        if(oBoxIt->m_bUseSlotSize) {
            nUsedSize += oBoxIt->m_nSlotSize;
            oBoxIt->m_nCalcSlotSize = oBoxIt->m_nSlotSize;
        } else {
            nTotalWeight += oBoxIt->m_nSlotWeight;
        }
        ++oBoxIt;
    }

    if(0 == nTotalWeight) {
        return S_OK;
    }

    // Calculate the rest slots
    INT32 nRestSize = nTotalSize - nUsedSize;
    double nBoxWeight = 0;
    oBoxIt = m_vBoxList.begin();
    while(oBoxIt != m_vBoxList.end()) {
        if(!(oBoxIt->m_bUseSlotSize)) {
            nBoxWeight = oBoxIt->m_nSlotWeight * 1.0 / nTotalWeight;
            oBoxIt->m_nCalcSlotSize = (INT32)(nRestSize * nBoxWeight);
        }
        ++oBoxIt;
    }

    return S_OK;
}