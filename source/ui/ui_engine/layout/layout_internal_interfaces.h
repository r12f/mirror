#pragma once

#include "ui/layout.h"

interface ILayoutBoxInternal;

struct LayoutContext {
    BOOL m_bForce;
    HDWP m_hDWP;
    ILayoutBoxInternal **m_pAffected;
    DWORD m_nAffectedNum;
};

// {9B4866C0-3B7A-4011-B178-81892A01FC69}
MIDL_INTERFACE("9B4866C0-3B7A-4011-B178-81892A01FC69")
ILayoutBoxInternal : ILayoutBox
{
public:
    // Hierarchy
    virtual HRESULT STDMETHODCALLTYPE SetParent(ILayoutBoxInternal *pParent) = 0;

    // Native children for BeginDeferWindowPos
    virtual HRESULT STDMETHODCALLTYPE AddNativeWindowCount(DWORD nChildCount) = 0;
    virtual HRESULT STDMETHODCALLTYPE RemoveNativeWindowCount(DWORD nChildCount) = 0;
    virtual DWORD STDMETHODCALLTYPE GetNativeWindowCount() = 0;
    virtual HRESULT STDMETHODCALLTYPE LayoutRelatedChildren(RECT *pUsefulRect, LayoutContext *pContext) = 0;
    virtual HRESULT STDMETHODCALLTYPE LayoutChildrenNoSelf(BOOL bForce) = 0;
};