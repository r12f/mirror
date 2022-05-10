#include "stdafx.h"
#include "ui/ui_engine/layout/layout_box_base.h"

LayoutBoxBase::LayoutBoxBase()
    : m_nNativeWindowCount(0)
    , m_pParent(NULL)
    , m_nLayoutType(MLT_LEFT | MLT_TOP | MLT_HFILL | MLT_VFILL)
{
    m_oBoxMaxSize.SetSize(INT_MAX, INT_MAX);
}

LayoutBoxBase::~LayoutBoxBase()
{

}

HRESULT
LayoutBoxBase::AddChild(ILayoutBox *pBox)
{
    ASSERT_RETURN(NULL != pBox, E_POINTER);

    CComQIPtr<ILayoutBoxInternal> pBoxInternal = pBox;
    if(NULL == pBoxInternal) {
        return E_FAIL;
    }

    AddNativeWindowCount(pBoxInternal->GetNativeWindowCount());

    pBoxInternal->SetParent(this);

    m_vChildren.push_back(pBoxInternal);

    return S_OK;
}

HRESULT
LayoutBoxBase::RemoveChild(ILayoutBox *pBox)
{
    ASSERT_RETURN(NULL != pBox, E_POINTER);
    
    CComQIPtr<ILayoutBoxInternal> pBoxInternal = pBox;
    if(NULL == pBoxInternal) {
        return E_FAIL;
    }

    ChildrenList::iterator oChildIt = m_vChildren.begin();
    while(oChildIt != m_vChildren.end()) {
        if((*oChildIt) == pBoxInternal) {
            pBoxInternal->SetParent(NULL);
            RemoveNativeWindowCount(pBoxInternal->GetNativeWindowCount());
            m_vChildren.erase(oChildIt);
            return S_OK;
        }

        ++oChildIt;
    }

    return S_OK;
}

DWORD
LayoutBoxBase::GetChildrenCount()
{
    return (DWORD)m_vChildren.size();
}

HRESULT
LayoutBoxBase::GetChildren(ILayoutBox **ppBox, DWORD nCount)
{
    ASSERT_RETURN(NULL != ppBox, E_POINTER);
    
    DWORD i = 0;
    ChildrenList::iterator oChildIt = m_vChildren.begin();
    while(i < nCount && oChildIt != m_vChildren.end()) {
        (*oChildIt)->QueryInterface(__uuidof(ILayoutBox), (void **)(ppBox[i]));
        ++i;
        ++oChildIt;
    }

    return S_OK;
}

HRESULT
LayoutBoxBase::GetParent(ILayoutBox **ppBox)
{
    ASSERT_RETURN(NULL != ppBox, E_POINTER);

    *ppBox = NULL;
    if(NULL == m_pParent) {
        return S_OK;
    }

    return m_pParent->QueryInterface(__uuidof(ILayoutBox), (void **)ppBox);
}

HRESULT
LayoutBoxBase::GetRoot(ILayoutBox **ppBox)
{
    ASSERT_RETURN(NULL != ppBox, E_POINTER);

    CComPtr<ILayoutBox> pParent, pNextParent;
    
    if(NULL != m_pParent) {
         m_pParent->QueryInterface(__uuidof(ILayoutBox), (void **)&pNextParent);
    }

    while(NULL != pNextParent) {
        pParent = pNextParent;
        pParent->GetParent(&pNextParent);
    }

    *ppBox = pParent.Detach();

    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutType(DWORD nLayout)
{
    m_nLayoutType = nLayout;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutX(INT32 nX)
{
    m_oBoxPosition.x = nX;
    m_nLayoutType &= ~MLT_HMASK;
    m_nLayoutType |= MLT_REL_X;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutY(INT32 nY)
{
    m_oBoxPosition.y = nY;
    m_nLayoutType &= ~MLT_VMASK;
    m_nLayoutType |= MLT_REL_Y;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutPosition(POINT *pPosition)
{
    ASSERT_RETURN(NULL != pPosition, E_POINTER);

    m_oBoxPosition = *pPosition;
    m_nLayoutType &= ~MLT_HMASK;
    m_nLayoutType &= ~MLT_VMASK;
    m_nLayoutType |= MLT_REL_X;
    m_nLayoutType |= MLT_REL_Y;

    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutWidth(INT32 nWidth)
{
    m_oBoxSize.cx = nWidth;
    m_nLayoutType &= ~MLT_HFILL;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutHeight(INT32 nHeight)
{
    m_oBoxSize.cy = nHeight;
    m_nLayoutType &= ~MLT_VFILL;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);

    m_oBoxSize = *pSize;
    m_nLayoutType &= ~(MLT_HFILL & MLT_VFILL);

    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutMinSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    m_oBoxMaxSize = *pSize;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutMaxSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    m_oBoxMinSize = *pSize;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutMargin(RECT *pRect)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    m_oBoxMargin = *pRect;
    return S_OK;
}

HRESULT
LayoutBoxBase::SetLayoutPadding(RECT *pRect)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    m_oBoxPadding = *pRect;
    return S_OK;
}

DWORD
LayoutBoxBase::GetLayoutType()
{
    return m_nLayoutType;
}

INT32
LayoutBoxBase::GetLayoutX()
{
    return m_oBoxPosition.x;
}

INT32
LayoutBoxBase::GetLayoutY()
{
    return m_oBoxPosition.y;
}

HRESULT
LayoutBoxBase::GetLayoutPosition(POINT *pPosition)
{
    ASSERT_RETURN(NULL != pPosition, E_POINTER);
    *pPosition = m_oBoxPosition;
    return S_OK;
}

INT32
LayoutBoxBase::GetLayoutWidth()
{
    return m_oBoxSize.cx;
}

INT32
LayoutBoxBase::GetLayoutHeight()
{
    return m_oBoxSize.cy;
}

HRESULT
LayoutBoxBase::GetLayoutSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    *pSize = m_oBoxSize;
    return S_OK;
}

HRESULT
LayoutBoxBase::GetLayoutMinSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    *pSize = m_oBoxMinSize;
    return S_OK;
}

HRESULT
LayoutBoxBase::GetLayoutMaxSize(SIZE *pSize)
{
    ASSERT_RETURN(NULL != pSize, E_POINTER);
    *pSize = m_oBoxMaxSize;
    return S_OK;
}


HRESULT
LayoutBoxBase::GetLayoutMargin(RECT *pRect)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    *pRect = m_oBoxMargin;
    return S_OK;
}

HRESULT
LayoutBoxBase::GetLayoutPadding(RECT *pRect)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    *pRect = m_oBoxPadding;
    return S_OK;
}

BOOL
LayoutBoxBase::IsBoxVisible()
{
    return TRUE;
}

HRESULT
LayoutBoxBase::GetBoxClientRect(RECT *pRect)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    *pRect = m_oBoxClientRect;
    return S_OK;
}

HRESULT
LayoutBoxBase::Layout(RECT *pRect, BOOL bForce)
{
    ASSERT_RETURN(NULL != pRect, E_POINTER);
    
    // Layout relate children
    LayoutContext oContext = {0};
    oContext.m_bForce = bForce;
    CreateLayoutContext(&oContext);

    HRESULT hr = LayoutRelatedChildren(pRect, &oContext);

    EndLayoutRelateChildren(&oContext);
    
    DWORD i = 0;
    for(i = 0; i < oContext.m_nAffectedNum; ++i) {
        oContext.m_pAffected[i]->LayoutChildrenNoSelf(bForce);
    }

    DestroyLayoutContext(&oContext);

    return hr;
}

HRESULT
LayoutBoxBase::SetParent(ILayoutBoxInternal *pParent)
{
    m_pParent = pParent;
    return S_OK;
}

HRESULT
LayoutBoxBase::AddNativeWindowCount(DWORD nChildCount)
{
    CComQIPtr<ILayoutBoxInternal> pParent = m_pParent;
    if(NULL != pParent) {
        pParent->AddNativeWindowCount(nChildCount);
    }

    m_nNativeWindowCount += nChildCount;

    return S_OK;
}

HRESULT
LayoutBoxBase::RemoveNativeWindowCount(DWORD nChildCount)
{
    CComQIPtr<ILayoutBoxInternal> pParent = m_pParent;
    if(NULL != pParent) {
        pParent->RemoveNativeWindowCount(nChildCount);
    }

    m_nNativeWindowCount -= nChildCount;

    return S_OK;
}

DWORD
LayoutBoxBase::GetNativeWindowCount()
{
    return m_nNativeWindowCount;
}

HRESULT
LayoutBoxBase::LayoutRelatedChildren(RECT *pUsefulRect, LayoutContext *pContext)
{
    if(!(pContext->m_bForce)) {
        if(!IsBoxVisible()) {
            return S_OK;
        }

        // 大小一样，则无需调整
        if(m_oBoxClientRect.EqualRect(pUsefulRect)) {
            return S_OK;
        }
    }

    // Margin
    CRect oBoxRect = *pUsefulRect;
    oBoxRect.DeflateRect(m_oBoxMargin.left, m_oBoxMargin.top, m_oBoxMargin.right, m_oBoxMargin.bottom);

    DoLayoutRelativeChildren(&oBoxRect, pContext);

    m_oBoxClientRect = *pUsefulRect;

    return S_OK;
}

HRESULT
LayoutBoxBase::LayoutChildrenNoSelf(BOOL bForce)
{
    ASSERT_RETURN(FALSE, E_NOTIMPL);
}

void
LayoutBoxBase::CreateLayoutContext(LayoutContext *pContext)
{
    DWORD nNativeChildrenNum = m_nNativeWindowCount - 1;
    if(0 == nNativeChildrenNum) {
        return;
    }

//     pContext->m_hDWP = ::BeginDeferWindowPos(nNativeChildrenNum);
//     pContext->m_pAffected = new ILayoutBoxInternal *[nNativeChildrenNum];
//     pContext->m_nAffectedNum = 0;

    return;
}

void
LayoutBoxBase::EndLayoutRelateChildren(LayoutContext *pContext)
{
    if(NULL != pContext->m_hDWP) {
        ::EndDeferWindowPos(pContext->m_hDWP);
    }
}

void
LayoutBoxBase::DestroyLayoutContext(LayoutContext *pContext)
{
    if(NULL != pContext->m_pAffected) {
        DWORD i = 0;
        for(i = 0; i < pContext->m_nAffectedNum; ++i) {
            pContext->m_pAffected[i]->Release();
        }

        delete pContext->m_pAffected;
    }

    return;
}

HRESULT
LayoutBoxBase::DoLayoutRelativeChildren(RECT *pUsefulRect, LayoutContext *pContext)
{
    // Calculate all the boxes
    CRect oChildRect = *pUsefulRect;
    oChildRect.DeflateRect(m_oBoxPadding.left, m_oBoxPadding.top, m_oBoxPadding.right, m_oBoxPadding.bottom);
    return LayoutChildrenToRect(m_vChildren, &oChildRect, pContext);
}

HRESULT
LayoutBoxBase::LayoutChildToRect(ILayoutBoxInternal *pBox, RECT *pRect, RECT *pChildRect, LayoutContext *pContext)
{
    if(NULL != pChildRect) {
        pChildRect->left = pChildRect->top = pChildRect->right = pChildRect->bottom = 0;
    }

    if(NULL == pBox) {
        return E_POINTER;
    }

    DWORD nLayoutType = pBox->GetLayoutType();

    // 估计需要的宽高
    CSize oBoxSize;

    // 对于Fill使用贪心的策略，先来的先占满
    // 计算宽度
    if(nLayoutType & MLT_HFILL) {
        oBoxSize.cx = pRect->right - pRect->left;
    } else {
        oBoxSize.cx = pBox->GetLayoutWidth();
    }

    // 计算高度
    if(nLayoutType & MLT_VFILL) {
        oBoxSize.cy = pRect->bottom - pRect->top;
    } else {
        oBoxSize.cy = pBox->GetLayoutHeight();
    }
    
    // 计算Rect
    CRect oBoxRect;

    // 计算X轴占位
    switch(nLayoutType & MLT_HMASK) {
    case MLT_LEFT:
        oBoxRect.left = pRect->left;
        oBoxRect.right = oBoxRect.left + oBoxSize.cx;
        break;
    case MLT_RIGHT:
        oBoxRect.right = pRect->right;
        oBoxRect.left = oBoxRect.right - oBoxSize.cx;
        break;
    case MLT_HCENTER:
        oBoxRect.left = (pRect->left + pRect->right) / 2 - oBoxSize.cx / 2;
        oBoxRect.right = (pRect->left + pRect->right) / 2 + oBoxSize.cx / 2;
        break;
    case MLT_REL_X:
        oBoxRect.left = pRect->left + pBox->GetLayoutX();
        oBoxRect.right = oBoxRect.left + oBoxSize.cx;
        break;
    }

    // 对于FILL类型的布局，需要判断其是否超出界限，如果超出，则需要拉回来
    if(nLayoutType & MLT_HFILL) {
        if(oBoxRect.Width() > pRect->right - pRect->left) {
            oBoxRect.right = oBoxRect.left + pRect->right - pRect->left;
        }
    }

    // 计算Y轴占位
    switch(nLayoutType & MLT_VMASK) {
    case MLT_TOP:
        oBoxRect.top = pRect->top;
        oBoxRect.bottom = oBoxRect.top + oBoxSize.cy;
        break;
    case MLT_BOTTOM:
        oBoxRect.bottom = pRect->bottom;
        oBoxRect.top = oBoxRect.bottom - oBoxSize.cy;
        break;
    case MLT_VCENTER:
        oBoxRect.top = (pRect->top + pRect->bottom) / 2 - oBoxSize.cy / 2;
        oBoxRect.bottom = (pRect->top + pRect->bottom) / 2 + oBoxSize.cy / 2;
        break;
    case MLT_REL_Y:
        oBoxRect.top = pRect->top + pBox->GetLayoutY();
        oBoxRect.bottom = oBoxRect.top + oBoxSize.cy;
        break;
    }

    // 对于FILL类型的布局，需要判断其是否超出界限，如果超出，则需要拉回来
    if(nLayoutType & MLT_VFILL) {
        if(oBoxRect.Height() > pRect->bottom - pRect->top) {
            oBoxRect.bottom = oBoxRect.top + pRect->bottom - pRect->top;
        }
    }

    // 最后需要判断该元素是否过小或者过大，如果小于其最小或最大大小，需要修正
    // 由于不是所有的box都是原生窗口，所以如果是虚box下套着原生窗口，那么最小
    // 值的设置可能导致子box蔓延到父box之外。
    CSize oMinSize;
    pBox->GetLayoutMinSize(&oMinSize);

    if(oBoxRect.Width() < oMinSize.cx) {
        oBoxRect.right = oBoxRect.left + oMinSize.cx;
    }

    if(oBoxRect.Height() < oMinSize.cy) {
        oBoxRect.bottom = oBoxRect.top + oMinSize.cy;
    }

    CSize oMaxSize;
    pBox->GetLayoutMaxSize(&oMaxSize);

    if(oBoxRect.Width() > oMaxSize.cx) {
        oBoxRect.right = oBoxRect.left + oMaxSize.cx;
    }

    if(oBoxRect.Height() > oMaxSize.cy) {
        oBoxRect.bottom = oBoxRect.top + oMaxSize.cy;
    }

    if(NULL != pChildRect) {
        *pChildRect = oBoxRect;
    }

    // Child的Margin不需要父来管理，在Child接受到他所在的区域之后，会自己减去Margin
    pBox->LayoutRelatedChildren(oBoxRect, pContext);

    return S_OK;
}

HRESULT
LayoutBoxBase::LayoutChildrenToRect(ChildrenList &vChildren, RECT *pRect, LayoutContext *pContext)
{
    if(vChildren.size() == 0) {
        return S_OK;
    }

    CRect oBoxRect = *pRect, oChildRect, oTempRect;

    DWORD nChildLayoutHType = 0, nChildLayoutVType;
    ChildrenList vLayoutChildren = vChildren;
    ChildrenList::iterator oChildIt;

    // 1.先对左右上下自动排布的元素进行排布，排布使用贪心的原则
    for(oChildIt = vLayoutChildren.begin(); oChildIt != vLayoutChildren.end();) {
        nChildLayoutHType = ((*oChildIt)->GetLayoutType() & MLT_HMASK);
        nChildLayoutVType = ((*oChildIt)->GetLayoutType() & MLT_VMASK);
        if(nChildLayoutHType == MLT_LEFT || nChildLayoutHType == MLT_RIGHT ||  nChildLayoutVType == MLT_TOP ||  nChildLayoutVType == MLT_BOTTOM) {
            LayoutChildToRect((*oChildIt), &oBoxRect, &oChildRect, pContext);
            oTempRect.SubtractRect(&oBoxRect, &oChildRect);
            oBoxRect = oTempRect;
            oChildIt = vLayoutChildren.erase(oChildIt);
            continue;
        }

        ++oChildIt;
    }

    // 2.对居中的元素进行排布
    for(oChildIt = vLayoutChildren.begin(); oChildIt != vLayoutChildren.end();) {
        nChildLayoutHType = ((*oChildIt)->GetLayoutType() & MLT_HMASK);
        nChildLayoutVType = ((*oChildIt)->GetLayoutType() & MLT_VMASK);
        if(nChildLayoutHType == MLT_VCENTER || nChildLayoutHType == MLT_HCENTER) {
            LayoutChildToRect((*oChildIt), &oBoxRect, &oChildRect, pContext);
            oTempRect.SubtractRect(&oBoxRect, &oChildRect);
            oBoxRect = oTempRect;
            oChildIt = vLayoutChildren.erase(oChildIt);
            continue;
        }

        ++oChildIt;
    }

    // 3.对剩下的元素（绝对定位的元素）进行排布
    oBoxRect = *pRect;  // 还原可用区域
    for(oChildIt = vLayoutChildren.begin(); oChildIt != vLayoutChildren.end(); ++oChildIt) {
        LayoutChildToRect((*oChildIt), &oBoxRect, &oChildRect, pContext);
    }

    return S_OK;
}
