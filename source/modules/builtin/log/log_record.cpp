#include "stdafx.h"
#include "modules/builtin/log/log_record.h"

LogRecord::LogRecord()
: m_nLevel(0)
, m_nLine(0)
, m_pParam(NULL)
{

}

LogRecord::~LogRecord()
{

}

HRESULT
LogRecord::GetType(DWORD *pType)
{
    ASSERT_RETURN(NULL != pType, E_INVALIDARG);
    *pType = LOG_RECORD_TYPE_BASIC;
    return S_OK;
}

HRESULT
LogRecord::GetLevel(DWORD *pLevel)
{
    ASSERT_RETURN(NULL != pLevel, E_INVALIDARG);
    *pLevel = m_nLevel;
    return S_OK;
}

HRESULT
LogRecord::SetLevel(DWORD nLevel)
{
    m_nLevel = nLevel;
    return S_OK;
}

HRESULT
LogRecord::GetModule(BSTR *pModule)
{
    ASSERT_RETURN(NULL != pModule, E_INVALIDARG);
    *pModule = m_strModule.Copy();
    return S_OK;
}

HRESULT
LogRecord::SetModule(BSTR strModule)
{
    m_strModule = strModule;
    return S_OK;
}

HRESULT
LogRecord::GetFile(BSTR *pFile)
{
    ASSERT_RETURN(NULL != pFile, E_INVALIDARG);
    *pFile = m_strFile.Copy();
    return S_OK;
}

HRESULT
LogRecord::SetFile(BSTR strFile)
{
    m_strFile = strFile;
    return S_OK;
}

HRESULT
LogRecord::GetFunction(BSTR *pFunction)
{
    ASSERT_RETURN(NULL != pFunction, E_INVALIDARG);
    *pFunction = m_strFunction.Copy();
    return S_OK;
}

HRESULT
LogRecord::SetFunction(BSTR strFunction)
{
    m_strFunction = strFunction;
    return S_OK;
}

HRESULT
LogRecord::GetLine(DWORD *pLine)
{
    ASSERT_RETURN(NULL != pLine, E_INVALIDARG);
    *pLine = m_nLine;
    return S_OK;
}

HRESULT
LogRecord::SetLine(DWORD nLine)
{
    m_nLine = nLine;
    return S_OK;
}

HRESULT
LogRecord::GetContent(BSTR *pContent)
{
    ASSERT_RETURN(NULL != pContent, E_INVALIDARG);
    *pContent = m_strContent.Copy();
    return S_OK;
}

HRESULT
LogRecord::SetContent(BSTR strContent)
{
    m_strContent = strContent;
    return S_OK;
}

HRESULT
LogRecord::GetParam(PVOID *ppParam)
{
    ASSERT_RETURN(NULL != ppParam, E_INVALIDARG);
    *ppParam = m_pParam;
    return S_OK;
}

HRESULT
LogRecord::SetParam(PVOID pParam)
{
    m_pParam = pParam;
    return S_OK;
}