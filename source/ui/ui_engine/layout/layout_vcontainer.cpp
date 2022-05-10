#include "stdafx.h"
#include "ui/ui_engine/layout/layout_vcontainer.h"

LayoutVContainer::LayoutVContainer()
{

}

LayoutVContainer::~LayoutVContainer()
{

}

HRESULT
LayoutVContainer::SetRowSize(DWORD nRow)
{
    return LayoutContainer::SetSlotNum(nRow);
}

HRESULT
LayoutVContainer::SetRowHeight(DWORD nRow, INT32 nHeight)
{
    return LayoutContainer::SetSlotSize(nRow, nHeight);
}

HRESULT
LayoutVContainer::SetRowWeight(DWORD nRow, DWORD nWeight)
{
    return LayoutContainer::SetSlotWeight(nRow, nWeight);
}

HRESULT
LayoutVContainer::SetChildBox(DWORD nRowId, ILayoutBox *pBox)
{
    return LayoutContainer::SetSlotBox(nRowId, pBox);
}

HRESULT
LayoutVContainer::GetRowSize(DWORD *pRow)
{
    return LayoutContainer::GetSlotSize(pRow);
}

HRESULT
LayoutVContainer::GetRowHeight(DWORD nRow, INT32 *pHeight)
{
    return LayoutVContainer::GetSlotSize(nRow, pHeight);
}

HRESULT
LayoutVContainer::GetRowWeight(DWORD nRow, DWORD *pWeight)
{
    return LayoutContainer::GetSlotWeight(nRow, pWeight);
}

HRESULT
LayoutVContainer::GetChildBox(DWORD nRowId, ILayoutBox **ppBox)
{
    return LayoutContainer::GetSlotBox(nRowId, ppBox);
}

HRESULT
LayoutVContainer::DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext)
{
    CRect oChildRect = *pUsefulRect;
    oChildRect.DeflateRect(m_oBoxPadding.left, m_oBoxPadding.top, m_oBoxPadding.right, m_oBoxPadding.bottom);

    HRESULT hr = RecaculateSlotSize(oChildRect.Height());
    if(FAILED(hr)) {
        return hr;
    }

    RECT oRect = { oChildRect.left, oChildRect.top, oChildRect.right, oChildRect.top };
    SIZE_T i = 0, nSize = m_vBoxList.size();
    for(i = 0; i < nSize; ++i) {
        oRect.bottom += m_vBoxList[i].m_nCalcSlotSize;
        LayoutChildToRect(m_vBoxList[i].m_pBox, &oRect, NULL, pContext);
        oRect.top = oRect.bottom;
    }

    return S_OK;
}