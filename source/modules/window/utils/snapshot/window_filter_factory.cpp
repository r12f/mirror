#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_factory.h"
#include "modules/window/utils/snapshot/window_filter_pid.h"
#include "modules/window/utils/snapshot/window_filter_tid.h"
#include "modules/window/utils/snapshot/window_filter_class_name.h"
#include "modules/window/utils/snapshot/window_filter_title.h"
#include "modules/window/utils/snapshot/window_filter_process_name.h"

CComPtr<IWindowFilterFactory>
CreateWindowFilterFactory(DWORD nFilterType)
{
    CComPtr<IWindowFilterFactory> spFactory;
    switch(nFilterType) {
    case WINDOW_FILTER_TYPE_CLASSNAME:
        WindowFilterFactoryClassName::CreateObject(__uuidof(IWindowFilterFactory), (void **)&spFactory);
        break;
    case WINDOW_FILTER_TYPE_TITLE:
        WindowFilterFactoryTitle::CreateObject(__uuidof(IWindowFilterFactory), (void **)&spFactory);
        break;
    case WINDOW_FILTER_TYPE_PID:
        WindowFilterFactoryPid::CreateObject(__uuidof(IWindowFilterFactory), (void **)&spFactory);
        break;
    case WINDOW_FILTER_TYPE_TID:
        WindowFilterFactoryTid::CreateObject(__uuidof(IWindowFilterFactory), (void **)&spFactory);
        break;
    case WINDOW_FILTER_TYPE_PROCESS_NAME:
        WindowFilterFactoryProcessName::CreateObject(__uuidof(IWindowFilterFactory), (void **)&spFactory);
        break;
    }

    return spFactory;
}