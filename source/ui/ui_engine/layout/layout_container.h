#pragma once

#include <vector>
#include "base/com/com_class.h"
#include "ui/layout.h"
#include "ui/ui_engine/layout/layout_box_base.h"

struct BoxSlotInfo {
    CComPtr<ILayoutBoxInternal> m_pBox;
    BOOL m_bUseSlotSize;
    INT32 m_nSlotSize;
    DWORD m_nSlotWeight;
    INT32 m_nCalcSlotSize;

    BoxSlotInfo();
    BoxSlotInfo(INT32 nSlotSize);
};

typedef std::vector<BoxSlotInfo> LayoutBoxList;

class LayoutContainer :
    public LayoutBoxBase
{
public:
    LayoutContainer();
    virtual ~LayoutContainer();

protected:
    HRESULT SetSlotNum(DWORD nSlotNum);
    HRESULT SetSlotSize(DWORD nSlotId, INT32 nSlotSize);
    HRESULT SetSlotWeight(DWORD nSlotId, DWORD nSlotWeight);
    HRESULT SetSlotBox(DWORD nSlotId, ILayoutBox *pBox);

    HRESULT GetSlotSize(DWORD *pSlotNum);
    HRESULT GetSlotSize(DWORD nSlotId, INT32 *pSlotSize);
    HRESULT GetSlotWeight(DWORD nSlotId, DWORD *pSlotWeight);
    HRESULT GetSlotBox(DWORD nSlotId, ILayoutBox **ppBox);

    HRESULT RecaculateSlotSize(INT32 nTotalSize);

protected:
    LayoutBoxList m_vBoxList;
};