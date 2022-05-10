#include "stdafx.h"
#include "modules/builtin/thread_pool/tp_core.h"

TPCore::TPCore()
    : m_nMainTid(0)
    , m_nMaximumWorker(0)
{
    Init();
}

TPCore::~TPCore()
{
    Uninit();
}

BOOL
TPCore::Init()
{
    // 创建主控窗口
    if(NULL == Create(HWND_MESSAGE)) {
        return FALSE;
    }

    // 设置可用的最大线程数
    SYSTEM_INFO oSystemInfo = {0};
    GetSystemInfo(&oSystemInfo);
    m_nMaximumWorker = oSystemInfo.dwNumberOfProcessors * 2;

    // 创建主线程Worker
    m_nMainTid = CreateTPWorkerIfPossible(TRUE);
    if(0 == m_nMainTid) {
        return FALSE;
    }

    // 创建默认初始线程Worker
    int i = 0;
    for(i = 0; i < TP_INITIAL_WORKER_NUM; ++i) {
        if(0 != CreateTPWorkerIfPossible(FALSE)) {
            return FALSE;
        }
    }

    return TRUE;
}

BOOL
TPCore::Uninit()
{
    DestroyWindow();

    AutoCS oLock(m_oLockWorker);

    WorkerList::iterator oWorkIt = m_vWorkers.begin();
    while(oWorkIt != m_vWorkers.end()) {
        oWorkIt->second->Uninit();
        delete oWorkIt->second;
        ++oWorkIt;
    }

    m_vWorkers.clear();

    return TRUE;
}

BOOL
TPCore::ScheduleWork()
{
    return PostMessage(WM_TP_SCHEDULE_WORK);
}

HRESULT
TPCore::SetMaximumWorker(DWORD nWorkerNum)
{
    m_nMaximumWorker = nWorkerNum;
    return S_OK;
}

HRESULT
TPCore::DispatchToMainThread(IThreadPoolWorkItem *pWorkItem)
{
    if(NULL == pWorkItem) {
        return E_INVALIDARG;
    }

    AutoCS oLock(m_oLockWorker);
    TPWorker *pWorker = m_vWorkers[m_nMainTid];
    if(NULL == pWorker) {
        return E_FAIL;
    }

    return pWorker->DispatchWork(this, pWorkItem);
}

HRESULT
TPCore::DispatchToWorker(IThreadPoolWorkItem *pWorkItem)
{
    if(NULL == pWorkItem) {
        return E_INVALIDARG;
    }

    {
        AutoCS oLock(m_oLockWorker);
        TPWorker *pWorker = GetFreeWorker();
        if(NULL != pWorker) {
            return pWorker->DispatchWork(this, pWorkItem);
        }
    }

    AddWorkToWaitQueue(pWorkItem);
    ScheduleWork();

    return S_OK;
}

HRESULT
TPCore::DispatchToSpecifiedWorker(DWORD nWorkerTid, IThreadPoolWorkItem *pWorkItem)
{
    if(0 == nWorkerTid || NULL == pWorkItem) {
        return E_INVALIDARG;
    }

    {
        AutoCS oLock(m_oLockWorker);
        WorkerList::iterator oWorkerIt = m_vWorkers.find(nWorkerTid);
        if(oWorkerIt == m_vWorkers.end()) {
            return E_FAIL;
        }

        oWorkerIt->second->DispatchWork(this, pWorkItem);
    }

    return S_OK;
}

LRESULT
TPCore::OnScheduleWork(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
    CComPtr<IThreadPoolWorkItem> pWorkItem;

    {
        AutoCS oLock(m_oLockWorker);
        TPWorker *pWorker = GetFreeWorker();
        if(NULL == pWorker) {
            return 0;
        }

        // 获取任务
        {
            AutoCS oLock(m_oLockPendingsWorks);
            if(m_vPendingWorks.size() == 0) {
                return 0;
            }
            pWorkItem = m_vPendingWorks.front();
            pWorkItem.p->Release();
            m_vPendingWorks.pop_front();
        }

        if(NULL == pWorkItem) {
            return 0;
        }

        // 分发给可用的Worker
        return pWorker->DispatchWork(this, pWorkItem);
    }

    return 0;
}

DWORD
TPCore::CreateTPWorkerIfPossible(BOOL bAttachCurrentThread)
{
    AutoCS oLock(m_oLockWorker);

    if(m_vWorkers.size() >= m_nMaximumWorker) {
        return 0;
    }

    TPWorker *pMainWorker = new TPWorker;
    if(NULL == pMainWorker) {
        return 0;
    }

    DWORD nTid = pMainWorker->Init(bAttachCurrentThread);
    m_vWorkers[nTid] = pMainWorker;

    return nTid;
}

TPWorker *
TPCore::GetFreeWorker()
{
    AutoCS oLock(m_oLockWorker);

    WorkerList::iterator oWorkIt = m_vWorkers.begin();
    while(oWorkIt != m_vWorkers.end()) {
        if(oWorkIt->first != m_nMainTid && oWorkIt->second->IsFree()) {
            return oWorkIt->second;
        }
        ++oWorkIt;
    }

    DWORD nWorkTid = CreateTPWorkerIfPossible(FALSE);
    if(0 == nWorkTid) {
        return NULL;
    }

    return m_vWorkers[nWorkTid];
}

void
TPCore::AddWorkToWaitQueue(IThreadPoolWorkItem * pWorkItem)
{
    AutoCS oLock(m_oLockPendingsWorks);
    pWorkItem->AddRef();
    m_vPendingWorks.push_back(pWorkItem);
}
