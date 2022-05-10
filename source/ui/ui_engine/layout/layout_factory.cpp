#include "stdafx.h"
#include "ui/layout.h"
#include "ui/ui_engine/layout/layout_box_base.h"
#include "ui/ui_engine/layout/layout_native_window.h"
#include "ui/ui_engine/layout/layout_hcontainer.h"
#include "ui/ui_engine/layout/layout_vcontainer.h"
#include "ui/ui_engine/layout/layout_loader.h"

HRESULT
InitLayout()
{
    if(!LayoutLoader::Get()->LoadLayoutFromResource(IDR_XML_UI, IDR_XML_RESOURCE_MAP)) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT
UninitLayout()
{
    if(!LayoutLoader::Get()->UnloadLayout()) {
        return E_FAIL;
    }

    return S_OK;
}

HRESULT CreateLayoutBox(ILayoutBox **ppContainer, DWORD nLayoutType)
{
    ASSERT_RETURN(NULL != ppContainer, E_POINTER);

    CComPtr<ILayoutBox> pContainer;
    if(FAILED(LayoutBoxBase::CreateObject(__uuidof(ILayoutBox), (void **)&pContainer)) || NULL == pContainer) {
        return E_OUTOFMEMORY;
    }

    if(0 != nLayoutType) {
        pContainer->SetLayoutType(nLayoutType);
    }

    *ppContainer = pContainer.Detach();

    return S_OK;
}

HRESULT CreateLayoutNativeWindow(ILayoutBox **ppContainer, HWND hWindow, DWORD nLayoutType)
{
    ASSERT_RETURN(NULL != hWindow, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppContainer, E_POINTER);

    CComPtr<ILayoutNativeWindow> pContainer;
    if(FAILED(LayoutNativeWindow::CreateObject(__uuidof(ILayoutNativeWindow), (void **)&pContainer)) || NULL == pContainer) {
        return E_OUTOFMEMORY;
    }

    pContainer->SetHWND(hWindow);

    CComQIPtr<ILayoutBox> pBox = pContainer;
    if(NULL == pBox) {
        return E_FAIL;
    }

    if(0 != nLayoutType) {
        pBox->SetLayoutType(nLayoutType);
    }

    *ppContainer = pBox.Detach();

    return S_OK;
}

HRESULT CreateLayoutHContainer(ILayoutHContainer **ppContainer, DWORD nLayoutType)
{
    ASSERT_RETURN(NULL != ppContainer, E_POINTER);

    CComPtr<ILayoutHContainer> pContainer;
    if(FAILED(LayoutHContainer::CreateObject(__uuidof(ILayoutHContainer), (void **)&pContainer)) || NULL == pContainer) {
        return E_OUTOFMEMORY;
    }

    CComQIPtr<ILayoutBox> pBox = pContainer;
    if(NULL == pBox) {
        return E_FAIL;
    }

    if(0 != nLayoutType) {
        pBox->SetLayoutType(nLayoutType);
    }

    *ppContainer = pContainer.Detach();

    return S_OK;
}

HRESULT CreateLayoutVContainer(ILayoutVContainer **ppContainer, DWORD nLayoutType)
{
    ASSERT_RETURN(NULL != ppContainer, E_POINTER);

    CComPtr<ILayoutVContainer> pContainer;
    if(FAILED(LayoutVContainer::CreateObject(__uuidof(ILayoutVContainer), (void **)&pContainer)) || NULL == pContainer) {
        return E_OUTOFMEMORY;
    }

    CComQIPtr<ILayoutBox> pBox = pContainer;
    if(NULL == pBox) {
        return E_FAIL;
    }

    if(0 != nLayoutType) {
        pBox->SetLayoutType(nLayoutType);
    }

    *ppContainer = pContainer.Detach();

    return S_OK;
}
