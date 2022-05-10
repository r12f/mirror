#pragma once

#include "ui/ui_engine/datatable/table_data_traits.h"

class CTableCell
{
public:
    virtual ~CTableCell() {}
    virtual bool Compare(const CTableCell &rhs) const = 0;
    virtual LPCTSTR ToString() const = 0;
};

template <class T>
class CTableCellT : 
    public CTableCell
{
public:
    CTableCellT(CTableColumnT<T> *pColumn, const typename CTableDataTraitT<T>::ValueType &v)
        : m_pColumn(pColumn)
    {
        ATLASSERT(NULL != pColumn);
        SetValue(v);
    }

    const typename CTableDataTraitT<T>::ValueType & GetValue() const
    {
        return m_nValue;
    }

    void SetValue(const typename CTableDataTraitT<T>::ValueType &v)
    {
        m_nValue = v;
        CTableDataTraitT<T>::ToString(m_pColumn, m_strValue, v);
    }

    virtual bool Compare(const CTableCell &rhs) const
    {
        const CTableCellT *rhsT = (const CTableCellT *)&rhs;
        return m_nValue < rhsT->m_nValue;
    }

    virtual LPCTSTR ToString() const
    {
        return m_strValue;
    }

private:
    CTableColumnT<T> *m_pColumn;
    typename CTableDataTraitT<T>::ValueType m_nValue;
    CString m_strValue;
};

template <>
class CTableCellT<LPCTSTR> : 
    public CTableCell
{
public:
    CTableCellT(CTableColumnT<LPCTSTR> *pColumn, LPCTSTR &v)
        : m_pColumn(pColumn)
    {
        ATLASSERT(NULL != pColumn);
        SetValue(v);
    }

    LPCTSTR GetValue() const
    {
        return m_strValue;
    }

    void SetValue(LPCTSTR v)
    {
        ASSERT_RETURN_VOID(NULL != v);
        m_strValue = v;
    }

    virtual bool Compare(const CTableCell &rhs) const
    {
        const CTableCellT<LPCTSTR> *rhsImpl = (const CTableCellT<LPCTSTR> *)&rhs;
        return m_strValue < rhsImpl->m_strValue;
    }

    virtual LPCTSTR ToString() const
    {
        return m_strValue;
    }

private:
    CTableColumnT<LPCTSTR> *m_pColumn;
    CString m_strValue;
};
