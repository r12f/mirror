#include "stdafx.h"
#include "modules/compile/ui/dlg_compile.h"

CDlgCompile::CDlgCompile()
    : CLayout(IDD)
    , CUIControl(this)
{
    
}

CDlgCompile::~CDlgCompile()
{

}

LRESULT
CDlgCompile::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    InitDropTarget(m_hWnd);
    return TRUE;
}

VOID
CDlgCompile::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgCompile::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgCompile::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgCompile::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgCompile::OnFileDropped(LPCTSTR pFilePath)
{
    AnalyzeFile(pFilePath);
}

VOID
CDlgCompile::AnalyzeFile(LPCTSTR pFilePath)
{
    if (NULL == pFilePath) {
        return;
    }

    LPCTSTR pFileExt = ::PathFindExtension(pFilePath);
    if (NULL == pFileExt) {
        return;
    }

    if (_tcsicmp(pFileExt, _T(".exe")) == 0 || _tcsicmp(pFileExt, _T(".dll")) == 0) {
        CDlgPE *pDlgPE = new CDlgPE();
        if (FAILED(pDlgPE->AnalyzePE(pFilePath))) {
            return;
        }

        pDlgPE->Create(NULL);
        pDlgPE->ShowWindow(SW_SHOW);
    }
    else if (_tcsicmp(pFileExt, _T(".pdb")) == 0) {
        CDlgPdb *pDlgPdb = new CDlgPdb();
        pDlgPdb->Create(NULL);
        pDlgPdb->ShowWindow(SW_SHOW);
    }

    return;
}