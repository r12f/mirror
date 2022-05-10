#pragma once

class CTabPanel :
    public CTabViewImpl<CTabPanel>
{
public:
    CTabPanel();
    virtual ~CTabPanel();

    void UpdateLayout();
};
