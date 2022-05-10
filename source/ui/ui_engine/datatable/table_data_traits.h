#pragma once

template <class T>
class CTableColumnT;

template <class T>
class CTableDataTraitT
{
public:
    typedef T ValueType;

    static void ToString(CTableColumnT<T> *pColumn, CString &s, const T &v) { return; }
};

#define DEFINE_TABLE_DATA_TRAIT_SIMPLE(type)    \
    template <> \
    class CTableDataTraitT<type>    \
    {   \
    public: \
        typedef type ValueType; \
    \
        static void ToString(CTableColumnT<type> *pColumn, CString &s, const type &v);   \
    }

DEFINE_TABLE_DATA_TRAIT_SIMPLE(bool);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(INT8);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(UINT8);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(INT16);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(UINT16);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(INT32);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(UINT32);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(INT64);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(UINT64);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(long);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(unsigned long);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(float);
DEFINE_TABLE_DATA_TRAIT_SIMPLE(double);