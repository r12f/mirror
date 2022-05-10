#pragma once

#include "modules/builtin/component_log.h"
#include "base/com/com_class.h"

class LogRecord :
    public ILogRecord
{
public:
    LogRecord();
    virtual ~LogRecord();

    COM_CLASS_FACTORY(LogRecord)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(LogRecord)
            QI_MAPPING(ILogRecord)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(GetType)(DWORD *pType);
    STDMETHOD(GetLevel)(DWORD *pLevel);
    STDMETHOD(SetLevel)(DWORD nLevel);
    STDMETHOD(GetModule)(BSTR *pModule);
    STDMETHOD(SetModule)(BSTR strModule);
    STDMETHOD(GetFile)(BSTR *pFile);
    STDMETHOD(SetFile)(BSTR strFile);
    STDMETHOD(GetFunction)(BSTR *pFunction);
    STDMETHOD(SetFunction)(BSTR strFunction);
    STDMETHOD(GetLine)(DWORD *pLine);
    STDMETHOD(SetLine)(DWORD nLine);
    STDMETHOD(GetContent)(BSTR *pContent);
    STDMETHOD(SetContent)(BSTR strContent);
    STDMETHOD(GetParam)(PVOID *ppParam);
    STDMETHOD(SetParam)(PVOID pParam);

private:
    DWORD m_nLevel;
    CComBSTR m_strModule;
    CComBSTR m_strFile;
    CComBSTR m_strFunction;
    DWORD m_nLine;
    CComBSTR m_strContent;
    PVOID m_pParam;
};