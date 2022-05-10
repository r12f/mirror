#include "stdafx.h"
#include "modules/process/wait_chain/wait_chain_node_parser.h"

// Human-readable names for the different synchronization types.
static TCHAR * s_strWaitChainObjectType[WctMaxType] = {
    _T(""),
    _T("CriticalSection"),
    _T("SendMessage"),
    _T("Mutex"),
    _T("Alpc"),
    _T("Com"),
    _T("ThreadWait"),
    _T("ProcWait"),
    _T("Thread"),
    _T("ComActivation"),
    _T("Unknown"),
    _T("SocketIo"),
    _T("SmbIo")
};

static TCHAR * s_strWaitChainObjectStatus[WctStatusMax] = {
    _T(""),
    _T("NoAccess"),
    _T("Running"),
    _T("Blocked"),
    _T("PidOnly"),
    _T("PidOnlyRpcss"),
    _T("Owned"),
    _T("NotOwned"),
    _T("Abandoned"),
    _T("Unknown"),
    _T("Error"),
};

VOID
WaitChainNodeParser::Parse(WAITCHAIN_NODE_INFO *pNode)
{
    ASSERT_RETURN_VOID(NULL != pNode);
    ASSERT_RETURN_VOID(pNode->ObjectType < WctMaxType);
    ASSERT_RETURN_VOID(pNode->ObjectStatus < WctStatusMax);

    CString strWaitType = s_strWaitChainObjectType[pNode->ObjectType];
    CString strWaitStatus = s_strWaitChainObjectStatus[pNode->ObjectStatus];
    CString strNodeInfo;
    switch(pNode->ObjectType) {
    case WctThreadType:
        strNodeInfo.Format(_T("进程Id = %lu，线程Id = %lu"),
            pNode->ThreadObject.ProcessId,
            pNode->ThreadObject.ThreadId);
        break;

    default:
        if(pNode->LockObject.ObjectName[0] != L'\0') {
            strNodeInfo.Format(_T("对象名称 = %s"), pNode->LockObject.ObjectName);
        }
        break;
    }

    if(strNodeInfo.GetLength() == 0) {
        m_strInfo.Format(_T("类型 = %s，状态 = %s"),
            strWaitType.GetBuffer(), strWaitStatus.GetBuffer());
    } else {
        m_strInfo.Format(_T("类型 = %s，状态 = %s，%s"),
            strWaitType.GetBuffer(), strWaitStatus.GetBuffer(), strNodeInfo.GetBuffer());
    }

    return;
}