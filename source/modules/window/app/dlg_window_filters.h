#pragma once

#include <vector>
#include "base/kv/kv.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"
#include "ui/ui_engine/controls/filter_combo_box_ex.h"
#include "modules/window/utils/snapshot/window_filter.h"
#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_snapshot.h"

class CDlgWindowFilters :
    public CDialogImpl<CDlgWindowFilters>
{
public:
    enum { IDD = IDD_DIALOG_WINDOW_FILTERS };

    CDlgWindowFilters();
    virtual ~CDlgWindowFilters();

    VOID SetWindowFilters(const std::vector<CComPtr<IWindowFilter>> &vFilters);
    VOID GetWindowFilters(std::vector<CComPtr<IWindowFilter>> &vFilters);

    BEGIN_MSG_MAP_EX(CDlgWindowFilters)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_CLOSE(OnClose)
        COMMAND_ID_HANDLER_EX(IDOK, OnOK)
        COMMAND_ID_HANDLER_EX(IDCANCEL, OnCancel)
        COMMAND_ID_HANDLER_EX(IDC_COMBO_WINDOW_FILTER_TYPE, OnCmdComboWindowFilterType)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_FILTER_ADD, OnCmdBtnWindowFilterAdd)
        COMMAND_ID_HANDLER_EX(IDC_BUTTON_WINDOW_FILTER_REMOVE, OnCmdBtnWindowFilterRemove)
    END_MSG_MAP()

    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnClose();
    VOID OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCmdComboWindowFilterType(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCmdBtnWindowFilterAdd(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    VOID OnCmdBtnWindowFilterRemove(UINT nNotifyCode, UINT nId, HWND hWndCtrl);
    
protected:
    BOOL ReloadFilterTypeInfo();
    CComPtr<IWindowFilterFactory> GetSelectedFilterFactory();
    BOOL GetSelectedFilterRelation(DWORD &nRelation);
    BOOL GetInputFilterValue(CString &strValue);
    BOOL GetSelectedFilterOp(DWORD &nOp);
    BOOL ReloadFilterList();

private:
    std::vector<CComPtr<IWindowFilterFactory>> m_vFilterFactories;
    std::vector<CComPtr<IWindowFilter>> m_vFilters;
    CEditableListCtrl m_oListFilters;
    CComboBoxEx m_oComboFilterType;
    CComboBoxEx m_oComboFilterRelation;
    CFilterComboBoxEx m_oComboFilterValue;
    CComboBoxEx m_oComboFilterOp;
};