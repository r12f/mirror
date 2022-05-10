#pragma once

#define TP_INITIAL_WORKER_NUM   1

// TP Master消息
#define WM_TP_SCHEDULE_WORK                     (WM_USER + 0x100)

// TP Worker消息
#define WM_TP_DISPATCH_WORK                     (WM_USER + 0x100)
#define WM_TP_QUIT_WORKER                       (WM_USER + 0x101)

#define TP_WORKER_STATE_FREE    1
#define TP_WORKER_STATE_BUSY    2

#include "modules/builtin/component_thread_pool.h"