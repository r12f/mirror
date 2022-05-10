#include "stdafx.h"
#include "base/system/system_snapshot_walker.h"

BOOL
GetProcNameByEnum(DWORD nPid, CString &strProcName)
{
    Process32Walker oWalker;
    if(oWalker.Start()) {
        return FALSE;
    }

    do {
        PROCESSENTRY32 *pEntry = oWalker.Get();
        if(pEntry->th32ProcessID == nPid) {
            strProcName = pEntry->szExeFile;
            return TRUE;
        }
    } while(oWalker.WalkNext());

    return FALSE;
}
