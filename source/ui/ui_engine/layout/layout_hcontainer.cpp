#include "stdafx.h"
#include "ui/ui_engine/layout/layout_hcontainer.h"

LayoutHContainer::LayoutHContainer()
{

}

LayoutHContainer::~LayoutHContainer()
{

}

HRESULT
LayoutHContainer::SetColSize(DWORD nCol)
{
    return LayoutContainer::SetSlotNum(nCol);
}

HRESULT
LayoutHContainer::SetColWidth(DWORD nCol, INT32 nWidth)
{
    return LayoutContainer::SetSlotSize(nCol, nWidth);
}

HRESULT
LayoutHContainer::SetColWeight(DWORD nCol, DWORD nWeight)
{
    return LayoutContainer::SetSlotWeight(nCol, nWeight);
}

HRESULT
LayoutHContainer::SetChildBox(DWORD nColId, ILayoutBox *pBox)
{
    return LayoutContainer::SetSlotBox(nColId, pBox);
}

HRESULT
LayoutHContainer::GetColSize(DWORD *pCol)
{
    return LayoutContainer::GetSlotSize(pCol);
}

HRESULT
LayoutHContainer::GetColWidth(DWORD nCol, INT32 *pWidth)
{
    return LayoutHContainer::GetSlotSize(nCol, pWidth);
}

HRESULT
LayoutHContainer::GetColWeight(DWORD nCol, DWORD *pWeight)
{
    return LayoutContainer::GetSlotWeight(nCol, pWeight);
}

HRESULT
LayoutHContainer::GetChildBox(DWORD nColId, ILayoutBox **ppBox)
{
    return LayoutContainer::GetSlotBox(nColId, ppBox);
}

HRESULT
LayoutHContainer::DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext)
{
    CRect oChildRect = *pUsefulRect;
    oChildRect.DeflateRect(m_oBoxPadding.left, m_oBoxPadding.top, m_oBoxPadding.right, m_oBoxPadding.bottom);

    HRESULT hr = RecaculateSlotSize(oChildRect.Width());
    if(FAILED(hr)) {
        return hr;
    }

    RECT oRect = { oChildRect.left, oChildRect.top, oChildRect.left, oChildRect.bottom };
    SIZE_T i = 0, nSize = m_vBoxList.size();
    for(i = 0; i < nSize; ++i) {
        oRect.right += m_vBoxList[i].m_nCalcSlotSize;
        LayoutChildToRect(m_vBoxList[i].m_pBox, &oRect, NULL, pContext);
        oRect.left = oRect.right;
    }

    return S_OK;
}