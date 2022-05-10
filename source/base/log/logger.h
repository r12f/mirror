#pragma once

#include "modules/builtin/component_log.h"

HRESULT WriteLogRecord(DWORD nType, DWORD nLevel, TCHAR *pModule, TCHAR *pFile, TCHAR *pFunction, UINT nCodeLine, TCHAR *pFormat, ...);