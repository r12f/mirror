#pragma once

#include "base/kv/kv.h"
#include "ui/ui_engine/layout/layout.h"
#include "ui/ui_engine/controls/ui_control.h"
#include "ui/ui_engine/controls/editable_list_ctrl.h"

class CDlgWindowClass :
    public CDialogImpl<CDlgWindowClass>,
    public CLayout,
    public CUIControl
{
    enum {
        WBI_LIST_CLASS_NAME_INDEX               = 0,
        WBI_LIST_CLASS_ATOM_INDEX,
        WBI_LIST_CLASS_STYLE_INDEX,
        WBI_LIST_CLASS_WNDPROC_INDEX,
        WBI_LIST_CLASS_INSTANCE_INDEX,
        WBI_LIST_CLASS_ICON_INDEX,
        WBI_LIST_CLASS_CURSOR_INDEX,
        WBI_LIST_CLASS_BACKGROUND_INDEX,
        WBI_LIST_CLASS_MENU_INDEX,
        WBI_LIST_CLASS_REGISTER_MODULE_INDEX,
        WBI_LIST_CLASS_EXTRA_DATA_INDEX,
        WBI_LIST_CLASS_WND_EXTRA_DATA_INDEX,
        WBI_LIST_CLASS_INFO_NUM,
    };

public:
    enum { IDD = IDD_DIALOG_WINDOW_CLASS };

    CDlgWindowClass();
    virtual ~CDlgWindowClass();

    PERSISTENT_COM_CLASS()

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgWindowClass)
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
    virtual BOOL ResetData(DWORD nParam);
    virtual BOOL FetchData(DWORD nParam);
    virtual BOOL UpdateUI(DWORD nParam);

    BOOL UpdateClassInfo(int nIndex, CString &strInfo);

private:
    CEdit m_vEditInfos[WBI_LIST_CLASS_INFO_NUM];
    CListBox m_oListClassStyle;
    CEdit m_oEditClassExtraData;
    CEdit m_oEditWindowExtraData;

    CString m_strClassName;
    ATOM m_nAtom;
    CString m_strAtomName;
    WNDCLASSEX m_oClassInfo;
    HMODULE m_hModule;
    std::vector<unsigned char> m_vClassExtraData;
    std::vector<unsigned char> m_vWindowExtraData;
};