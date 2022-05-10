#pragma once

__interface IThreadPool;

// {BE8B6AF4-923B-4eda-8B9F-0D520F6A6077}
MIDL_INTERFACE("BE8B6AF4-923B-4eda-8B9F-0D520F6A6077")
IThreadPoolWorkItem : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE Run(IThreadPool *pThreadPool) = 0;
};

// {4B69FF25-89AF-44b4-B3F7-4583A34AFA9E}
MIDL_INTERFACE("4B69FF25-89AF-44b4-B3F7-4583A34AFA9E")
IThreadPool : IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetMaximumWorker(DWORD nWorkerNum) = 0;
    virtual HRESULT STDMETHODCALLTYPE DispatchToMainThread(IThreadPoolWorkItem *pWorkItem) = 0;
    virtual HRESULT STDMETHODCALLTYPE DispatchToWorker(IThreadPoolWorkItem *pWorkItem) = 0;
    virtual HRESULT STDMETHODCALLTYPE DispatchToSpecifiedWorker(DWORD nWorkerTid, IThreadPoolWorkItem *pWorkItem) = 0;
};