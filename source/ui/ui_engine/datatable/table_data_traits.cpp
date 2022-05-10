#include "stdafx.h"
#include "table_data_traits.h"
#include "table_column.h"

void CTableDataTraitT<bool>::ToString(CTableColumnT<bool> *pColumn, CString &s, const bool &v)
{
    s = v ? _T("True") : _T("False");
}

// Integers
#define DEFINE_TABLE_DATA_TRAIT_IMPL_INT(type, formatDec, formatHex)     \
    void CTableDataTraitT<type>::ToString(CTableColumnT<type> *pColumn, CString &s, const type &v)  \
    {                                                           \
        if (pColumn->IsShowAsHex()) { s.Format(formatHex, v); } \
        else { s.Format(formatDec, v); }                        \
    }

DEFINE_TABLE_DATA_TRAIT_IMPL_INT(INT8, _T("%I8d"), _T("0x%02x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(UINT8, _T("%I8u"), _T("0x%02x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(INT16, _T("%I16d"), _T("0x%04x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(UINT16, _T("%I16u"), _T("0x%04x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(INT32, _T("%I32d"), _T("0x%08x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(UINT32, _T("%I32u"), _T("0x%08x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(INT64, _T("%I64d"), _T("0x%I64x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(UINT64, _T("%I64u"), _T("0x%I64x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(long, _T("%ld"), _T("0x%08x"));
DEFINE_TABLE_DATA_TRAIT_IMPL_INT(unsigned long, _T("%lu"), _T("0x%08x"));

// Floats
#define DEFINE_TABLE_DATA_TRAIT_IMPL_FLOAT(type)     \
    void CTableDataTraitT<type>::ToString(CTableColumnT<type> *pColumn, CString &s, const type &v)  \
    {                                                           \
        int nPrecision = pColumn->GetPrecision();   \
        \
        CString strFormat;  \
        strFormat.Format(_T("%%.%df"), nPrecision); \
        \
        s.Format(strFormat, v); \
    }

DEFINE_TABLE_DATA_TRAIT_IMPL_FLOAT(float);
DEFINE_TABLE_DATA_TRAIT_IMPL_FLOAT(double);

