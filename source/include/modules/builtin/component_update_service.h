#pragma once

MIDL_INTERFACE("9FF113D2-9AF9-480c-8229-ECDEA0AED764")
IUpdateService : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE CheckUpdate(BOOL bAutoCheck) = 0;
};