#include "stdafx.h"
#include "modules/window/utils/snapshot/window_filter_const.h"

CString
TranslateWindowFilterType(DWORD nType)
{
    switch(nType) {
    case WINDOW_FILTER_TYPE_CLASSNAME:
        return _T("������");
    case WINDOW_FILTER_TYPE_TITLE:
        return _T("���ڱ���");
    case WINDOW_FILTER_TYPE_PID:
        return _T("����ID");
    case WINDOW_FILTER_TYPE_TID:
        return _T("�߳�ID");
    case WINDOW_FILTER_TYPE_PROCESS_NAME:
        return _T("������");
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
        return _T("����");
    case WINDOW_FILTER_RELATION_NOT_CONTAIN:
        return _T("������");
    }

    return _T("");
}

CString
TranslateWindowFilterOp(DWORD nOp)
{
    switch(nOp) {
    case WINDOW_FILTER_OP_INCLUDE:
        return _T("��ʾ");
    case WINDOW_FILTER_OP_EXCLUDE:
        return _T("����ʾ");
    }

    return _T("");
}