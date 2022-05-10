#include "stdafx.h"
#include "ui/ui_engine/layout/layout_loader.h"

LayoutLoader * LayoutLoader::Get()
{
    static LayoutLoader s_oLayoutLoader;
    return &s_oLayoutLoader;
}

LayoutLoader::LayoutLoader()
{

}

LayoutLoader::~LayoutLoader()
{

}

BOOL
LayoutLoader::LoadLayoutFromResource(DWORD nUIXmlId, DWORD nResourceMapId)
{
    HMODULE hInstance = ::GetModuleHandle(NULL);

    if(!LoadResourceMapXml(hInstance, nResourceMapId)) {
        return FALSE;
    }

    if(!LoadUIXml(hInstance, nUIXmlId)) {
        return FALSE;
    }

    return TRUE;
}

BOOL
LayoutLoader::UnloadLayout()
{
    m_vRootMap.clear();
    return TRUE;
}

HRESULT
LayoutLoader::CreateLayoutRoot(DWORD nRootId, HWND hWnd, ILayoutBox **ppBox, CLayout *pLayout)
{
    ASSERT_RETURN(0 != nRootId, E_INVALIDARG);
    ASSERT_RETURN(NULL != hWnd, E_INVALIDARG);
    ASSERT_RETURN(NULL != ppBox, E_POINTER);

    RootMap::iterator oRoot = m_vRootMap.find(nRootId);
    if(oRoot == m_vRootMap.end()) {
        return E_FAIL;
    }

    return LayoutRootCreator(m_vIdMap, pLayout).Run(oRoot->second, hWnd, ppBox);
}

BYTE *
LayoutLoader::LoadResource(HMODULE hInstance, DWORD nUIXmlId)
{
    HRSRC hResInfo = ::FindResource(hInstance, MAKEINTRESOURCE(nUIXmlId), _T("DATA"));
    ASSERT_RETURN(NULL != hResInfo, FALSE);

    HGLOBAL hResGlobal = ::LoadResource(hInstance, hResInfo);
    ASSERT_RETURN(NULL != hResGlobal, FALSE);

    return (BYTE *)::LockResource(hResGlobal);
}

BOOL
LayoutLoader::LoadResourceMapXml(HMODULE hInstance, DWORD nResourceMapId)
{
    char *pResourceMapXmlRes = (char *)LoadResource(hInstance, nResourceMapId);
    ASSERT_RETURN(NULL != pResourceMapXmlRes, FALSE);

    TiXmlDocument oXmlDocument;
    if(!oXmlDocument.Parse(pResourceMapXmlRes)) {
        return FALSE;
    }

    TiXmlElement *pRoot = oXmlDocument.RootElement();
    if(NULL == pRoot) {
        return FALSE;
    }

    CStringA strName;
    int nId = 0;
    TiXmlElement *pResource = pRoot->FirstChildElement("Resource");
    while(NULL != pResource) {
        strName = "";
        nId = 0;

        strName = pResource->Attribute("Name");
        pResource->Attribute("Id", &nId);
        if(strName.GetLength() > 0 && nId > 0) {
            m_vIdMap[strName] = (DWORD)nId;
        }

        pResource = pResource->NextSiblingElement("Resource");
    }

    return TRUE;
}

BOOL
LayoutLoader::LoadUIXml(HMODULE hInstance, DWORD nUIXmlId)
{
    char *pUIXmlRes = (char *)LoadResource(hInstance, nUIXmlId);
    ASSERT_RETURN(NULL != pUIXmlRes, FALSE);

    if(!m_oXmlDocument.Parse(pUIXmlRes)) {
        return FALSE;
    }

    TiXmlElement *pRoot = m_oXmlDocument.RootElement();
    if(NULL == pRoot) {
        return FALSE;
    }

    TiXmlElement *pElement = pRoot->FirstChildElement("Root");
    while(NULL != pElement) {
        if(!ParseUIRoot(pElement)) {
            return FALSE;
        }

        pElement = pElement->NextSiblingElement("Root");
    }

    return TRUE;
}

BOOL
LayoutLoader::ParseUIRoot(TiXmlElement *pElement)
{
    DWORD nId = 0;
    CStringA strId = pElement->Attribute("Id");
    if(!MapIdStrToDWord(strId, nId)) {
        return FALSE;
    }

    m_vRootMap[nId] = pElement;

    return TRUE;
}

BOOL
LayoutLoader::MapIdStrToDWord(CStringA &strName, DWORD &nId)
{
    nId = 0;

    IdMap::iterator oIdIt = m_vIdMap.find(strName);
    if(oIdIt == m_vIdMap.end()) {
        return FALSE;
    }

    nId = oIdIt->second;

    return TRUE;
}