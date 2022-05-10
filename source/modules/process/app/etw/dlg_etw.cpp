#include "platform.h"
#include "app/process/etw/dlg_etw.h"

CDlgEtw::CDlgEtw()
    : CLayout(this)
    , CUIControl(this)
{

}

CDlgEtw::~CDlgEtw()
{

}

LRESULT
CDlgEtw::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    return TRUE;
}

VOID
CDlgEtw::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgEtw::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgEtw::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

BOOL
CDlgEtw::Layout()
{
    RECT oRect = {0};
    GetClientRect(&oRect);

    return TRUE;
}