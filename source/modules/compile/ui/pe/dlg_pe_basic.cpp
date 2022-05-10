#include "stdafx.h"
#include "dlg_pe_basic.h"
#include "base/string_number_convertions.h"
#include "pe_summary_table_filler.h"

CDlgPEBasic::CDlgPEBasic()
    : CLayout(IDD)
    , CUIControl(this)
{

}

CDlgPEBasic::~CDlgPEBasic()
{

}

LRESULT
CDlgPEBasic::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oPEBreakdown.SubclassWindow(GetDlgItem(IDC_TREE_PE_STRUCT));

    ASSERT_RETURN(NULL != m_pPEFile, TRUE);

    PESummaryTableFiller oTableFiller(m_pPEFile);
    oTableFiller.SetTargetAsTreeList(&m_oPEBreakdown).ParseDosHeader().ParseNtHeader().ParseSection();

    return TRUE;
}

VOID
CDlgPEBasic::OnSysCommand(UINT nId, WTL::CPoint oPoint)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPEBasic::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgPEBasic::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgPEBasic::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}
