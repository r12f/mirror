#pragma once

#define COMPONENT_CONFIG                        _T("com.config")
#define COMPONENT_HOTKEY                        _T("com.hotkey")
#define COMPONENT_LOG                           _T("com.log")
#define COMPONENT_THREAD_POOL                   _T("com.thread_pool")
#define COMPONENT_HTTP_DOWNLOADER               _T("com.http_downloader")
#define COMPONENT_UPDATE_SERVICE                _T("com.update_service")
#define COMPONENT_CODE_INJECTOR                 _T("com.code_injector")
#define COMPONENT_WINDOW_HIGHLIGHT_SERVICE      _T("com.window.highlight_service")

// 所有的UIComponent都不是真的使用引用计数来控制生命周期的，所以请不要保存UIComponent的指针，需要的时候临时去拿
#define COMPONENT_UI_DLG_MAIN                   _T("com.ui.dlg_main")
#define COMPONENT_UI_DLG_WINDOW                 _T("com.ui.dlg_window")
#define COMPONENT_UI_DLG_WINDOW_LIST            _T("com.ui.dlg_window_list")
#define COMPONENT_UI_DLG_WINDOW_PROPERTIES      _T("com.ui.dlg_window_properties")
#define COMPONENT_UI_DLG_WINDOW_BASIC_INFO      _T("com.ui.dlg_window_basic_info")
#define COMPONENT_UI_DLG_WINDOW_CLASS           _T("com.ui.dlg_window_class")
#define COMPONENT_UI_DLG_WINDOW_DWM             _T("com.ui.dlg_window_dwm")
#define COMPONENT_UI_DLG_WINDOW_DWM_OP          _T("com.ui.dlg_window_dwm_op")
#define COMPONENT_UI_DLG_WINDOW_DWM_TIMING      _T("com.ui.dlg_window_dwm_timing")
#define COMPONENT_UI_DLG_WINDOW_INPUT           _T("com.ui.dlg_window_input")
#define COMPONENT_UI_DLG_WINDOW_INPUT_KEY_MOUSE _T("com.ui.dlg_window_input_key_mouse")
#define COMPONENT_UI_DLG_WINDOW_INPUT_BATCH     _T("com.ui.dlg_window_input_batch")
#define COMPONENT_UI_DLG_WINDOW_OP              _T("com.ui.dlg_window_op")
#define COMPONENT_UI_DLG_WINDOW_PROP            _T("com.ui.dlg_window_prop")