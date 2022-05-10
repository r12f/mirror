#pragma once

#include "modules/compile/ui/pe/dlg_pe_basic.h"
#include "third_party/LibPE/Include/LibPE.h"
using namespace LibPE;

class CDlgPE :
    public CDialogImpl<CDlgPE>,
    public CLayout,
    public CUIControl
{
public:
    enum { IDD = IDD_DIALOG_PE };

    enum {
        MIRROR_DIALOG_PE_MENU_SET_TOPMOST       = 0x100,
    };

    CDlgPE();
    virtual ~CDlgPE();

    PERSISTENT_COM_CLASS()

    HRESULT AnalyzePE(LPCTSTR strFilePath);

    BEGIN_QUERY_INTERFACE()
        QI_CHAIN_CLASS(CUIControl)
    END_QUERY_INTERFACE()

    BEGIN_MSG_MAP_EX(CDlgPE)
        CHAIN_MSG_MAP(CLayout)
        CHAIN_MSG_MAP(CUIControl)
        MSG_WM_INITDIALOG(OnInitDialog)
        MSG_WM_SYSCOMMAND(OnSysCommand)
        MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_CLOSE(OnClose)
        MSG_WM_DESTROY(OnDestroy)
    END_MSG_MAP()

protected:
    LRESULT OnInitDialog(HWND hWnd, LPARAM lInitData);
    VOID OnSysCommand(UINT nId, WTL::CPoint oPoint);
    VOID OnShowWindow(BOOL bShow, int nStatus);
    VOID OnClose();
    VOID OnDestroy();
    VOID OnSysMenuSetTopMost();

    virtual HWND GetNativeWindowFromResId(LPCSTR strId);
    virtual void OnFinalMessage(_In_ HWND /*hWnd*/);

private:
    CString m_strPEFilePath;
    LibPEPtr<IPEFile> m_pPEFile;

    CTabPanel m_oTabPEInfo;
    CDlgPEBasic m_oDlgPEBasic;
};