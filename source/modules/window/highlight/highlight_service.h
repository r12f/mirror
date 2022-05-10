#pragma once

#include "base/com/com_class.h"
#include "modules/window/component_highlight_service.h"
#include "modules/window/highlight/dlg_window_tracker.h"

class HighlightService :
    public IHighlightService
{
public:
    HighlightService();
    virtual ~HighlightService();

    COM_CLASS_FACTORY(HighlightService)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(HighlightService)
            QI_MAPPING(IHighlightService)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override IHighlightService
    STDMETHODIMP StartHighlight(BSTR strHint, HWND hWindow, COLORREF nColor);
    STDMETHODIMP StopHighlight(BSTR strHint);
    STDMETHODIMP Sparkcle(BSTR strHint, HWND hWindow, COLORREF nColor);

private:
    CDlgWindowTracker m_oDlgWindowTracker;
};