#pragma once

#include <list>
#include <map>
#include "base/threading/lock.h"
#include "base/com/com_class.h"
#include "modules/builtin/thread_pool/tp_common.h"
#include "modules/builtin/thread_pool/tp_worker.h"

class TPCore :
    public IThreadPool,
    public CWindowImpl<TPCore>
{
    typedef std::map<DWORD, TPWorker *> WorkerList;
    typedef std::list<IThreadPoolWorkItem *> WorkQueue;

public:
    TPCore();
    virtual ~TPCore();

    /// 主动触发分发任务
    BOOL ScheduleWork();

    COM_CLASS_FACTORY(TPCore)
    MTA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(TPCore)
            QI_MAPPING(IThreadPool)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHODIMP SetMaximumWorker(DWORD nWorkerNum);
    STDMETHODIMP DispatchToMainThread(IThreadPoolWorkItem *pWorkItem);
    STDMETHODIMP DispatchToWorker(IThreadPoolWorkItem *pWorkItem);
    STDMETHODIMP DispatchToSpecifiedWorker(DWORD nWorkerTid, IThreadPoolWorkItem *pWorkItem);

    BEGIN_MSG_MAP(TPCore)
        MESSAGE_HANDLER(WM_TP_SCHEDULE_WORK, OnScheduleWork)
    END_MSG_MAP()

protected:
    LRESULT OnScheduleWork(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
    /// 初始化线程池，创建主控窗口，主线程Worker和默认初始化数量的Worker
    BOOL Init();

    /// 销毁线程池
    BOOL Uninit();

    /// 创建Worker
    DWORD CreateTPWorkerIfPossible(BOOL bAttachCurrentThread);

    /// 获取空闲的Worker
    TPWorker * GetFreeWorker();

    /// 将任务添加进等待队列中
    void AddWorkToWaitQueue(IThreadPoolWorkItem * pWorkItem);

private:
    DWORD m_nMainTid;
    DWORD m_nMaximumWorker;
    WorkerList m_vWorkers;
    LockCS m_oLockWorker;
    WorkQueue m_vPendingWorks;
    LockCS m_oLockPendingsWorks;
};