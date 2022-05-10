#include "stdafx.h"
#include "base/string_number_convertions.h"

int HexToInt(const CString &strHex)
{
    CString strFixedHex = strHex;
    if(strFixedHex.Left(2).MakeLower() == _T("0x")) {
        strFixedHex = strFixedHex.Mid(2);
    }
    return (int)(LONG_PTR)_tcstoul(strFixedHex.GetBuffer(), NULL, 16);
}

__int64 HexToInt64(const CString &strHex)
{
    // Not implement
    return 0;
}

template <class CharTrait>
static int findIntInStringT(typename CharTrait::ConstString pString, DWORD nMaxSize)
{
    if(NULL == pString) {
        return 0;
    }

    bool bNeg = false, bFound = false;
    DWORD i = 0;
    int n = 0;
    for(; (0 == nMaxSize || i < nMaxSize) && pString[i] != 0; ++i) {
        if(pString[i] == CharTrait::NegSign()) {
            bNeg = true;
            continue;
        }
        
        if(CharTrait::FromAscii('0') <= pString[i] && pString[i] <= CharTrait::FromAscii('9')) {
            n = n * 10 + (pString[i] - CharTrait::FromAscii('0'));
            bFound = true;
        } else {
            if(bFound) {
                break;
            }
            if(pString[i] != CharTrait::NegSign()) {
                bNeg = false;
            }
        }
    }

    return (bNeg ? -n : n);
}

struct CharTraitAscii {
    typedef char CharType;
    typedef char * String;
    typedef const char * ConstString;

    static char NegSign() { return '-'; }
    static char FromAscii(char n) { return n; }
};

struct CharTraitUnicode {
    typedef WCHAR CharType;
    typedef WCHAR * String;
    typedef const WCHAR * ConstString;

    static WCHAR NegSign() { return L'-'; }
    static WCHAR FromAscii(char n) { return (WCHAR)(n); }
};

int findIntInAsciiString(LPCSTR pString, DWORD nMaxSize)
{
    return findIntInStringT<CharTraitAscii>(pString, nMaxSize);
}

int findIntInUnicodeString(LPCWSTR pString, DWORD nMaxSize)
{
    return findIntInStringT<CharTraitUnicode>(pString, nMaxSize);
}

template <class CharTrait>
static BOOL SplitStringToNumberT(typename CharTrait::ConstString pNumberString, typename CharTrait::CharType nSplit, int *pNumberList, int nNumberCount)
{
    CStringT<CharTrait::CharType, StrTraitATL< CharTrait::CharType, ChTraitsCRT< CharTrait::CharType > > > strNumber = pNumberString, strPart;
    int i = 0, j = 0, nLastDotPos = 0, nDotPos = 0;
    for(i = 0; i < nNumberCount; ++i) {
        nDotPos = strNumber.Find(nSplit, nLastDotPos);
        if(nDotPos < 0) {
            nDotPos = strNumber.GetLength();
        }

        strPart = strNumber.Mid(nLastDotPos, nDotPos - nLastDotPos);

        int nPartSize = strPart.GetLength();
        for(j = 0; j < nPartSize; ++j) {
            if((CharTrait::FromAscii('0') > strPart[j] || CharTrait::FromAscii('9') < strPart[j])
                && nSplit != strPart[j] && CharTrait::FromAscii('-') != strPart[j]
                && CharTrait::FromAscii(' ') != strPart[j] && CharTrait::FromAscii('\t') != strPart[j]) {
                return FALSE;
            }
        }

        pNumberList[i] = findIntInStringT<CharTrait>(strPart, 0);
        nLastDotPos = nDotPos + 1;
    }

    return TRUE;
}

BOOL SplitAsciiStringToNumber(LPCSTR pNumberString, CHAR nSplit, int *pNumberList, int nNumberCount)
{
    return SplitStringToNumberT<CharTraitAscii>(pNumberString, nSplit, pNumberList, nNumberCount);
}

BOOL SplitUnicodeStringToNumber(LPCWSTR pNumberString, WCHAR nSplit, int *pNumberList, int nNumberCount)
{
    return SplitStringToNumberT<CharTraitUnicode>(pNumberString, nSplit, pNumberList, nNumberCount);
}

BOOL VersionStringToNumber(const CString &strVersion, UINT *pVerMajor, UINT *pVerMinor, UINT *pVerBuild, UINT *pVerSpecial)
{
    if(NULL != pVerMajor) { *pVerMajor = 0; }
    if(NULL != pVerMinor) { *pVerMinor = 0; }
    if(NULL != pVerBuild) { *pVerBuild = 0; }
    if(NULL != pVerSpecial) { *pVerSpecial = 0; }

    UINT nVersion[4] = {0};
    int i = 0, j = 0, nLastDotPos = 0, nDotPos = 0;
    CString strPart;
    for(i = 0; i < 4; ++i) {
        if(i < 3) {
            nDotPos = strVersion.Find(_T('.'), nLastDotPos);
            if(nDotPos < 0) {
                return FALSE;
            }
        } else {
            nDotPos = strVersion.GetLength();
        }

        strPart = strVersion.Mid(nLastDotPos, nDotPos - nLastDotPos);
        
        int nVersionSize = strPart.GetLength();
        for(j = 0; j < nVersionSize; ++j) {
            if((_T('0') > strPart[j] || _T('9') < strPart[j]) && _T('.') != strPart[j]) {
                return FALSE;
            }
        }

        nVersion[i] = findIntInString(strPart);
        nLastDotPos = nDotPos + 1;
    }

    if(NULL != pVerMajor) { *pVerMajor = nVersion[0]; }
    if(NULL != pVerMinor) { *pVerMinor = nVersion[1]; }
    if(NULL != pVerBuild) { *pVerBuild = nVersion[2]; }
    if(NULL != pVerSpecial) { *pVerSpecial = nVersion[3]; }

    return TRUE;
}

CString FormatSizeToString(ULONGLONG nSize)
{
    static TCHAR *s_pSizeUnit[] = { _T(" B"), _T(" KiB"), _T(" MiB"), _T(" GiB"), _T(" TiB"), _T(" PiB") };
    static DWORD s_nSizeUnitCount = sizeof(s_pSizeUnit) / sizeof(TCHAR *);

    double nFinalSize = (double)nSize;
    DWORD nSizeUnit = 0;
    while(nFinalSize > 1024 && nSizeUnit < s_nSizeUnitCount - 1) {
        nFinalSize /= 1024;
        ++nSizeUnit;
    }

    CString strSize;
    strSize.Format(_T("%.2lf%s"), nFinalSize, s_pSizeUnit[nSizeUnit]);

    return strSize;
}