#pragma once

#include <vector>
#include <map>
#include "modules/window/utils/snapshot/window_snapshot.h"
#include "modules/window/utils/snapshot/window_filter.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/filter_combo_box_ex.h"
#include "modules/window/app/window_tree_ctrl.h"

class CDlgWindowList :
    public CDialogImpl<CDlgWindowList>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_LIST };

    enum {
        TIMER_ID_WINDOW_LIST_AUTO_REFRESH       = 0x200,
    };

    CDlgWindowList();
    virtual ~CDlgWindowList();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowList)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
        MSG_WM_TIMER(OnTimer)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_PROPERTIES, OnMenuShowProperties)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_REFRESH, OnMenuRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_NO_AUTO_REFRESH, OnMenuNoAutoRefresh)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_AUTO_REFRESH_PER_SECOND, OnMenuAutoRefreshPerSecond)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_AUTO_REFRESH_PER_THREE_SECONDS, OnMenuAutoRefreshPerThreeSeconds)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_AUTO_REFRESH_PER_FIVE_SECONDS, OnMenuAutoRefreshPerFiveSeconds)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_AUTO_EXPAND, OnMenuAutoExpand)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_FILTER, OnMenuSetFilter)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_RESET_FILTER, OnMenuResetFilter)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_FILTER_TO_CURRENT_PROCESS, OnMenuSetFilterToCurrentProcess)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_FILTER_TO_CURRENT_THREAD, OnMenuSetFilterToCurrentThread)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_FILTER_TO_CURRENT_PROCESS_NAME, OnMenuSetFilterToCurrentProcessName)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_FILTER_TO_CURRENT_CLASSNAME, OnMenuSetFilterToCurrentClassName)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SET_WINDOW_AS_ROOT, OnMenuSetWindowAsRoot)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_RESET_ROOT_WINDOW, OnMenuResetRootWindow)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_DESTROY_WINDOW, OnMenuDestroyWindow)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_GOTO_PROCESS, OnMenuGotoProcess)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SAVE_TO_FILE, OnMenuSaveToFile)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_PARSE_WAIT_CHAIN, OnMenuParseWaitChain)
        COMMAND_ID_HANDLER_EX(ID_WINDOW_TREE_SPARKCLE, OnMenuSparkcle)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnTimer(UINT_PTR nTimerId);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnMenuShowProperties(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuRefresh(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuNoAutoRefresh(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuAutoRefreshPerSecond(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuAutoRefreshPerThreeSeconds(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuAutoRefreshPerFiveSeconds(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuAutoExpand(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetFilter(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuResetFilter(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetFilterToCurrentProcess(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetFilterToCurrentThread(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetFilterToCurrentProcessName(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetFilterToCurrentClassName(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSetWindowAsRoot(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuResetRootWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuDestroyWindow(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuGotoProcess(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSaveToFile(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuParseWaitChain(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnMenuSparkcle(UINT nNotifyCode, UINT nId, HWND hWndCtrl);

protected:
    virtual BOOL DoOperation(UINT nOpId, LPARAM lParam);
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

    BOOL UpdateWindowListNoFetch();
    BOOL UpdateWindowList(IWindowSnapshot *pWindow, HTREEITEM hParentTreeItem, BOOL bAutoExpand);
    CString GetWindowBrief(IWindowSnapshot *pWindow);

    CComPtr<IWindowSnapshot> GetSelectedWindowInfo();

    VOID ShowProperties();
    VOID GotoProcess();
    VOID SetWindowAsRoot();
    VOID ResetRootWindow();
    VOID StartAutoRefresh(UINT nIntervalMs);
    VOID StopAutoRefresh();
    VOID SelectWatchWindow();

private:
    CWindowTreeCtrl m_oTreeWindowList;
    CImageList m_vImageList;
    CComPtr<IWindowSnapshot> m_pWindowSnapshot;
    std::vector<CComPtr<IWindowFilter>> m_vFilterList;
    std::map<HWND, HTREEITEM> m_vTreeWindowItemMap;
};