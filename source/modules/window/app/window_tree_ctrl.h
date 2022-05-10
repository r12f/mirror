#pragma once

#include "ui/ui_engine/controls/tree_ctrl_ex.h"

class CWindowTreeCtrl :
    public CTreeCtrlEx
{
public:
    CWindowTreeCtrl();
    virtual ~CWindowTreeCtrl();

protected:
    virtual VOID BeforeContextMenuPopup(CMenuHandle &oMenu);
    virtual VOID AfterContextMenuPopup(CMenuHandle &oMenu);
};