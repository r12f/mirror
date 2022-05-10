#include "stdafx.h"
#include "comm_ctrl_utils.h"

CDItem::CDItem()
{
    memset(this, 0, sizeof(HDITEM));
}

CDItem &
CDItem::Width(int nWidth)
{
    mask |= HDI_WIDTH;
    cxy = nWidth;
    return *this;
}

CDItem &
CDItem::Title(LPTSTR strTitle)
{
    mask |= HDI_TEXT;
    pszText = strTitle;
    return *this;
}

CDItem &
CDItem::SetAlign(int nAlign)
{
    mask |= HDI_FORMAT;
    fmt &= 0xFFFFFFF0;
    fmt |= nAlign & 0x000000F;

    return *this;
}

BOOL
CHeaderCtrlExtensions::DeleteAllItems(CHeaderCtrl &oHeaderCtrl)
{
    int nItemCount = oHeaderCtrl.GetItemCount();
    for (int nItemIndex = nItemCount - 1; nItemIndex >= 0; --nItemIndex) {
        oHeaderCtrl.DeleteItem(nItemIndex);
    }

    return TRUE;
}

BOOL
CHeaderCtrlExtensions::SetHeaders(CHeaderCtrl &oHeaderCtrl, HDITEM *pHeaders, int nHeaderCount)
{
    if (NULL == pHeaders || 0 == nHeaderCount) {
        return FALSE;
    }

    CHeaderCtrlExtensions::DeleteAllItems(oHeaderCtrl);
    for (int nHeaderIndex = 0; nHeaderIndex < nHeaderCount; ++nHeaderIndex) {
        oHeaderCtrl.AddItem(&pHeaders[nHeaderIndex]);
    }

    return TRUE;
}