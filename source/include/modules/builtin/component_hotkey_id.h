#pragma once

// Section: 基本
#define HOTKEY_SECTION_BASIC                            _T("基本")

// Section: 基本\切换标签
#define HOTKEY_SECTION_BASIC_SWITCH_TO_TAB                      _T("基本\\切换标签")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_LIST             _T("切换到：窗口管理->窗口列表")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INFO             _T("切换到：窗口管理->窗口信息")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_BASIC_INFO       _T("切换到：窗口管理->窗口信息->基本信息")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_CLASS            _T("切换到：窗口管理->窗口信息->窗口类")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_PROP             _T("切换到：窗口管理->窗口信息->窗口Prop")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_OP               _T("切换到：窗口管理->窗口信息->窗口操作")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT            _T("切换到：窗口管理->窗口信息->模拟输入")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT_KEYMOUSE   _T("切换到：窗口管理->窗口信息->模拟输入->鼠标键盘")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_INPUT_BATCH      _T("切换到：窗口管理->窗口信息->模拟输入->批量输入")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM              _T("切换到：窗口管理->窗口信息->DWM")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM_OP           _T("切换到：窗口管理->窗口信息->DWM->基本操作")
#define HOTKEY_NAME_BASIC_SWITCH_TO_TAB_WINDOW_DWM_TIMING       _T("切换到：窗口管理->窗口信息->DWM->性能数据")

// Section: 窗口
#define HOTKEY_SECTION_WINDOW                           _T("窗口")
#define HOTKEY_NAME_WINDOW_SELECT_WINDOW                _T("选择当前鼠标下的窗口")
#define HOTKEY_NAME_WINDOW_TRACK_WATCH_WINDOW           _T("高亮当前正在观察的窗口")

// Section: 窗口\基本信息
#define HOTKEY_SECTION_WINDOW_INFO                      _T("窗口\\基本信息")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_PARENT             _T("跳转至Parent")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_OWNER              _T("跳转至Owner")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_CHILD              _T("跳转至Child")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_HWNDPREV           _T("跳转至HwndPrev")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_HWNDNEXT           _T("跳转至HwndNext")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_FOCUS              _T("跳转至焦点窗口")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_ACTIVE             _T("跳转至激活窗口")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_CAPTURE            _T("跳转至鼠标捕获")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_FOREGROUND         _T("跳转至前景窗口")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_FOCUS       _T("跳转至全局焦点")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_ACTIVE      _T("跳转至全局激活")
#define HOTKEY_NAME_WINDOW_INFO_GOTO_GLOBAL_CAPTURE     _T("跳转至全局捕获")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_FOCUS             _T("高亮焦点窗口")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_ACTIVE            _T("高亮激活窗口")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_CAPTURE           _T("高亮鼠标捕获")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_FOREGROUND        _T("高亮前景窗口")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_FOCUS      _T("高亮全局焦点")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_ACTIVE     _T("高亮全局激活")
#define HOTKEY_NAME_WINDOW_INFO_TRACK_GLOBAL_CAPTURE    _T("高亮全局捕获")
#define HOTKEY_NAME_WINDOW_INFO_SET_PARENT              _T("设置父窗口")

// Section: 窗口\窗口Prop
#define HOTKEY_SECTION_WINDOW_PROP                      _T("窗口\\窗口Prop")
#define HOTKEY_NAME_WINDOW_PROP_CREATE_PROP             _T("创建Prop")

// Section: 窗口\窗口操作
#define HOTKEY_SECTION_WINDOW_OP                                _T("窗口\\窗口操作")
#define HOTKEY_NAME_WINDOW_OP_SEND_MESSAGE                      _T("发送消息")
#define HOTKEY_NAME_WINDOW_OP_SET_FOCUS                         _T("设置焦点")
#define HOTKEY_NAME_WINDOW_OP_SET_ACTIVE                        _T("设为活动窗口")
#define HOTKEY_NAME_WINDOW_OP_SET_FOREGROUND                    _T("设为前景窗口")
#define HOTKEY_NAME_WINDOW_OP_ENABLE_WINDOW_TRUE                _T("启用窗口")
#define HOTKEY_NAME_WINDOW_OP_ENABLE_WINDOW_FALSE               _T("禁用窗口")
#define HOTKEY_NAME_WINDOW_OP_ZORDER_HWND_TOPMOST               _T("Zorder: 置于顶层")
#define HOTKEY_NAME_WINDOW_OP_ZORDER_HWND_TOP                   _T("Zorder: 前置")
#define HOTKEY_NAME_WINDOW_OP_ZORDER_HWND_NOTOPMOST             _T("Zorder: 取消置顶")
#define HOTKEY_NAME_WINDOW_OP_ZORDER_HWND_BOTTOM                _T("Zorder: 置于底层")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOW               _T("ShowWindow: SW_SHOW")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWNORMAL         _T("ShowWindow: SW_SHOWNORMAL")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWDEFAULT        _T("ShowWindow: SW_SHOWDEFAULT")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_MAXIMIZE           _T("ShowWindow: SW_MAXIMIZE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWMAXIMIZED      _T("ShowWindow: SW_SHOWMAXIMIZED")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_MINIMIZE           _T("ShowWindow: SW_MINIMIZE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWMINIMIZED      _T("ShowWindow: SW_SHOWMINIMIZED")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWMINNOACTIVE    _T("ShowWindow: SW_SHOWMINNOACTIVE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_FORCEMINIMIZE      _T("ShowWindow: SW_FORCEMINIMIZE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_RESTORE            _T("ShowWindow: SW_RESTORE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWNA             _T("ShowWindow: SW_SHOWNA")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_SHOWNOACTIVATE     _T("ShowWindow: SW_SHOWNOACTIVATE")
#define HOTKEY_NAME_WINDOW_OP_SHOW_WINDOW_SW_HIDE               _T("ShowWindow: SW_HIDE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_CLOSE               _T("WM_SYSCOMMAND: SC_CLOSE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_CONTEXTHELP         _T("WM_SYSCOMMAND: SC_CONTEXTHELP")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_DEFAULT             _T("WM_SYSCOMMAND: SC_DEFAULT")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_HOTKEY              _T("WM_SYSCOMMAND: SC_HOTKEY")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_HSCROLL             _T("WM_SYSCOMMAND: SC_HSCROLL")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_KEYMENU             _T("WM_SYSCOMMAND: SC_KEYMENU")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_MAXIMIZE            _T("WM_SYSCOMMAND: SC_MAXIMIZE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_MINIMIZE            _T("WM_SYSCOMMAND: SC_MINIMIZE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_MONITORPOWER        _T("WM_SYSCOMMAND: SC_MONITORPOWER")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_MOUSEMENU           _T("WM_SYSCOMMAND: SC_MOUSEMENU")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_MOVE                _T("WM_SYSCOMMAND: SC_MOVE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_NEXTWINDOW          _T("WM_SYSCOMMAND: SC_NEXTWINDOW")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_PREVWINDOW          _T("WM_SYSCOMMAND: SC_PREVWINDOW")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_RESTORE             _T("WM_SYSCOMMAND: SC_RESTORE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_SCREENSAVE          _T("WM_SYSCOMMAND: SC_SCREENSAVE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_SIZE                _T("WM_SYSCOMMAND: SC_SIZE")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_TASKLIST            _T("WM_SYSCOMMAND: SC_TASKLIST")
#define HOTKEY_NAME_WINDOW_OP_SYSCOMMAND_SC_VSCROLL             _T("WM_SYSCOMMAND: SC_VSCROLL")
#define HOTKEY_NAME_WINDOW_OP_PARSE_WAIT_CHAIN                  _T("检测窗口所在线程的等待链")

// Section: 窗口\模拟输入
#define HOTKEY_SECTION_WINDOW_INPUT                     _T("窗口\\模拟输入")

// Section: 窗口\模拟输入\鼠标
#define HOTKEY_SECTION_WINDOW_INPUT_MOUSE               _T("窗口\\模拟输入\\鼠标")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_DOWN          _T("WM_*DOWN")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_UP            _T("WM_*UP")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_DBLCLK        _T("WM_*DBLCLK")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCDOWN        _T("WM_NC*DOWN")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCUP          _T("WM_NC*UP")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_WM_NCDBLCLK      _T("WM_NC*DBLCLK")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_CLICK            _T("模拟点击消息")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_DBLCLK           _T("模拟双击消息")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_REAL_CLICK       _T("模拟真实点击")
#define HOTKEY_NAME_WINDOW_INPUT_MOUSE_REAL_DBLCLK      _T("模拟真实双击")

// Section: 窗口\模拟输入\键盘
#define HOTKEY_SECTION_WINDOW_INPUT_KEY                 _T("窗口\\模拟输入\\键盘")
#define HOTKEY_NAME_WINDOW_INPUT_KEY_WM_DOWN            _T("WM_KEYDOWN")
#define HOTKEY_NAME_WINDOW_INPUT_KEY_WM_UP              _T("WM_KEYUP")
#define HOTKEY_NAME_WINDOW_INPUT_KEY_WM_CHAR            _T("WM_CHAR")
#define HOTKEY_NAME_WINDOW_INPUT_KEY_STROKE             _T("模拟输入消息")
#define HOTKEY_NAME_WINDOW_INPUT_KEY_REAL_STROKE        _T("模拟真实输入")

// Section: 窗口\DWM
#define HOTKEY_SECTION_WINDOW_DWM                                               _T("窗口\\DWM")
#define HOTKEY_NAME_WINDOW_DWM_ENABLE_COMPOSITION                               _T("启动/禁用DWM合成")
#define HOTKEY_NAME_WINDOW_DWM_EXTEND_FRAME                                     _T("Extend Frame Into Child Area")
#define HOTKEY_NAME_WINDOW_DWM_INVALIDATE_ICONIC_BITMAPS                        _T("Invalidate Iconic Bitmaps")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_USEWINDOWSTYLE                  _T("NC Render Policy: DWMNCRP_USEWINDOWSTYLE")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_DISABLED                        _T("NC Render Policy: DWMNCRP_DISABLED")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMNCRP_ENABLED                         _T("NC Render Policy: DWMNCRP_ENABLED")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_TRANSITIONS_FORCEDISABLED_ON      _T("Transition Disabled: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_TRANSITIONS_FORCEDISABLED_OFF     _T("Transition Disabled: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_ALLOW_NCPAINT_ON                  _T("Allow NC Paint: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_ALLOW_NCPAINT_OFF                 _T("Allow NC Paint: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_NONCLIENT_RTL_LAYOUT_ON           _T("NC Rtl Layout: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_NONCLIENT_RTL_LAYOUT_OFF          _T("NC Rtl Layout: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_FORCE_ICONIC_REPRESENTATION_ON    _T("Force Iconic Representation: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_FORCE_ICONIC_REPRESENTATION_OFF   _T("Force Iconic Representation: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_DEFAULT                       _T("Flip3D Policy: DWMFLIP3D_DEFAULT")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_EXCLUDEBELOW                  _T("Flip3D Policy: DWMFLIP3D_EXCLUDEBELOW")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMFLIP3D_EXCLUDEABOVE                  _T("Flip3D Policy: DWMFLIP3D_EXCLUDEABOVE")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_HAS_ICONIC_BITMAP_ON              _T("Has Iconic Bitmap: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_HAS_ICONIC_BITMAP_OFF             _T("Has Iconic Bitmap: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_DISALLOW_PEEK_ON                  _T("Disallow peek: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_DISALLOW_PEEK_OFF                 _T("Disallow peek: Off")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_EXCLUDED_FROM_PEEK_ON             _T("Exclude from peek: On")
#define HOTKEY_NAME_WINDOW_DWM_SET_ATTR_DWMWA_EXCLUDED_FROM_PEEK_OFF            _T("Exclude from peek: Off")