#include "stdafx.h"
#include "modules/window/app/properties/dlg_window_input_batch.h"
#include "modules/builtin/component_config.h"
#include "modules/component_id.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindowInputBatch::CDlgWindowInputBatch()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT_BATCH, this);
}

CDlgWindowInputBatch::~CDlgWindowInputBatch()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_INPUT_BATCH);
}

LRESULT
CDlgWindowInputBatch::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    return TRUE;
}

VOID
CDlgWindowInputBatch::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowInputBatch::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowInputBatch::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}