#include "stdafx.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/layout/layout_internal_interfaces.h"
#include "ui/ui_engine/layout/layout_loader.h"

CLayout::CLayout(DWORD nResourceId)
    : m_nResourceId(nResourceId)
{

}

CLayout::~CLayout()
{

}

BOOL
CLayout::AddChildBox(ILayoutBox *pBox)
{
    if(NULL == m_pRootLayoutBox) {
        return FALSE;
    }

    if(FAILED(m_pRootLayoutBox->AddChild(pBox))) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CLayout::RemoveChildBox(ILayoutBox *pBox)
{
    if(NULL == m_pRootLayoutBox) {
        return FALSE;
    }

    if(FAILED(m_pRootLayoutBox->AddChild(pBox))) {
        return FALSE;
    }

    return TRUE;
}

BOOL
CLayout::Layout(BOOL bForce)
{
    if(!::IsWindow(m_hWindow)) {
        return FALSE;
    }

    CRect oClientRect;
    ::GetClientRect(m_hWindow, &oClientRect);

    if(NULL != m_pRootLayoutBox && 0 < m_pRootLayoutBox->GetChildrenCount()) {
        HRESULT hr = m_pRootLayoutBox->Layout(&oClientRect, bForce);
        if(SUCCEEDED(hr)) {
            return TRUE;
        }
    }

    return DoCustomLayout();
}

LRESULT
CLayout::OnInitDialog(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    m_hWindow = hWnd;

    LayoutLoader::Get()->CreateLayoutRoot(m_nResourceId, m_hWindow, &m_pRootLayoutBox, this);
    
    if(NULL == m_pRootLayoutBox) {
        CreateLayoutBox(&m_pRootLayoutBox);
    }

    return 1;
}

LRESULT
CLayout::OnNCDestroy(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    m_hWindow = NULL;
    m_pRootLayoutBox = NULL;
    return 1;
}

LRESULT
CLayout::OnShowWindow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;

    Layout(TRUE);

    return 1;
}

LRESULT
CLayout::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    bHandled = FALSE;
    
    if(!::IsWindowVisible(m_hWindow)) {
        Layout(TRUE);
    } else {
        Layout(FALSE);
    }

    return 1;
}
