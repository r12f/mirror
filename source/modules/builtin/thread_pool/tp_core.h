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

    /// ���������ַ�����
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
    /// ��ʼ���̳߳أ��������ش��ڣ����߳�Worker��Ĭ�ϳ�ʼ��������Worker
    BOOL Init();

    /// �����̳߳�
    BOOL Uninit();

    /// ����Worker
    DWORD CreateTPWorkerIfPossible(BOOL bAttachCurrentThread);

    /// ��ȡ���е�Worker
    TPWorker * GetFreeWorker();

    /// ��������ӽ��ȴ�������
    void AddWorkToWaitQueue(IThreadPoolWorkItem * pWorkItem);

private:
    DWORD m_nMainTid;
    DWORD m_nMaximumWorker;
    WorkerList m_vWorkers;
    LockCS m_oLockWorker;
    WorkQueue m_vPendingWorks;
    LockCS m_oLockPendingsWorks;
};