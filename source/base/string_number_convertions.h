#pragma once

int HexToInt(const CString &strHex);
__int64 HexToInt64(const CString &strHex);

int findIntInAsciiString(LPCSTR pString, DWORD nMaxSize = 0);
int findIntInUnicodeString(LPCWSTR pString, DWORD nMaxSize = 0);

#ifdef _UNICODE
#define findIntInString findIntInUnicodeString
#else
#define findIntInString findIntInAsciiString
#endif

BOOL SplitAsciiStringToNumber(LPCSTR pNumberString, CHAR nSplit, int *pNumberList, int nNumberCount);
BOOL SplitUnicodeStringToNumber(LPCWSTR pNumberString, WCHAR nSplit, int *pNumberList, int nNumberCount);
BOOL VersionStringToNumber(const CString &strVersion, UINT *pVerMajor, UINT *pVerMinor, UINT *pVerBuild, UINT *pVerSpecial);

CString FormatSizeToString(ULONGLONG nSize);