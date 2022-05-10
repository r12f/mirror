#include "stdafx.h"
#include "base/string_number_convertions.h"
#include "ui/ui_engine/layout/layout_root_creator.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/layout/layout_native_window.h"
#include "ui/ui_engine/layout/layout_box_root.h"

LayoutRootCreator::LayoutRootCreator(IdMap &vIdMap, CLayout *pLayout)
    : m_vIdMap(vIdMap)
    , m_pLayout(pLayout)
{

}

LayoutRootCreator::~LayoutRootCreator()
{

}

HRESULT
LayoutRootCreator::Run(TiXmlElement *pElement, HWND hWnd, ILayoutBox **ppBox)
{
    m_vRootWndChain.push_back(hWnd);
    m_vParentWndChain.push_back(hWnd);

    CComPtr<ILayoutBox> pBox;
    if(FAILED(LayoutBoxRoot::CreateObject(__uuidof(ILayoutBox), (void **)&pBox)) || NULL == pBox) {
        return E_OUTOFMEMORY;
    }

    CComQIPtr<ILayoutNativeWindow> pNativeRoot = pBox;
    if(NULL == pNativeRoot) {
        return E_FAIL;
    }

    pNativeRoot->SetHWND(hWnd);

    if(!ParseBoxCommon(pElement, pBox)) {
        return E_FAIL;
    }

    *ppBox = pBox.Detach();

    m_vRootWndChain.pop_back();
    m_vParentWndChain.pop_back();

    return S_OK;
}

BOOL
LayoutRootCreator::ParseUILayoutBox(TiXmlElement *pElement, ILayoutBox **ppBox)
{
    CStringA strType = pElement->Attribute("Type");
    if(strType == "NativeWindow") {
        return ParseUILayoutNativeWindow(pElement, ppBox);
    } else if(strType == "VContainer") {
        return ParseUILayoutVContainer(pElement, ppBox);
    } else if(strType == "HContainer") {
        return ParseUILayoutHContainer(pElement, ppBox);
    }
    return ParseUILayoutBasic(pElement, ppBox);
}

BOOL
LayoutRootCreator::ParseUILayoutBasic(TiXmlElement *pElement, ILayoutBox **ppBox)
{
    CComPtr<ILayoutBox> pBox;
    if(FAILED(CreateLayoutBox(&pBox)) || NULL == pBox) {
        return FALSE;
    }

    if(!ParseBoxCommon(pElement, pBox)) {
        return FALSE;
    }

    *ppBox = pBox.Detach();

    return TRUE;
}

BOOL
LayoutRootCreator::ParseUILayoutNativeWindow(TiXmlElement *pElement, ILayoutBox **ppBox)
{
    CComPtr<ILayoutNativeWindow> pNativeWindow;
    if(FAILED(LayoutNativeWindow::CreateObject(__uuidof(ILayoutNativeWindow), (void **)&pNativeWindow)) || NULL == pNativeWindow) {
        return FALSE;
    }

    HWND hWnd = NULL, hRootWnd = m_vRootWndChain.back(), hParentWnd = m_vParentWndChain.back();
    DWORD nId = 0;
    CStringA strId = pElement->Attribute("Id");
    if(MapIdStrToDWord(strId, nId)) {
        hWnd = ::GetDlgItem(hRootWnd, nId);
    }

    if(NULL == hWnd && NULL != m_pLayout) {
        hWnd = m_pLayout->GetNativeWindowFromResId(strId);
    }

    ATLASSERT(NULL != hWnd);
    pNativeWindow->SetHWND(hWnd);

    BOOL bIsWindowDialog = FALSE;
    {
        TCHAR szClassName[MAX_PATH] = {0};
        ::GetClassName(hWnd, szClassName, MAX_PATH);
        if(0 == _tcsncmp(szClassName, _T("#32770"), 7)) {
            bIsWindowDialog = TRUE;
        }
    }

    if(bIsWindowDialog) {
        m_vRootWndChain.push_back(hWnd);
    }
    m_vParentWndChain.push_back(hWnd);

    CComQIPtr<ILayoutBox> pBox = pNativeWindow;
    if(NULL == pBox) {
        return FALSE;
    }

    if(!ParseBoxCommon(pElement, pBox)) {
        return FALSE;
    }

    if(bIsWindowDialog) {
        m_vRootWndChain.pop_back();
    }
    m_vParentWndChain.pop_back();

    *ppBox = pBox.Detach();

    return TRUE;
}

BOOL
LayoutRootCreator::ParseUILayoutVContainer(TiXmlElement *pElement, ILayoutBox **ppBox)
{
    CComPtr<ILayoutVContainer> pContainer;
    if(FAILED(CreateLayoutVContainer(&pContainer)) || NULL == pContainer) {
        return FALSE;
    }

    CComQIPtr<ILayoutBox> pBox = pContainer;
    if(NULL == pBox) {
        return FALSE;
    }

    // Common
    if(!ParseBoxCommon(pElement, pBox, FALSE)) {
        return FALSE;
    }

    // Need size
    DWORD nRowCount = 0;
    TiXmlElement *pChild = pElement->FirstChildElement("Row");
    while(NULL != pChild) {
        ++nRowCount;
        pChild = pChild->NextSiblingElement("Row");
    }

    pContainer->SetRowSize(nRowCount);

    DWORD nRowId = 0;
    CComPtr<ILayoutBox> pRowBox;
    pChild = pElement->FirstChildElement("Row");
    while(NULL != pChild) {
        pRowBox = NULL;
        if(FAILED(CreateLayoutBox(&pRowBox)) || NULL == pRowBox) {
            return FALSE;
        }

        if(!ParseUILayoutChildBoxes(pChild, pRowBox)) {
            return FALSE;
        }

        int nChildSize = 0;
        if(NULL != pChild->Attribute("Size", &nChildSize) && 0 <= nChildSize) {
            pContainer->SetRowHeight(nRowId, (DWORD)nChildSize);
        }

        int nChildWeight = 0;
        if(NULL != pChild->Attribute("Weight", &nChildWeight) && 0 <= nChildWeight) {
            pContainer->SetRowWeight(nRowId, (DWORD)nChildWeight);
        }

        pContainer->SetChildBox(nRowId, pRowBox);

        pChild = pChild->NextSiblingElement("Row");
        ++nRowId;
    }

    *ppBox = pBox.Detach();

    return TRUE;
}

BOOL
LayoutRootCreator::ParseUILayoutHContainer(TiXmlElement *pElement, ILayoutBox **ppBox)
{
    CComPtr<ILayoutHContainer> pContainer;
    if(FAILED(CreateLayoutHContainer(&pContainer)) || NULL == pContainer) {
        return FALSE;
    }

    CComQIPtr<ILayoutBox> pBox = pContainer;
    if(NULL == pBox) {
        return FALSE;
    }

    // Common
    if(!ParseBoxCommon(pElement, pBox, FALSE)) {
        return FALSE;
    }

    // Need size
    DWORD nColCount = 0;
    TiXmlElement *pChild = pElement->FirstChildElement("Col");
    while(NULL != pChild) {
        ++nColCount;
        pChild = pChild->NextSiblingElement("Col");
    }

    pContainer->SetColSize(nColCount);

    DWORD nColId = 0;
    CComPtr<ILayoutBox> pColBox;
    pChild = pElement->FirstChildElement("Col");
    while(NULL != pChild) {
        pColBox = NULL;
        if(FAILED(CreateLayoutBox(&pColBox)) || NULL == pColBox) {
            return FALSE;
        }

        if(!ParseUILayoutChildBoxes(pChild, pColBox)) {
            return FALSE;
        }

        int nChildSize = 0;
        if(NULL != pChild->Attribute("Size", &nChildSize) && 0 <= nChildSize) {
            pContainer->SetColWidth(nColId, (DWORD)nChildSize);
        }

        int nChildWeight = 0;
        if(NULL != pChild->Attribute("Weight", &nChildWeight) && 0 <= nChildWeight) {
            pContainer->SetColWeight(nColId, (DWORD)nChildWeight);
        }

        pContainer->SetChildBox(nColId, pColBox);

        pChild = pChild->NextSiblingElement("Col");
        ++nColId;
    }

    *ppBox = pBox.Detach();

    return TRUE;
}

BOOL
LayoutRootCreator::ParseUILayoutChildBoxes(TiXmlElement *pElement, ILayoutBox *pParentBox)
{
    CComPtr<ILayoutBox> pChildBox;
    TiXmlElement *pChild = pElement->FirstChildElement("Box");
    while(NULL != pChild) {
        pChildBox = NULL;
        if(!ParseUILayoutBox(pChild, &pChildBox)) {
            return FALSE;
        }
        pParentBox->AddChild(pChildBox);
        pChild = pChild->NextSiblingElement("Box");
    }

    return TRUE;
}

BOOL
LayoutRootCreator::ParseBoxCommon(TiXmlElement *pElement, ILayoutBox *pBox, BOOL bParseChildren)
{
    INT32 nX = 0;
    if(NULL != pElement->Attribute("X", &nX)) {
        pBox->SetLayoutX(nX);
    }

    INT32 nY = 0;
    if(NULL != pElement->Attribute("Y", &nY)) {
        pBox->SetLayoutY(nY);
    }

    INT32 nWidth = 0;
    if(NULL != pElement->Attribute("Width", &nWidth)) {
        pBox->SetLayoutWidth(nWidth);
    }

    INT32 nHeight = 0;
    if(NULL != pElement->Attribute("Height", &nHeight)) {
        pBox->SetLayoutHeight(nHeight);
    }

    const char *pMargin = pElement->Attribute("Margin");
    if(NULL != pMargin) {
        CRect oMargin;
        CStringA strMargin = pMargin;
        ParseRect(&oMargin, strMargin);
        pBox->SetLayoutMargin(&oMargin);
    }

    CRect oPadding;
    const char *pPadding = pElement->Attribute("Padding");
    if(NULL != pPadding) {
        CStringA strPadding = pPadding;
        ParseRect(&oPadding, strPadding);
        pBox->SetLayoutPadding(&oPadding);
    }

    DWORD nLayoutType = pBox->GetLayoutType();
    const char *pHAlign = pElement->Attribute("HAlign");
    if(NULL != pHAlign) {
        nLayoutType &= ~MLT_HMASK;
        CStringA strHAlign = pHAlign;
        if(strHAlign == "Left") {
            nLayoutType |= MLT_LEFT;
        } else if(strHAlign == "Right") {
            nLayoutType |= MLT_RIGHT;
        } else if(strHAlign == "Center") {
            nLayoutType |= MLT_HCENTER;
        } else {
            nLayoutType |= MLT_HCENTER;
        }
    }

    const char *pVAlign = pElement->Attribute("VAlign");
    if(NULL != pVAlign) {
        nLayoutType &= ~MLT_VMASK;
        CStringA strVAlign = pVAlign;
        if(strVAlign == "Top") {
            nLayoutType |= MLT_TOP;
        } else if(strVAlign == "Bottom") {
            nLayoutType |= MLT_BOTTOM;
        } else if(strVAlign == "Center") {
            nLayoutType |= MLT_VCENTER;
        } else {
            nLayoutType |= MLT_VCENTER;
        }
    }

    pBox->SetLayoutType(nLayoutType);

    if(!bParseChildren) {
        return TRUE;
    }

    return ParseUILayoutChildBoxes(pElement, pBox);
}

BOOL
LayoutRootCreator::ParseRect(RECT *pRect, CStringA &strRect)
{
    int nInput[4] = {0};
    if(!SplitAsciiStringToNumber(strRect, _T(','), nInput, 4)) {
        return FALSE;
    }

    pRect->left = nInput[0];
    pRect->top = nInput[1];
    pRect->right = nInput[2];
    pRect->bottom = nInput[3];

    return TRUE;
}

BOOL
LayoutRootCreator::MapIdStrToDWord(CStringA &strName, DWORD &nId)
{
    nId = 0;

    IdMap::iterator oIdIt = m_vIdMap.find(strName);
    if(oIdIt == m_vIdMap.end()) {
        return FALSE;
    }

    nId = oIdIt->second;

    return TRUE;
}