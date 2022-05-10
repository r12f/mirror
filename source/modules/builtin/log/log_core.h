#pragma once

#include <vector>
#include "modules/builtin/component_log.h"
#include "base/com/com_class.h"

class LogCore :
    public ILogCore
{
    typedef std::vector<CComPtr<ILogConsumer>> ConsumerList;

public:
    LogCore();
    virtual ~LogCore();

    COM_CLASS_FACTORY(LogCore)
    STA_THREAD_MODAL()

    BEGIN_QUERY_INTERFACE()
        BEGIN_QI_MAP(LogCore)
            QI_MAPPING(ILogCore)
        END_QI_MAP()
    END_QUERY_INTERFACE()

    STDMETHOD(RegisterConsumer)(ILogConsumer *pConsumer);
    STDMETHOD(UnregisterConsumer)(ILogConsumer *pConsumer);
    STDMETHOD(SetLogLevel)(DWORD nLogLevel);
    STDMETHOD(GetLogLevel)(DWORD *pLogLevel);
    STDMETHOD(CreateLogRecord)(DWORD nType, ILogRecord **pLogRecord);
    STDMETHOD(Log)(ILogRecord *pLogRecord);

protected:
    ConsumerList::iterator GetConsumer(ILogConsumer *pConsumer);

private:
    ConsumerList m_vConsumers;
    DWORD m_nLogLevel;
};