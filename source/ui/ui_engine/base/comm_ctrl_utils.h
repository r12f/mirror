#pragma once

class CDItem : 
    public HDITEM
{
public:
    CDItem();

    CDItem & Width(int nWidth);
    CDItem & Title(LPTSTR strTitle);
    CDItem & SetAlign(int nAlign);
};

class CHeaderCtrlExtensions
{
public:
    static BOOL DeleteAllItems(CHeaderCtrl &oHeaderCtrl);
    static BOOL SetHeaders(CHeaderCtrl &oHeaderCtrl, HDITEM *pHeaders, int nHeaderCount);
};