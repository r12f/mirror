#pragma once

// {61FA245F-FF53-4840-864A-BCBC452321B5}
MIDL_INTERFACE("61FA245F-FF53-4840-864A-BCBC452321B5")
IProcess : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE GetPid(DWORD *pId) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetProcessName(BSTR *pName) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetExePath(BSTR *pPath) = 0;
};

// {BFAA95BA-02B3-49ed-98BD-E0F1A2F341DD}
MIDL_INTERFACE("BFAA95BA-02B3-49ed-98BD-E0F1A2F341DD")
IProcessTree : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Snapshot() = 0;
};
