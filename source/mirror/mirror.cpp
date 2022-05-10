#include "stdafx.h"
#include <list>
#include "modules/component_id.h"
#include "ui/layout.h"
#include "modules/builtin/component_config.h"
#include "base/string_number_convertions.h"
#include "base/system/process_control.h"
#include "app/dlg_main.h"
#include "app/dlg_user_agreement.h"
#include "modules/builtin/module_builtin.h"
#include "modules/window/module_window.h"
#include "modules/process/module_process.h"
#include "modules/compile/module_compile.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_IA64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='ia64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

CAppModule _Module;

void
RegisterMirModules()
{
    CComPtr<IMirModule> pModule;
    MirModuleWindow::CreateObject(__uuidof(IMirModule), (void **)&pModule);
    GetFramework()->RegisterModule(MODULE_WINDOW, pModule);

    pModule = NULL;
    MirModuleCompile::CreateObject(__uuidof(IMirModule), (void **)&pModule);
    GetFramework()->RegisterModule(MODULE_COMPILE, pModule);

    pModule = NULL;
    MirModuleProcess::CreateObject(__uuidof(IMirModule), (void **)&pModule);
    GetFramework()->RegisterModule(MODULE_PROCESS, pModule);

    pModule = NULL;
    MirModuleBuiltin::CreateObject(__uuidof(IMirModule), (void **)&pModule);
    GetFramework()->RegisterModule(MODULE_BUILTIN, pModule);

    return;
}

void 
InitFramework()
{
    CComQIPtr<IFrameworkControl> pFrameworkControl = GetFramework();
    if(NULL == pFrameworkControl) {
        return;
    }

    pFrameworkControl->Init();
    RegisterMirModules();
    pFrameworkControl->InitModules();

    return;
}

void
UninitFramework()
{
    CComQIPtr<IFrameworkControl> pFrameworkControl = GetFramework();
    if(NULL == pFrameworkControl) {
        return;
    }

    pFrameworkControl->UninitModules();
    pFrameworkControl->Uninit();

    return;
}

BOOL
CheckAgreement()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    INT nAgreeVersion = 0;
    if(FAILED(pConfig->GetInt(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_AGREEMENT_VERSION), &nAgreeVersion))) {
        return FALSE;
    }

    if(nAgreeVersion < MIRROR_VERSION_AGREEMENT) {
        return FALSE;
    }

    return TRUE;
}

BOOL
AgreeUserAgreement()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    if(FAILED(pConfig->SetInt(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_AGREEMENT_VERSION), MIRROR_VERSION_AGREEMENT))) {
        return FALSE;
    }

    return TRUE;
}

BOOL
GetLastWindowRect(RECT *pLastWindowRect, BOOL *pIsMaxmized)
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    if(NULL == pConfig) {
        return FALSE;
    }

    CComBSTR strLastWindowPos;
    if(FAILED(pConfig->GetString(CComBSTR(CONFIG_SECTION_MIRROR), CComBSTR(CONFIG_KEY_LAST_WINDOW_POSITION), &strLastWindowPos)) || NULL == strLastWindowPos) {
        return FALSE;
    }

    int nCord[5] = {0};
    if(!SplitUnicodeStringToNumber(strLastWindowPos, L',', nCord, 5)) {
        return FALSE;
    }

    pLastWindowRect->left = nCord[0];
    pLastWindowRect->top = nCord[1];
    pLastWindowRect->right = nCord[2];
    pLastWindowRect->bottom = nCord[3];
    *pIsMaxmized = nCord[4];

    return TRUE;
}

int
RunMirror(CMessageLoop &theLoop, int nCmdShow)
{
    if(!CheckAgreement()) {
        CDlgUserAgreement oDlg;
        switch(oDlg.DoModal()) {
        case IDOK:
            if(oDlg.IsAgreed()) {
                AgreeUserAgreement();
            } else {
                return 0;
            }
            break;
        case IDCANCEL:
            return 0;
        }
    }

    CDlgMain oDlgMain;
    if(oDlgMain.Create(NULL) == NULL) {
        return 0;
    }

    BOOL bIsMaxmized = FALSE;
    CRect oLastWindowRect;
    if(GetLastWindowRect(&oLastWindowRect, &bIsMaxmized)) {
        if(bIsMaxmized) {
            oDlgMain.ShowWindow(SW_MAXIMIZE);
        } else {
            oDlgMain.SetWindowPos(NULL, &oLastWindowRect, SWP_NOZORDER);
            oDlgMain.ShowWindow(SW_SHOW);
        }
    } else {
        oDlgMain.ShowWindow(nCmdShow);
    }

    return theLoop.Run();
}

// Code from MSDN
BOOL
GrantDebugPrivilege()
{
    BOOL             fSuccess    = FALSE;
    HANDLE           TokenHandle = NULL;
    TOKEN_PRIVILEGES TokenPrivileges;

    if (!OpenProcessToken(GetCurrentProcess(),
        TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
        &TokenHandle))
    {
        printf("Could not get the process token");
        goto Cleanup;
    }

    TokenPrivileges.PrivilegeCount = 1;

    if (!LookupPrivilegeValue(NULL,
        SE_DEBUG_NAME,
        &TokenPrivileges.Privileges[0].Luid))
    {
        printf("Couldn't lookup SeDebugPrivilege name");
        goto Cleanup;
    }

    TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(TokenHandle,
        FALSE,
        &TokenPrivileges,
        sizeof(TokenPrivileges),
        NULL,
        NULL))
    {
        printf("Could not revoke the debug privilege");
        goto Cleanup;
    }

    fSuccess = TRUE;

Cleanup:

    if (TokenHandle)
    {
        CloseHandle(TokenHandle);
    }

    return fSuccess;
}

int
Run(LPTSTR /*lpstrCmdLine*/ = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
    CMessageLoop theLoop;
    _Module.AddMessageLoop(&theLoop);

    GrantDebugPrivilege();

    InitFramework();

    InitLayout();

    int nRet = RunMirror(theLoop, nCmdShow);

    _Module.RemoveMessageLoop();

    UninitLayout();

    UninitFramework();

    return nRet;
}

int WINAPI
_tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
    HRESULT hRes = ::CoInitialize(NULL);
    // If you are running on NT 4.0 or higher you can use the following call instead to 
    // make the EXE free threaded. This means that calls come in on a random RPC thread.
    //	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
    ATLASSERT(SUCCEEDED(hRes));

    // this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
    ::DefWindowProc(NULL, 0, 0, 0L);

    AtlInitCommonControls(ICC_COOL_CLASSES | ICC_BAR_CLASSES);	// add flags to support other controls

    hRes = _Module.Init(NULL, hInstance);
    ATLASSERT(SUCCEEDED(hRes));

    int nRet = Run(lpstrCmdLine, nCmdShow);

    _Module.Term();
    ::CoUninitialize();

    return nRet;
}
