#include "stdafx.h"
#include "modules/system/app/dlg_system.h"

CDlgSystem::CDlgSystem()
    : CUIControl(this)
    , CLayout(IDD)
{

}

CDlgSystem::~CDlgSystem()
{

}

LRESULT
CDlgSystem::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    return TRUE;
}

VOID
CDlgSystem::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgSystem::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgSystem::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}
