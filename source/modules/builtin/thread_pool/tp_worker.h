#pragma once

#include "modules/builtin/thread_pool/tp_common.h"

class TPCore;

class TPWorker :
    public CWindowImpl<TPWorker>
{
public:
    TPWorker();
    ~TPWorker();

    /// 初始化
    DWORD Init(BOOL bAttachCurrentThread);

    /// 反初始化
    BOOL Uninit();

    /// 设置Worker状态
    DWORD SetState(DWORD nNewState);

    /// 获取Worker状态
    DWORD GetState();

    /// 判断是否是一个可用的Worker
    BOOL IsFree();

    /// 分发任务
    HRESULT DispatchWork(TPCore *pCore, IThreadPoolWorkItem *pWorkItem);
    
    BEGIN_MSG_MAP(CDlgConfig)
        MESSAGE_HANDLER(WM_TP_DISPATCH_WORK, OnDispatchWork)
        MESSAGE_HANDLER(WM_TP_QUIT_WORKER, OnQuitWorker)
    END_MSG_MAP()

protected:
    LRESULT OnDispatchWork(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT OnQuitWorker(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);

protected:
    static DWORD WINAPI TPWorkerProc(LPVOID lpParameter);
    DWORD ThreadMain();

private:
    DWORD m_nTid;
    HANDLE m_hThread;
    DWORD m_nState;
};