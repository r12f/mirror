#pragma once

#include <list>

class CFilterComboBoxEx :
    public CWindowImpl<CFilterComboBoxEx, CComboBoxEx>
{
    struct SnapshotItem {
        COMBOBOXEXITEM m_oItem;
        CString m_strText;
    };

    typedef std::list<SnapshotItem> SnapshotList;

    enum {
        FCBE_TIMER_ID_REFRESH_LIST  = 0x100,
    };

    class CFilterComboBoxExEdit :
        public CWindowImpl<CFilterComboBoxExEdit, CEdit>
    {
    public:
        CFilterComboBoxExEdit(CFilterComboBoxEx *pComboBox);
        virtual ~CFilterComboBoxExEdit();

        BEGIN_MSG_MAP_EX(CFilterComboBoxExEdit)
            MSG_WM_KEYUP(OnKeyDown)
            MSG_WM_KEYUP(OnKeyUp)
            MSG_WM_SETTEXT(OnSetText)
        END_MSG_MAP()

        VOID OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
        VOID OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
        LRESULT OnSetText(LPCTSTR pText);

    private:
        CFilterComboBoxEx *m_pComboBox;
    };

public:
    CFilterComboBoxEx();
    virtual ~CFilterComboBoxEx();

    BOOL SubclassWindow(HWND hWnd);
    HWND UnsubclassWindow(BOOL bForce = FALSE);

    // 在修改完ComboBox时，调用一次这个接口
    VOID TakeSnapshot();
    
    BEGIN_MSG_MAP_EX(CFilterComboBoxEx)
        MSG_WM_TIMER(OnTimer)
    END_MSG_MAP()

    VOID OnTimer(UINT_PTR nIDEvent);

private:
    SnapshotList m_vItemList;
    CFilterComboBoxExEdit m_oEditInput;
};