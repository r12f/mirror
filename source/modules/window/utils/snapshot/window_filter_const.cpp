#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_const.h"

CString
TranslateWindowFilterType(DWORD nType)
{
    switch(nType) {
    case WINDOW_FILTER_TYPE_CLASSNAME:
        return _T("窗口类");
    case WINDOW_FILTER_TYPE_TITLE:
        return _T("窗口标题");
    case WINDOW_FILTER_TYPE_PID:
        return _T("进程ID");
    case WINDOW_FILTER_TYPE_TID:
        return _T("线程ID");
    case WINDOW_FILTER_TYPE_PROCESS_NAME:
        return _T("进程名");
    }

    return _T("");
}

CString
TranslateWindowFilterRelation(DWORD nRelation)
{
    switch(nRelation) {
    case WINDOW_FILTER_RELATION_EQ:
        return _T("=");
    case WINDOW_FILTER_RELATION_NE:
        return _T("!=");
    case WINDOW_FILTER_RELATION_GT:
        return _T(">");
    case WINDOW_FILTER_RELATION_GE:
        return _T(">=");
    case WINDOW_FILTER_RELATION_LT:
        return _T("<");
    case WINDOW_FILTER_RELATION_LE:
        return _T("<=");
    case WINDOW_FILTER_RELATION_CONTAIN:
        return _T("包含");
    case WINDOW_FILTER_RELATION_NOT_CONTAIN:
        return _T("不包含");
    }

    return _T("");
}

CString
TranslateWindowFilterOp(DWORD nOp)
{
    switch(nOp) {
    case WINDOW_FILTER_OP_INCLUDE:
        return _T("显示");
    case WINDOW_FILTER_OP_EXCLUDE:
        return _T("不显示");
    }

    return _T("");
}