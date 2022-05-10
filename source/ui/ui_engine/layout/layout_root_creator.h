#pragma once

#include <string>
#include <vector>
#include <map>
#include "ui/layout.h"
#include "third_party/tinyxml/tinyxml.h"

typedef std::map<CStringA, DWORD> IdMap;

class CLayout;
class LayoutRootCreator
{
    typedef std::vector<HWND> WndChain;
    
public:
    LayoutRootCreator(IdMap &vIdMap, CLayout *pLayout);
    ~LayoutRootCreator();

    HRESULT Run(TiXmlElement *pElement, HWND hWnd, ILayoutBox **ppBox);

protected:
    BOOL ParseUILayoutBox(TiXmlElement *pElement, ILayoutBox **ppBox);
    BOOL ParseUILayoutBasic(TiXmlElement *pElement, ILayoutBox **ppBox);
    BOOL ParseUILayoutNativeWindow(TiXmlElement *pElement, ILayoutBox **ppBox);
    BOOL ParseUILayoutVContainer(TiXmlElement *pElement, ILayoutBox **ppBox);
    BOOL ParseUILayoutHContainer(TiXmlElement *pElement, ILayoutBox **ppBox);
    BOOL ParseUILayoutChildBoxes(TiXmlElement *pElement, ILayoutBox *pParentBox);
    BOOL ParseBoxCommon(TiXmlElement *pElement, ILayoutBox *pBox, BOOL bParseChildren = TRUE);
    BOOL ParseRect(RECT *pRect, CStringA &strRect);
    BOOL MapIdStrToDWord(CStringA &strName, DWORD &nId);

private:
    IdMap &m_vIdMap;
    CLayout *m_pLayout;
    WndChain m_vRootWndChain;
    WndChain m_vParentWndChain;
};