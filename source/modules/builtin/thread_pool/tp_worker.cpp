#include "stdafx.h"
#include "modules/builtin/thread_pool/tp_worker.h"
#include "modules/builtin/thread_pool/tp_core.h"

TPWorker::TPWorker()
    : m_nTid(0)
    , m_hThread(NULL)
    , m_nState(TP_WORKER_STATE_FREE)
{

}

TPWorker::~TPWorker()
{

}

DWORD
TPWorker::Init(BOOL bAttachCurrentThread)
{
    if(bAttachCurrentThread) {
        Create(HWND_MESSAGE);
        m_nTid = ::GetCurrentThreadId();
    } else {
        m_hThread = ::CreateThread(NULL, 0, TPWorker::TPWorkerProc, (LPVOID)this, 0, &m_nTid);
        if(NULL == m_hThread) {
            m_nTid = 0;
        }
    }

    return m_nTid;
}

BOOL
TPWorker::Uninit()
{
    if(NULL == m_hThread) {
        DestroyWindow();
        return TRUE;
    }

    PostMessage(WM_TP_QUIT_WORKER);

    ::WaitForSingleObject(m_hThread, INFINITE);
    ::CloseHandle(m_hThread);
    
    m_hThread = NULL;

    return TRUE;
}

DWORD
TPWorker::SetState(DWORD nNewState)
{
    return (DWORD)::InterlockedExchange(&m_nState, nNewState);
}

DWORD
TPWorker::GetState()
{
    volatile DWORD *pState = &m_nState;
    return *pState;
}

BOOL
TPWorker::IsFree()
{
    return GetState() == TP_WORKER_STATE_FREE;
}

HRESULT
TPWorker::DispatchWork(TPCore *pCore, IThreadPoolWorkItem *pWorkItem)
{
    if(NULL == pWorkItem) {
        return E_INVALIDARG;
    }

    // TPCore无需改变引用计数，因为TPCore的生命周期一定比TPWorker长，这是必须要保证的
    pWorkItem->AddRef();

    if(!PostMessage(WM_TP_DISPATCH_WORK, (WPARAM)pWorkItem, (LPARAM)pCore)) {
        pWorkItem->Release();
        return E_FAIL;
    }

    return S_OK;
}

LRESULT
TPWorker::OnDispatchWork(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    IThreadPoolWorkItem *pWorkItem = (IThreadPoolWorkItem *)wParam;
    TPCore *pTPCore = (TPCore *)lParam;

    SetState(TP_WORKER_STATE_BUSY);

    if(NULL != pWorkItem) {
        pWorkItem->Run(pTPCore);
        pWorkItem->Release();
    }

    SetState(TP_WORKER_STATE_FREE);

    pTPCore->ScheduleWork();

    return 0;
}

LRESULT
TPWorker::OnQuitWorker(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    DestroyWindow();
    PostQuitMessage(0);
    return 0;
}

DWORD WINAPI
TPWorker::TPWorkerProc(LPVOID lpParameter)
{
    TPWorker *pThis = (TPWorker *)lpParameter;
    if(NULL == pThis) {
        return 0;
    }

    return pThis->ThreadMain();
}

DWORD
TPWorker::ThreadMain()
{
    Create(HWND_MESSAGE);

    CMessageLoop theLoop;
    theLoop.Run();

    return 0;
}
