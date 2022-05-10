#include "stdafx.h"
#include "modules/window/app/dlg_window_filters.h"

CDlgWindowFilters::CDlgWindowFilters()
{
    m_vFilterFactories.push_back(CreateWindowFilterFactory(WINDOW_FILTER_TYPE_CLASSNAME));
    m_vFilterFactories.push_back(CreateWindowFilterFactory(WINDOW_FILTER_TYPE_TITLE));
    m_vFilterFactories.push_back(CreateWindowFilterFactory(WINDOW_FILTER_TYPE_PID));
    m_vFilterFactories.push_back(CreateWindowFilterFactory(WINDOW_FILTER_TYPE_TID));
    m_vFilterFactories.push_back(CreateWindowFilterFactory(WINDOW_FILTER_TYPE_PROCESS_NAME));
}

CDlgWindowFilters::~CDlgWindowFilters()
{
    
}

VOID
CDlgWindowFilters::SetWindowFilters(const std::vector<CComPtr<IWindowFilter>> &vFilters)
{
    m_vFilters = vFilters;
    return;
}

VOID
CDlgWindowFilters::GetWindowFilters(std::vector<CComPtr<IWindowFilter>> &vFilters)
{
    vFilters = m_vFilters;
    return;
}

LRESULT
CDlgWindowFilters::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_oComboFilterType.Attach(GetDlgItem(IDC_COMBO_WINDOW_FILTER_TYPE));
    m_oComboFilterRelation.Attach(GetDlgItem(IDC_COMBO_WINDOW_FILTER_RELATION));
    m_oComboFilterValue.SubclassWindow(GetDlgItem(IDC_COMBO_WINDOW_FILTER_VALUE));
    m_oComboFilterOp.Attach(GetDlgItem(IDC_COMBO_WINDOW_FILTER_OP));

    DWORD i = 0;
    std::vector<CComPtr<IWindowFilterFactory>>::iterator oFactoryIt = m_vFilterFactories.begin();
    while(oFactoryIt != m_vFilterFactories.end()) {
        DWORD nFilterType = 0;
        (*oFactoryIt)->GetType(&nFilterType);

        CString strName = TranslateWindowFilterType(nFilterType);

        COMBOBOXEXITEM oItem = {0};
        oItem.mask = CBEIF_TEXT | CBEIF_LPARAM;
        oItem.iItem = i;
        oItem.pszText = strName.GetBuffer();
        oItem.lParam = (LPARAM)((*oFactoryIt).p);
        m_oComboFilterType.InsertItem(&oItem);

        ++i;
        ++oFactoryIt;
    }

    if(m_vFilterFactories.size() > 0) {
        m_oComboFilterType.SetCurSel(0);
    }

    ReloadFilterTypeInfo();

    m_oListFilters.SubclassWindow(GetDlgItem(IDC_LIST_WINDOW_FILTER));
    m_oListFilters.ModifyStyle(0, LVS_SINGLESEL, 0);
    m_oListFilters.SetExtendedListViewStyle(m_oListFilters.GetExtendedListViewStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

    LVCOLUMN oColumn = {0};
    oColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
    oColumn.fmt = LVCFMT_LEFT;

    oColumn.pszText = _T("类型");
    oColumn.iSubItem = 0;
    oColumn.cx = 100;
    m_oListFilters.InsertColumn(0, &oColumn);

    oColumn.pszText = _T("关系");
    oColumn.iSubItem = 0;
    oColumn.cx = 60;
    m_oListFilters.InsertColumn(2, &oColumn);

    oColumn.pszText = _T("值");
    oColumn.iSubItem = 0;
    oColumn.cx = 300;
    m_oListFilters.InsertColumn(3, &oColumn);

    oColumn.pszText = _T("动作");
    oColumn.iSubItem = 0;
    oColumn.cx = 100;
    m_oListFilters.InsertColumn(4, &oColumn);

    ReloadFilterList();

    return TRUE;
}

VOID
CDlgWindowFilters::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgWindowFilters::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDOK);
}

VOID
CDlgWindowFilters::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
}

VOID
CDlgWindowFilters::OnCmdComboWindowFilterType(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    if(nNotifyCode != 1) {
        return;
    }

    ReloadFilterTypeInfo();

    return;
}

VOID
CDlgWindowFilters::OnCmdBtnWindowFilterAdd(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CComPtr<IWindowFilterFactory> pFilterFactory = GetSelectedFilterFactory();
    ASSERT_RETURN_VOID(NULL != pFilterFactory);

    DWORD nRelation = 0, nOp = 0, nValueData = 0;
    CString strValue;
    if(!GetSelectedFilterRelation(nRelation) || !GetInputFilterValue(strValue) || !GetSelectedFilterOp(nOp)) {
        return;
    }

    if(strValue == _T("")) {
        return;
    }

    CComPtr<IWindowFilter> spFilter;
    if(FAILED(pFilterFactory->CreateWindowFilter(nRelation, CComBSTR(strValue), nOp, &spFilter)) || NULL == spFilter) {
        return;
    }

    m_vFilters.push_back(spFilter);

    m_oComboFilterValue.GetEditCtrl().SetWindowText(_T(""));

    ReloadFilterList();

    return;
}

VOID
CDlgWindowFilters::OnCmdBtnWindowFilterRemove(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    int nSelectIndex = m_oListFilters.GetSelectedIndex();
    if(0 > nSelectIndex) {
        return;
    }

    IWindowFilter *pFilter = (IWindowFilter *)m_oListFilters.GetItemData(nSelectIndex);
    std::vector<CComPtr<IWindowFilter>>::iterator oFilterIt = m_vFilters.begin();
    while(oFilterIt != m_vFilters.end()) {
        if(pFilter == (*oFilterIt).p) {
            m_vFilters.erase(oFilterIt);
            break;
        }
        ++oFilterIt;
    }

    ReloadFilterList();

    return;
}

BOOL
CDlgWindowFilters::ReloadFilterTypeInfo()
{
    m_oComboFilterRelation.ResetContent();
    m_oComboFilterValue.ResetContent();
    m_oComboFilterOp.ResetContent();

    CComPtr<IWindowFilterFactory> pFactory = GetSelectedFilterFactory();
    if(NULL == pFactory) {
        return FALSE;
    }

    IWindowFilter **ppFilters = NULL;
    DWORD nFilterNum = (DWORD)m_vFilters.size();
    if(nFilterNum > 0) {
        ppFilters = &(m_vFilters[0].p);
    }
    pFactory->PrepareSupportedData(ppFilters, nFilterNum);

    DWORD i = 0, nCount = 0;
    std::vector<DWORD> vSupportRelations;
    pFactory->GetSupportedRelationCount(&nCount);
    if(0 < nCount) {
        vSupportRelations.resize(nCount);
        pFactory->GetSupportedRelations(&(vSupportRelations[0]), nCount);
        for(i = 0; i < nCount; ++i) {
            CString strName = TranslateWindowFilterRelation(vSupportRelations[i]);
            COMBOBOXEXITEM oItem = {0};
            oItem.mask = CBEIF_TEXT | CBEIF_LPARAM;
            oItem.iItem = i;
            oItem.pszText = strName.GetBuffer();
            oItem.lParam = (LPARAM)(vSupportRelations[i]);
            m_oComboFilterRelation.InsertItem(&oItem);
        }
        m_oComboFilterRelation.SetCurSel(0);
    }

    nCount = 0;
    std::vector<CComBSTR> vSupportValues;
    std::vector<DWORD> vSupportValueDatas;
    pFactory->GetSupportedValueCount(&nCount);
    if(0 < nCount) {
        vSupportValues.resize(nCount, NULL);
        vSupportValueDatas.resize(nCount, 0);
        pFactory->GetSupportedValues(&(vSupportValues[0].m_str), &(vSupportValueDatas[0]), nCount);
        for(i = 0; i < nCount; ++i) {
            COMBOBOXEXITEM oItem = {0};
            oItem.mask = CBEIF_TEXT | CBEIF_LPARAM;
            oItem.iItem = i;
            oItem.pszText = vSupportValues[i];
            oItem.lParam = (LPARAM)vSupportValueDatas[i];
            m_oComboFilterValue.InsertItem(&oItem);
        }
    }

    m_oComboFilterValue.TakeSnapshot();

    nCount = 0;
    std::vector<DWORD> vSupportOps;
    pFactory->GetSupportedOpCount(&nCount);
    if(0 < nCount) {
        vSupportOps.resize(nCount);
        pFactory->GetSupportedOps(&(vSupportOps[0]), nCount);
        for(i = 0; i < nCount; ++i) {
            CString strName = TranslateWindowFilterOp(vSupportOps[i]);
            COMBOBOXEXITEM oItem = {0};
            oItem.mask = CBEIF_TEXT | CBEIF_LPARAM;
            oItem.iItem = i;
            oItem.pszText = strName.GetBuffer();
            oItem.lParam = (LPARAM)(vSupportOps[i]);
            m_oComboFilterOp.InsertItem(&oItem);
        }
        m_oComboFilterOp.SetCurSel(0);
    }

    pFactory->ClearSupportedData();

    return TRUE;
}

CComPtr<IWindowFilterFactory>
CDlgWindowFilters::GetSelectedFilterFactory()
{
    int nTypeIndex = m_oComboFilterType.GetCurSel();
    if(0 > nTypeIndex) {
        return NULL;
    }

    IWindowFilterFactory *pFactory = (IWindowFilterFactory *)m_oComboFilterType.GetItemDataPtr(nTypeIndex);
    std::vector<CComPtr<IWindowFilterFactory>>::iterator oFactoryIt = m_vFilterFactories.begin();
    while(oFactoryIt != m_vFilterFactories.end()) {
        if((*oFactoryIt).p == pFactory) {
            return (*oFactoryIt);
        }
        ++oFactoryIt;
    }

    return NULL;
}

BOOL
CDlgWindowFilters::GetSelectedFilterRelation(DWORD &nRelation)
{
    nRelation = 0;

    int nFilterIndex = m_oComboFilterRelation.GetCurSel();
    if(0 > nFilterIndex) {
        return FALSE;
    }

    nRelation = (DWORD)m_oComboFilterRelation.GetItemData(nFilterIndex);

    return TRUE;
}

BOOL
CDlgWindowFilters::GetInputFilterValue(CString &strValue)
{
    strValue = _T("");

    CEdit oEditValue = m_oComboFilterValue.GetEditCtrl();
    oEditValue.GetWindowText(strValue);

    return TRUE;
}

BOOL
CDlgWindowFilters::GetSelectedFilterOp(DWORD &nOp)
{
    nOp = 0;

    int nFilterIndex = m_oComboFilterOp.GetCurSel();
    if(0 > nFilterIndex) {
        return FALSE;
    }

    nOp = (DWORD)m_oComboFilterOp.GetItemData(nFilterIndex);

    return TRUE;
}

BOOL
CDlgWindowFilters::ReloadFilterList()
{
    m_oListFilters.SetRedraw(FALSE);
    m_oListFilters.DeleteAllItems();

    size_t i = 0, nCount = m_vFilters.size();
    for(i = 0; i < nCount; ++i) {
        DWORD nFilterType = 0, nRelation = 0, nOp = 0, nValueData = 0;
        CComBSTR strValue;

        if(NULL != m_vFilters[i]) {
            m_vFilters[i]->GetType(&nFilterType);
            m_vFilters[i]->GetRelation(&nRelation);
            m_vFilters[i]->GetValue(&strValue, &nValueData);
            m_vFilters[i]->GetOp(&nOp);

            m_oListFilters.InsertItem(LVIF_TEXT|LVIF_IMAGE|LVIF_PARAM, 0, _T(""), 0, 0, -1, (LPARAM)(m_vFilters[i].p));
            m_oListFilters.SetItemText(0, 0, TranslateWindowFilterType(nFilterType));
            m_oListFilters.SetItemText(0, 1, TranslateWindowFilterRelation(nRelation));
            m_oListFilters.SetItemText(0, 2, strValue);
            m_oListFilters.SetItemText(0, 3, TranslateWindowFilterOp(nOp));
        }
    }

    m_oListFilters.SetRedraw(TRUE);

    return TRUE;
}