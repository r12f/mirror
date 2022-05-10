#pragma once

#include "ui/layout.h"
#include "third_party/tinyxml/tinyxml.h"
#include "ui/ui_engine/layout/layout_root_creator.h"

class CLayout;

class LayoutLoader
{
    typedef std::map<DWORD, TiXmlElement *> RootMap;

public:
    static LayoutLoader * Get();

    LayoutLoader();
    ~LayoutLoader();

    BOOL LoadLayoutFromResource(DWORD nUIXmlId, DWORD nResourceMapId);
    BOOL UnloadLayout();
    HRESULT CreateLayoutRoot(DWORD nRootId, HWND hWnd, ILayoutBox **ppBox, CLayout *pLayout);

protected:
    BYTE * LoadResource(HMODULE hInstance, DWORD nUIXmlId);
    BOOL LoadResourceMapXml(HMODULE hInstance, DWORD nResourceMapId);
    BOOL LoadUIXml(HMODULE hInstance, DWORD nUIXmlId);
    BOOL ParseUIRoot(TiXmlElement *pElement);
    BOOL MapIdStrToDWord(CStringA &strName, DWORD &nId);

private:
    TiXmlDocument m_oXmlDocument;
    RootMap m_vRootMap;
    IdMap m_vIdMap;
};