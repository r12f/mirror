#include "stdafx.h"
#include "modules/window/utils/style_translator.h"
#include "modules/window/app/properties/dlg_window_class.h"
#include "modules/window/app/window_app_utils.h"

CDlgWindowClass::CDlgWindowClass()
    : CLayout(IDD)
    , CUIControl(this)
{
    GetFramework()->RegisterComponent(COMPONENT_UI_DLG_WINDOW_CLASS, this);
}

CDlgWindowClass::~CDlgWindowClass()
{
    GetFramework()->UnregisterComponent(COMPONENT_UI_DLG_WINDOW_CLASS);
}

LRESULT
CDlgWindowClass::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    m_vEditInfos[WBI_LIST_CLASS_NAME_INDEX] = GetDlgItem(IDC_EDIT_WCI_CLASS_NAME);
    m_vEditInfos[WBI_LIST_CLASS_ATOM_INDEX] = GetDlgItem(IDC_EDIT_WCI_ATOM);
    m_vEditInfos[WBI_LIST_CLASS_STYLE_INDEX] = GetDlgItem(IDC_EDIT_WCI_CLASS_STYLE);
    m_vEditInfos[WBI_LIST_CLASS_WNDPROC_INDEX] = GetDlgItem(IDC_EDIT_WCI_WND_PROC);
    m_vEditInfos[WBI_LIST_CLASS_INSTANCE_INDEX] = GetDlgItem(IDC_EDIT_WCI_INSTANCE);
    m_vEditInfos[WBI_LIST_CLASS_ICON_INDEX] = GetDlgItem(IDC_EDIT_WCI_ICON);
    m_vEditInfos[WBI_LIST_CLASS_CURSOR_INDEX] = GetDlgItem(IDC_EDIT_WCI_CURSOR);
    m_vEditInfos[WBI_LIST_CLASS_BACKGROUND_INDEX] = GetDlgItem(IDC_EDIT_WCI_BG_BRUSH);
    m_vEditInfos[WBI_LIST_CLASS_MENU_INDEX] = GetDlgItem(IDC_EDIT_WCI_MENU);
    m_vEditInfos[WBI_LIST_CLASS_REGISTER_MODULE_INDEX] = GetDlgItem(IDC_EDIT_WCI_REGISTER_MODULE);
    m_vEditInfos[WBI_LIST_CLASS_EXTRA_DATA_INDEX] = GetDlgItem(IDC_EDIT_WCI_CLASS_EXTRA_DATA);
    m_vEditInfos[WBI_LIST_CLASS_WND_EXTRA_DATA_INDEX] = GetDlgItem(IDC_EDIT_WCI_WINDOW_EXTRA_DATA);

    m_oListClassStyle = GetDlgItem(IDC_LIST_WCI_CLASS_STYLE);

    m_oEditClassExtraData = GetDlgItem(IDC_EDIT_WCI_CLASS_EXTRA_DATA_CONTENT);
    m_oEditWindowExtraData = GetDlgItem(IDC_EDIT_WCI_WINDOW_EXTRA_DATA_CONTENT);

    return TRUE;
}

VOID
CDlgWindowClass::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}


VOID
CDlgWindowClass::OnClose()
{
    SetMsgHandled(FALSE);
    DestroyWindow();
    return;
}

VOID
CDlgWindowClass::OnDestroy()
{
    SetMsgHandled(FALSE);
    return;
}

BOOL
CDlgWindowClass::ResetData(DWORD nParam)
{
    m_strClassName = _T("");
    m_nAtom = 0;
    m_strAtomName = _T("");
    memset(&m_oClassInfo, 0, sizeof(m_oClassInfo));
    m_hModule = NULL;
    m_vClassExtraData.clear();
    m_vWindowExtraData.clear();

    return TRUE;
}

BOOL
CDlgWindowClass::FetchData(DWORD nParam)
{
    CWindow oWindow = GetWatchWindow();
    if(NULL == oWindow.m_hWnd || !oWindow.IsWindow()) {
        return FALSE;
    }

    // 窗口类
    TCHAR szClassName[1024] = {0};
    ::GetClassName(oWindow.m_hWnd, szClassName, 1023);
    m_strClassName = szClassName;

    // ATOM
    m_nAtom = (ATOM)::GetClassLongPtr(oWindow.m_hWnd, GCW_ATOM);
    TCHAR szAtomName[1024] = {0};
    ::GetAtomName(m_nAtom, szAtomName, 1023);
    m_strAtomName = szAtomName;

    // 获取窗口类信息，这里不能使用GetClassInfoEx来获取，这个API只能获取部分的ClassInfo
    m_oClassInfo.style = (UINT)::GetClassLongPtr(oWindow.m_hWnd, GCL_STYLE);
    m_oClassInfo.lpfnWndProc = (WNDPROC)::GetClassLongPtr(oWindow.m_hWnd, GCLP_WNDPROC);
    m_oClassInfo.cbClsExtra = (int)::GetClassLongPtr(oWindow.m_hWnd, GCL_CBCLSEXTRA);
    m_vClassExtraData.resize(m_oClassInfo.cbClsExtra);
    int nClsExtraIndex = 0;
    for(nClsExtraIndex = 0; nClsExtraIndex < m_oClassInfo.cbClsExtra; ++nClsExtraIndex) {
        m_vClassExtraData[nClsExtraIndex] = (unsigned char)::GetClassLongPtr(oWindow.m_hWnd, nClsExtraIndex);
    }

    m_oClassInfo.cbWndExtra = (int)::GetClassLongPtr(oWindow.m_hWnd, GCL_CBWNDEXTRA);
    m_vWindowExtraData.resize(m_oClassInfo.cbWndExtra);
    int nWndExtraIndex = 0;
    for(nWndExtraIndex = 0; nWndExtraIndex < m_oClassInfo.cbWndExtra; ++nWndExtraIndex) {
        m_vWindowExtraData[nWndExtraIndex] = (unsigned char)::GetClassLongPtr(oWindow.m_hWnd, nWndExtraIndex);
    }

   // m_oClassInfo.hInstance = ::GetClassLongPtr(oWindow.m_hWnd, GCLP_CBCLSEXTRA);
    m_oClassInfo.hIcon = (HICON)::GetClassLongPtr(oWindow.m_hWnd, GCLP_HICON);
    m_oClassInfo.hCursor = (HCURSOR)::GetClassLongPtr(oWindow.m_hWnd, GCLP_HCURSOR);
    m_oClassInfo.hbrBackground = (HBRUSH)::GetClassLongPtr(oWindow.m_hWnd, GCLP_HBRBACKGROUND);
    m_oClassInfo.lpszMenuName = (LPCWSTR)::GetClassLongPtr(oWindow.m_hWnd, GCLP_MENUNAME);
    m_oClassInfo.hIconSm = (HICON)::GetClassLongPtr(oWindow.m_hWnd, GCLP_HICONSM);

    // Module
    m_hModule = (HMODULE)::GetClassLongPtr(oWindow.m_hWnd, GCLP_HMODULE);

    return TRUE;
}

BOOL
CDlgWindowClass::UpdateUI(DWORD nParam)
{
    // ClassName
    UpdateClassInfo(WBI_LIST_CLASS_NAME_INDEX, m_strClassName);

    // Atom
    CString strAtom;
    strAtom.Format(_T("[%08x] %s"), m_nAtom, m_strAtomName.GetBuffer());
    UpdateClassInfo(WBI_LIST_CLASS_ATOM_INDEX, strAtom);

    // 窗口样式
    CString strStyle;
    strStyle.Format(_T("%08x"), m_oClassInfo.style);
    UpdateClassInfo(WBI_LIST_CLASS_STYLE_INDEX, strStyle);

    // 窗口过程
    CString strWndProc;
    strWndProc.Format(_T("%08x"), m_oClassInfo.lpfnWndProc);
    UpdateClassInfo(WBI_LIST_CLASS_WNDPROC_INDEX, strWndProc);

    // Class Extra Data
    CString strClsExtraDataSize;
    strClsExtraDataSize.Format(_T("%ld"), m_oClassInfo.cbClsExtra);
    UpdateClassInfo(WBI_LIST_CLASS_EXTRA_DATA_INDEX, strClsExtraDataSize);

    CString strClassExtraData, strClassExtraDataChar;
    int nClsExtraIndex = 0;
    for(nClsExtraIndex = 0; nClsExtraIndex < m_oClassInfo.cbClsExtra; ++nClsExtraIndex) {
        strClassExtraDataChar.Format(_T("%02x "), m_vClassExtraData[nClsExtraIndex]);
        strClassExtraData += strClassExtraDataChar;
    }

    m_oEditClassExtraData.SetWindowText(strClassExtraData);

    // Window Extra Data
    CString strWindowExtra;
    strWindowExtra.Format(_T("%ld"), m_oClassInfo.cbWndExtra);
    UpdateClassInfo(WBI_LIST_CLASS_WND_EXTRA_DATA_INDEX, strWindowExtra);

    CString strWindowExtraData, strWindowExtraDataChar;
    int nWndExtraIndex = 0;
    for(nWndExtraIndex = 0; nWndExtraIndex < m_oClassInfo.cbWndExtra; ++nWndExtraIndex) {
        strWindowExtraDataChar.Format(_T("%02x "), m_vWindowExtraData[nWndExtraIndex]);
        strWindowExtraData += strWindowExtraDataChar;
    }

    m_oEditWindowExtraData.SetWindowText(strWindowExtraData);

    // Instance
    CString strInstance;
    strInstance.Format(_T("%08x"), m_oClassInfo.hInstance);
    UpdateClassInfo(WBI_LIST_CLASS_INSTANCE_INDEX, strInstance);

    // Icon
    CString strIcon;
    strIcon.Format(_T("%08x"), m_oClassInfo.hIcon);
    UpdateClassInfo(WBI_LIST_CLASS_ICON_INDEX, strIcon);

    // Cursor
    CString strCursor;
    strCursor.Format(_T("%08x"), m_oClassInfo.hCursor);
    UpdateClassInfo(WBI_LIST_CLASS_CURSOR_INDEX, strCursor);

    // Background Brush
    CString strBGBrush;
    strBGBrush.Format(_T("%08x"), m_oClassInfo.hbrBackground);
    UpdateClassInfo(WBI_LIST_CLASS_BACKGROUND_INDEX, strBGBrush);

    // MenuName
    CString strMenuName = m_oClassInfo.lpszMenuName;
    if(strMenuName.GetLength() == 0) {
        strMenuName = _T("[None]");
    }
    UpdateClassInfo(WBI_LIST_CLASS_MENU_INDEX, strMenuName);

    // Module
    CString strRegModule;
    strRegModule.Format(_T("%08x"), m_hModule);
    UpdateClassInfo(WBI_LIST_CLASS_REGISTER_MODULE_INDEX, strRegModule);

    // Styles
    StyleInfoList vClassStyles;
    TranslateClassStyle(m_oClassInfo.style, vClassStyles);

    m_oListClassStyle.ResetContent();
    size_t i = 0, nCount = vClassStyles.size();
    for(i = 0; i < nCount; ++i) {
        m_oListClassStyle.AddString(vClassStyles[i].m_strName);
    }

    return TRUE;
}

BOOL
CDlgWindowClass::UpdateClassInfo(int nIndex, CString &strInfo)
{
    if(nIndex >= WBI_LIST_CLASS_INFO_NUM) {
        return FALSE;
    }

    m_vEditInfos[nIndex].SetWindowText(strInfo);

    return TRUE;
}