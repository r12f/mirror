#include "stdafx.h"
#include "modules/builtin/module_builtin.h"
#include "modules/builtin/config/config.h"
#include "modules/builtin/hotkey/hotkey_manager.h"
#include "modules/builtin/log/log_core.h"
#include "modules/builtin/thread_pool/tp_core.h"
#include "modules/builtin/net/http/http_downloader.h"
#include "modules/builtin/update/update_service.h"
#include "modules/builtin/code_injector/code_injector.h"
#include "modules/framework.h"

MirModuleBuiltin::MirModuleBuiltin()
{

}

MirModuleBuiltin::~MirModuleBuiltin()
{

}

STDMETHODIMP
MirModuleBuiltin::Create()
{
    CComPtr<IUnknown> spUnk;
    CConfig::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_CONFIG), spUnk);

    spUnk = NULL;
    HotkeyManager::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_HOTKEY), spUnk);

    spUnk = NULL;
    LogCore::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_LOG), spUnk);

    spUnk = NULL;
    TPCore::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_THREAD_POOL), spUnk);

    spUnk = NULL;
    HttpDownloader::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_HTTP_DOWNLOADER), spUnk);

    spUnk = NULL;
    UpdateService::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_UPDATE_SERVICE), spUnk);

    spUnk = NULL;
    CodeInjector::CreateObject(__uuidof(IUnknown), (void **)&spUnk);
    GetFramework()->RegisterComponent(CComBSTR(COMPONENT_CODE_INJECTOR), spUnk);

    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    ASSERT_RETURN(NULL != pConfig, E_FAIL);
    pConfig->Load();

    return S_OK;
}

STDMETHODIMP
MirModuleBuiltin::Destroy()
{
    CComPtr<IConfig> pConfig = GetComponentInterface<IConfig>(COMPONENT_CONFIG);
    ASSERT_RETURN(NULL != pConfig, E_FAIL);
    pConfig->Save();

    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_CONFIG));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_HOTKEY));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_LOG));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_THREAD_POOL));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_HTTP_DOWNLOADER));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_UPDATE_SERVICE));
    GetFramework()->UnregisterComponent(CComBSTR(COMPONENT_CODE_INJECTOR));

    return S_OK;
}

STDMETHODIMP
MirModuleBuiltin::Init()
{
    return S_OK;
}

STDMETHODIMP
MirModuleBuiltin::Uninit()
{
    return S_OK;
}


STDMETHODIMP
MirModuleBuiltin::RequestUI(HWND hMainWnd, BSTR *pTitle, HWND *pUI)
{
    ASSERT_RETURN(NULL != pTitle, E_INVALIDARG);
    ASSERT_RETURN(NULL != pUI, E_INVALIDARG);

    // 创建窗口
    m_oDlgConfig.Create(hMainWnd);
    ASSERT_RETURN(m_oDlgConfig.IsWindow(), E_FAIL);

    *pTitle = CComBSTR(L"设置和其他").Detach();
    *pUI = m_oDlgConfig.m_hWnd;

    return S_OK;
}

HRESULT
MirModuleBuiltin::CreateInstance(REFGUID pGUID, void **pObject)
{
    return E_NOINTERFACE;
}