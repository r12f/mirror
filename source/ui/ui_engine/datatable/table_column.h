#pragma once

#include "ui/ui_engine/datatable/table_cell.h"

class CTableColumn
{
public:
    enum Align {
        ALIGN_LEFT      = 0,
        ALIGN_CENTER,
        ALIGN_RIGHT,
    };

    CTableColumn();
    virtual ~CTableColumn() {}

    CTableColumn & SetWidth(int nWidth);
    CTableColumn & SetTitle(LPCTSTR pTitle);
    CTableColumn & SetAlign(Align nAlign);

    virtual CTableCell * ProcessData(void *pData) = 0;

    int m_nWidth;
    CString m_strTitle;
    Align m_nAlign;
    void *m_pfnProjector;
};

template <class T>
class CTableColumnT :
    public CTableColumn
{
public:
    typedef typename CTableDataTraitT<T>::ValueType (*FuncProjector)(void *);
    CTableColumnT & SetProjector(FuncProjector pfnProjector) { m_pfnProjector = pfnProjector; return *this; }

    virtual CTableCell * ProcessData(void *pData)
    {
        ASSERT_RETURN(NULL != pData, NULL);
        ASSERT_RETURN(NULL != m_pfnProjector, NULL);
        CTableDataTraitT<T>::ValueType v = ((FuncProjector)m_pfnProjector)(pData);
        return new CTableCellT<T>(this, v);
    }
};

// Integers 
#define DEFINE_TABLE_COLUMN_INT(type)   \
    template <> \
    class CTableColumnT<type> : \
        public CTableColumn \
    {   \
    public: \
        CTableColumnT() : m_bIsShowAsHex(false) {}  \
    \
        bool IsShowAsHex() const { return m_bIsShowAsHex; }   \
        CTableColumnT<type> & SetIsShowAsHex(bool bIsShowAsHex) { m_bIsShowAsHex = bIsShowAsHex; return *this; }   \
    \
        typedef CTableDataTraitT<type>::ValueType (*FuncProjector)(void *);    \
        CTableColumnT<type> & SetProjector(FuncProjector pfnProjector) { m_pfnProjector = pfnProjector; return *this; }   \
    \
        virtual CTableCell * ProcessData(void *pData)   \
        {   \
            ASSERT_RETURN(NULL != pData, NULL); \
            ASSERT_RETURN(NULL != m_pfnProjector, NULL);    \
            CTableDataTraitT<type>::ValueType v = ((FuncProjector)m_pfnProjector)(pData);  \
            return new CTableCellT<type>(this, v);  \
        }   \
    \
    private:    \
        bool m_bIsShowAsHex;  \
    }

DEFINE_TABLE_COLUMN_INT(INT8);
DEFINE_TABLE_COLUMN_INT(UINT8);
DEFINE_TABLE_COLUMN_INT(INT16);
DEFINE_TABLE_COLUMN_INT(UINT16);
DEFINE_TABLE_COLUMN_INT(INT32);
DEFINE_TABLE_COLUMN_INT(UINT32);
DEFINE_TABLE_COLUMN_INT(INT64);
DEFINE_TABLE_COLUMN_INT(UINT64);
DEFINE_TABLE_COLUMN_INT(long);
DEFINE_TABLE_COLUMN_INT(unsigned long);

// Floats
#define DEFINE_TABLE_COLUMN_FLOAT(type)   \
    template <> \
    class CTableColumnT<type> : \
        public CTableColumn \
    {   \
    public: \
        CTableColumnT() : m_nPrecision(0) {}  \
    \
        int GetPrecision() const { return m_nPrecision; }   \
        CTableColumnT<type> & SetPrecision(int nPrecision) { m_nPrecision = nPrecision; return *this; }   \
    \
        typedef CTableDataTraitT<type>::ValueType (*FuncProjector)(void *);    \
        CTableColumnT<type> & SetProjector(FuncProjector pfnProjector) { m_pfnProjector = pfnProjector; return *this; }   \
    \
        virtual CTableCell * ProcessData(void *pData)   \
        {   \
            ASSERT_RETURN(NULL != pData, NULL); \
            ASSERT_RETURN(NULL != m_pfnProjector, NULL);    \
            CTableDataTraitT<type>::ValueType v = ((FuncProjector)m_pfnProjector)(pData);  \
            return new CTableCellT<type>(this, v);  \
        }   \
    \
    private:    \
        int m_nPrecision;  \
    }

DEFINE_TABLE_COLUMN_FLOAT(float);
DEFINE_TABLE_COLUMN_FLOAT(double);