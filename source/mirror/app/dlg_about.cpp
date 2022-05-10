#include "stdafx.h"
#include "app/dlg_about.h"
#include "modules/builtin/component_update_service.h"
#include "mirror_license.h"

#define MIRROR_ABOUT_INTRO      \
    _T("Mirror��Ŀ���ǳ�Ϊһ���㹻��ɫ���Կ�����Ա�㹻�ѺõĹ۲촰�ڵ���Ϊ��С���ߡ�\r\n")  \
    _T("���ڵ�Mirror���ܼ򵥣�ֻ��������Ҫ���ܣ����ڲ������ȼ�����ϣ�����Ժ������ǿ��������\r\n")    \
    _T("����Mirror��һ�������ĺ��ӣ����������ΪSpy++��������ʱ������ӹ���Hook������Ϣ�Ĺ��ܡ�\r\n")   \
    _T("- �鿴/�޸Ĵ��ڻ�����Ϣ\r\n")   \
    _T("- �鿴�������ڵ��߳���Ϣ\r\n")  \
    _T("- �鿴/������ʾ/����ǰ�����ڣ����㴰�ڣ������\r\n")  \
    _T("- �鿴��������Ϣ\r\n")  \
    _T("- �鿴/�޸Ĵ���Prop\r\n")   \
    _T("- ���ʹ�����Ϣ�����ṩһЩ���õĴ�����ص�Api����\r\n") \
    _T("- ģ����꣬��������\r\n")  \
    _T("- �鿴/�޸�DWM��Ϣ�������Լ����ü������еĴ���DWM������\r\n")   \
    _T("\r\n")  \
    _T("���������ᵽ�����еĹ��ܣ������Զ��������ȼ������������л�����е�Tab\r\n") \
    _T("Ӧ������֮���µ��ȼ��ᱻ�Զ�������mirror.ini�����ļ��С�\r\n")    \
    _T("\r\n")  \
    _T("============================ �������ķָ��� ============================\r\n")  \
    _T("\r\n")  \
    _T("Mirror��һ���ú��ӣ�����������Զ��ѣ������벻Ҫ�����޸Ĵ���������߽��䵱����ҵ���������\r\n")    \
    _T("��Ȼ��ӭ��Ҹ��ֽ�������ͬ�����������������ʲô����ͽ��飬����ʱ������ϵ��\r\n")    \
    _T("���䣺iambigasp@gmail.com\r\n") \
    _T("�������ҵĲ���תת��bigasp.com\r\n")    \
    _T(":D~\r\n")   \
    _T("\r\n")  \
    _T("============================ �������ķָ��� ============================\r\n")  \
    _T("\r\n")

CDlgAbout::CDlgAbout()
{

}

CDlgAbout::~CDlgAbout()
{

}

LRESULT
CDlgAbout::OnInitDialog(HWND hWnd, LPARAM lInitData)
{
    CString strAboutVersion;
    strAboutVersion.Format(_T("Mirror v%lu.%lu %s"), MIRROR_VERSION_MAJOR, MIRROR_VERSION_MINOR, _T(MIRROR_VERSION_SPECIAL_STRING));

    CStatic oStaAboutVersion = GetDlgItem(IDC_STATIC_ABOUT_VERSION);
    if(oStaAboutVersion.IsWindow()) {
        oStaAboutVersion.SetWindowText(strAboutVersion);
    }

    CString strAboutContent;
    strAboutContent.Format(_T("Mirror v%lu.%lu %s - Windows������������\r\n%s\r\n%s"),
        MIRROR_VERSION_MAJOR, MIRROR_VERSION_MINOR, _T(MIRROR_VERSION_SPECIAL_STRING), MIRROR_ABOUT_INTRO, MIRROR_USER_AGREEMENT);

    CEdit oAbout = GetDlgItem(IDC_EDIT_ABOUT);
    if(oAbout.IsWindow()) {
        oAbout.SetWindowText(strAboutContent);
    }

    return TRUE;
}

VOID
CDlgAbout::OnShowWindow(BOOL bShow, int nStatus)
{
    SetMsgHandled(FALSE);
    return;
}

VOID
CDlgAbout::OnClose()
{
    SetMsgHandled(FALSE);
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgAbout::OnOK(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDOK);
    return;
}

VOID
CDlgAbout::OnCancel(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    EndDialog(IDCANCEL);
    return;
}

VOID
CDlgAbout::OnAboutCheckUpdate(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    CComPtr<IUpdateService> pUpdateService = GetComponentInterface<IUpdateService>(COMPONENT_UPDATE_SERVICE);
    if(NULL == pUpdateService) {
        return;
    }

    if(FAILED(pUpdateService->CheckUpdate(FALSE))) {
        MLOG_INFO("UpdateService", "������ʧ�ܣ� Error Code: %lu", ::GetLastError());
    }

    return;
}

VOID
CDlgAbout::OnVisitWebsite(UINT nNotifyCode, UINT nId, HWND hWndCtrl)
{
    switch(nId) {
    case IDC_BUTTON_ABOUT_DONATE:
        ::ShellExecute(NULL, _T("open"), _T("https://me.alipay.com/bigasp"), NULL, NULL, SW_SHOW);
        break;
    case IDC_BUTTON_ABOUT_GOTO_HOME:
        ::ShellExecute(NULL, _T("open"), _T("https://bitbucket.org/bigasp/mirror"), NULL, NULL, SW_SHOW);
        break;
    case IDC_BUTTON_ABOUT_GOTO_BLOG:
        ::ShellExecute(NULL, _T("open"), _T("http://bigasp.com"), NULL, NULL, SW_SHOW);
        break;
    }

    return;
}
