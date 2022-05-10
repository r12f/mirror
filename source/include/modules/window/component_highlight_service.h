#pragma once

// {41DAC9D6-1E6A-4d66-8B70-584C494755E8}
MIDL_INTERFACE("41DAC9D6-1E6A-4d66-8B70-584C494755E8")
IHighlightService : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE StartHighlight(BSTR strHint, HWND hWindow, COLORREF nColor) = 0;
    virtual HRESULT STDMETHODCALLTYPE StopHighlight(BSTR strHint) = 0;
    virtual HRESULT STDMETHODCALLTYPE Sparkcle(BSTR strHint, HWND hWindow, COLORREF nColor) = 0;
};