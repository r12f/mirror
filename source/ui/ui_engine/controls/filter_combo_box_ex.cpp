#include "stdafx.h"
#include "ui/ui_engine/controls/filter_combo_box_ex.h"

CFilterComboBoxEx::CFilterComboBoxExEdit::CFilterComboBoxExEdit(CFilterComboBoxEx *pComboBox)
    : m_pComboBox(pComboBox)
{

}

CFilterComboBoxEx::CFilterComboBoxExEdit::~CFilterComboBoxExEdit()
{

}

VOID
CFilterComboBoxEx::CFilterComboBoxExEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    SetMsgHandled(FALSE);

    if(NULL == m_pComboBox) {
        return;
    }

    if(nChar != VK_RETURN) {
        return;
    }

    if(m_pComboBox->GetDroppedState()) {
        m_pComboBox->ShowDropDown(FALSE);
    }

    SetMsgHandled(TRUE);

    return;
}

VOID
CFilterComboBoxEx::CFilterComboBoxExEdit::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
    if(nChar == VK_RETURN) {
        return;
    }

    SetMsgHandled(FALSE);

    HWND hComboBox = ::GetParent(GetParent());
    if(NULL == hComboBox || !::IsWindow(hComboBox)) {
        return;
    }

    ::SetTimer(hComboBox, FCBE_TIMER_ID_REFRESH_LIST, 300, NULL);
}

LRESULT
CFilterComboBoxEx::CFilterComboBoxExEdit::OnSetText(LPCTSTR pText)
{
    SetMsgHandled(FALSE);
    return 0;
}

CFilterComboBoxEx::CFilterComboBoxEx()
    : m_oEditInput(this)
{

}

CFilterComboBoxEx::~CFilterComboBoxEx()
{

}

BOOL
CFilterComboBoxEx::SubclassWindow(HWND hWnd)
{
    ASSERT_RETURN(NULL != hWnd && ::IsWindow(hWnd), FALSE);

    if(!CWindowImpl<CFilterComboBoxEx, CComboBoxEx>::SubclassWindow(hWnd)) {
        return FALSE;
    }

    if(!m_oEditInput.SubclassWindow(GetEditCtrl())) {
        UnsubclassWindow();
        return FALSE;
    }

    m_oEditInput.ModifyStyle(0, ES_WANTRETURN, 0);

    return TRUE;
}

HWND
CFilterComboBoxEx::UnsubclassWindow(BOOL bForce)
{
    m_oEditInput.UnsubclassWindow(bForce);
    return CWindowImpl<CFilterComboBoxEx, CComboBoxEx>::UnsubclassWindow(bForce);
}

VOID
CFilterComboBoxEx::TakeSnapshot()
{
    m_vItemList.clear();

    int i = 0, nCount = GetCount();
    for(i = 0; i < nCount; ++i) {
        SnapshotItem oItem = {0};
        oItem.m_oItem.mask = CBEIF_IMAGE | CBEIF_SELECTEDIMAGE | CBEIF_INDENT | CBEIF_LPARAM;
        oItem.m_oItem.iItem = i;
        if(GetItem(&(oItem.m_oItem))) {
            GetItemText(i, oItem.m_strText);
            oItem.m_oItem.pszText = oItem.m_strText.GetBuffer();
            m_vItemList.push_back(oItem);
        }
    }
}

VOID
CFilterComboBoxEx::OnTimer(UINT_PTR nIDEvent)
{
    if(FCBE_TIMER_ID_REFRESH_LIST != nIDEvent) {
        SetMsgHandled(FALSE);
        return;
    }

    KillTimer(FCBE_TIMER_ID_REFRESH_LIST);

    if(GetFocus() != m_oEditInput.m_hWnd) {
        return;
    }

    if(!m_oEditInput.GetModify()) {
        return;
    }

    ResetContent();

    CString strInput;
    m_oEditInput.GetWindowText(strInput);
    strInput.MakeLower();

    int nInsertCount = 0;
    CString strItem;
    SnapshotList::iterator oItemIt = m_vItemList.begin();
    while(oItemIt != m_vItemList.end()) {
        strItem = oItemIt->m_strText;
        strItem.MakeLower();
        if(strItem.Find(strInput) >= 0) {
            AddItem(oItemIt->m_oItem.pszText, oItemIt->m_oItem.iImage, oItemIt->m_oItem.iSelectedImage, oItemIt->m_oItem.iIndent, oItemIt->m_oItem.lParam);
            ++nInsertCount;
        }
        ++oItemIt;
    }

    if(nInsertCount > 0) {
        if(!GetDroppedState()) {
            ShowDropDown(TRUE);
        }
    } else {
        if(GetDroppedState()) {
            ShowDropDown(FALSE);
        }
    }

    m_oEditInput.SetFocus();
    m_oEditInput.SetSelNone();
    m_oEditInput.SetModify(FALSE);

    return;
}
