#include "stdafx.h"
#include "table_column.h"

CTableColumn::CTableColumn()
    : m_nWidth(0)
    , m_nAlign(ALIGN_LEFT)
    , m_pfnProjector(NULL)
{}

CTableColumn &
CTableColumn::SetWidth(int nWidth)
{
    m_nWidth = nWidth;
    return *this;
}

CTableColumn &
CTableColumn::SetTitle(LPCTSTR pTitle)
{
    m_strTitle = pTitle;
    return *this;
}

CTableColumn &
CTableColumn::SetAlign(Align nAlign)
{
    m_nAlign = nAlign;
    return *this;
}