#include "stdafx.h"
#include "modules/window/highlight/highlight_service.h"

HighlightService::HighlightService()
{

}

HighlightService::~HighlightService()
{

}

HRESULT
HighlightService::StartHighlight(BSTR strHint, HWND hWindow, COLORREF nColor)
{
    m_oDlgWindowTracker.StartTrack(CString(strHint), hWindow, nColor);
    return S_OK;
}

HRESULT
HighlightService::StopHighlight(BSTR strHint)
{
    m_oDlgWindowTracker.StopTrack(CString(strHint));
    return S_OK;
}

HRESULT
HighlightService::Sparkcle(BSTR strHint, HWND hWindow, COLORREF nColor)
{
    m_oDlgWindowTracker.Sparkcle(CString(strHint), hWindow, nColor);
    return S_OK;
}