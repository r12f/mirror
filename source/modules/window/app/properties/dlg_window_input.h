#pragma once

#include <map>
#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/menu_button.h"
#include "ui/ui_engine/controls/tab_panel.h"
#include "ui/ui_engine/controls/filter_combo_box_ex.h"
#include "modules/window/app/properties/dlg_window_input_key_mouse.h"
#include "modules/window/app/properties/dlg_window_input_batch.h"

class CDlgWindowInput :
    public CDialogImpl<CDlgWindowInput>,
    public CLayout,
    public CUIControl
{
    typedef std::map<CString, DWORD> WMList;

public:
    enum { IDD = IDD_DIALOG_WINDOW_INPUT };

    CDlgWindowInput();
    virtual ~CDlgWindowInput();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowInput)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();

protected:
    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);

protected:
    VOID ProvideHotkey();

private:
    CTabPanel m_oTabInput;
    CDlgWindowInputKeyMouse m_oDlgInputKeyMouse;
    CDlgWindowInputBatch m_oDlgInputBatch;
};