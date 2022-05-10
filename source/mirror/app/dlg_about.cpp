#include "stdafx.h"
#include "app/dlg_about.h"
#include "modules/builtin/component_update_service.h"
#include "mirror_license.h"

#define MIRROR_ABOUT_INTRO      \
    _T("Mirror的目标是成为一个足够绿色，对开发人员足够友好的观察窗口的行为的小工具。\r\n")  \
    _T("现在的Mirror还很简单，只有两个主要功能：窗口操作和热键管理，希望他以后会慢慢强大起来。\r\n")    \
    _T("但是Mirror是一个单纯的孩子，它并不想成为Spy++，所以暂时不会添加关于Hook窗口消息的功能。\r\n")   \
    _T("- 查看/修改窗口基本信息\r\n")   \
    _T("- 查看窗口所在的线程信息\r\n")  \
    _T("- 查看/高亮显示/设置前景窗口，焦点窗口，激活窗口\r\n")  \
    _T("- 查看窗口类信息\r\n")  \
    _T("- 查看/修改窗口Prop\r\n")   \
    _T("- 发送窗口消息，并提供一些常用的窗口相关的Api调用\r\n") \
    _T("- 模拟鼠标，键盘输入\r\n")  \
    _T("- 查看/修改DWM信息，可以自己设置几乎所有的窗口DWM的属性\r\n")   \
    _T("\r\n")  \
    _T("基本上面提到的所有的功能，都可以对其设置热键，甚至包括切换软件中的Tab\r\n") \
    _T("应用设置之后，新的热键会被自动保存在mirror.ini配置文件中。\r\n")    \
    _T("\r\n")  \
    _T("============================ 华丽丽的分隔线 ============================\r\n")  \
    _T("\r\n")  \
    _T("Mirror是一个好孩子，而且他将永远免费，所以请不要随意修改此软件，或者将其当作商业软件发布。\r\n")    \
    _T("当然欢迎大家各种交流，共同完善这款软件，如果有什么问题和建议，请随时与我联系。\r\n")    \
    _T("邮箱：iambigasp@gmail.com\r\n") \
    _T("或者来我的博客转转：bigasp.com\r\n")    \
    _T(":D~\r\n")   \
    _T("\r\n")  \
    _T("============================ 华丽丽的分隔线 ============================\r\n")  \
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
    strAboutContent.Format(_T("Mirror v%lu.%lu %s - Windows开发辅助工具\r\n%s\r\n%s"),
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
        MLOG_INFO("UpdateService", "检查更新失败！ Error Code: %lu", ::GetLastError());
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
