#pragma once

#include "modules/builtin/component_log.h"
#include "base/log/logger.h"

#ifdef _DEBUG
#define DMLOG(module, level, fmt, ...)          \
    do {                                        \
        WriteLogRecord(LOG_RECORD_TYPE_BASIC, level, _T(module), _T(__FILE__), _T(__FUNCTION__), __LINE__, _T(fmt), __VA_ARGS__);   \
        ATLASSERT(LOG_LEVEL_FATAL != level);    \
    } while(0)
#else
#define DMLOG(module, level, fmt, ...)   
#endif

#define DMLOG_DEBUG(module, fmt, ...)   DMLOG(module, LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)
#define DMLOG_INFO(module, fmt, ...)    DMLOG(module, LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define DMLOG_WARN(module, fmt, ...)    DMLOG(module, LOG_LEVEL_WARN, fmt, __VA_ARGS__)
#define DMLOG_ERROR(module, fmt, ...)   DMLOG(module, LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define DMLOG_FATAL(module, fmt, ...)   DMLOG(module, LOG_LEVEL_FATAL, fmt, __VA_ARGS__)

#define MLOG(module, level, fmt, ...)           \
    do {                                        \
        WriteLogRecord(LOG_RECORD_TYPE_BASIC, level, _T(module), _T(__FILE__), _T(__FUNCTION__), __LINE__, _T(fmt), __VA_ARGS__);   \
        ATLASSERT(LOG_LEVEL_FATAL != level);    \
    } while(0)

#define MLOG_DEBUG(module, fmt, ...)    MLOG(module, LOG_LEVEL_DEBUG, fmt, __VA_ARGS__)
#define MLOG_INFO(module, fmt, ...)     MLOG(module, LOG_LEVEL_INFO, fmt, __VA_ARGS__)
#define MLOG_WARN(module, fmt, ...)     MLOG(module, LOG_LEVEL_WARN, fmt, __VA_ARGS__)
#define MLOG_ERROR(module, fmt, ...)    MLOG(module, LOG_LEVEL_ERROR, fmt, __VA_ARGS__)
#define MLOG_FATAL(module, fmt, ...)    MLOG(module, LOG_LEVEL_FATAL, fmt, __VA_ARGS__)

#define MCHECK_RETURN(module, condition, fmt, ...)          \
    if(!(condition)) {                                      \
        DMLOG_FATAL(module, fmt, __VA_ARGS__);              \
        return ret;                                         \
    }

#define MCHECK_RETURN_VOID(module, condition, fmt, ...)     \
    if(!(condition)) {                                      \
        DMLOG_FATAL(module, fmt, __VA_ARGS__);              \
        return;                                             \
    }