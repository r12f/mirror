#include "stdafx.h"
#include "base/log/logger.h"

HRESULT
WriteLogRecord(DWORD nType, DWORD nLevel, TCHAR *pModule, TCHAR *pFile, TCHAR *pFunction, UINT nCodeLine, TCHAR *pFormat, ...)
{
    static CComPtr<ILogCore> pLogCore = GetComponentInterface<ILogCore>(COMPONENT_LOG);
    if(NULL == pLogCore) {
        return E_FAIL;
    }

    DWORD nCurrentLogLevel = LOG_LEVEL_DEBUG;
    pLogCore->GetLogLevel(&nCurrentLogLevel);
    if(nCurrentLogLevel > nLevel) {
        return S_FALSE;
    }

    CComPtr<ILogRecord> pLogRecord;
    if(FAILED(pLogCore->CreateLogRecord(nType, &pLogRecord)) && NULL == pLogRecord) {
        return E_FAIL;
    }

    va_list args;
    va_start(args, pFormat);
    CString strLogContent;
    strLogContent.FormatV(pFormat, args);
    va_end(args);

    pLogRecord->SetLevel(nLevel);
    pLogRecord->SetModule(CComBSTR(pModule));
    pLogRecord->SetFile(CComBSTR(pFile));
    pLogRecord->SetFunction(CComBSTR(pFunction));
    pLogRecord->SetLine(nCodeLine);
    pLogRecord->SetContent(CComBSTR(strLogContent));

    pLogCore->Log(pLogRecord);

    return S_OK;
}
