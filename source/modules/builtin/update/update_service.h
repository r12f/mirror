#pragma once

#include "base/com/com_class.h"
#include "modules/builtin/component_update_service.h"

class UpdateService :
    public IUpdateService
{
public:
    UpdateService();
    virtual ~UpdateService();

    COM_CLASS_FACTORY(UpdateService)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(UpdateService)
            QI_MAPPING(IUpdateService)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    // Override IUpdateService
    STDMETHODIMP CheckUpdate(BOOL bAutoCheck);

protected:
    BOOL NeedCheckUpdate();
};