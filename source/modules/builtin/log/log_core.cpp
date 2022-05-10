#include "stdafx.h"
#include "modules/builtin/log/log_core.h"
#include "modules/builtin/log/log_record.h"

LogCore::LogCore()
#ifdef DEBUG
    : m_nLogLevel(LOG_LEVEL_DEBUG)
#else
    : m_nLogLevel(LOG_LEVEL_INFO)
#endif
{

}

LogCore::~LogCore()
{

}

HRESULT
LogCore::RegisterConsumer(ILogConsumer *pConsumer)
{
    if(NULL == pConsumer) {
        return E_INVALIDARG;
    }

    ConsumerList::iterator oConsumerIt = GetConsumer(pConsumer);
    if(oConsumerIt != m_vConsumers.end()) {
        return S_FALSE;
    }

    m_vConsumers.push_back(pConsumer);

    return S_OK;
}

HRESULT
LogCore::UnregisterConsumer(ILogConsumer *pConsumer)
{
    if(NULL == pConsumer) {
        return E_INVALIDARG;
    }

    ConsumerList::iterator oConsumerIt = GetConsumer(pConsumer);
    if(oConsumerIt == m_vConsumers.end()) {
        return S_FALSE;
    }

    m_vConsumers.erase(oConsumerIt);

    return S_OK;
}

HRESULT
LogCore::SetLogLevel(DWORD nLogLevel)
{
    m_nLogLevel = nLogLevel;
    return S_OK;
}

HRESULT
LogCore::GetLogLevel(DWORD *pLogLevel)
{
    if(NULL == pLogLevel) {
        return E_POINTER;
    }

    *pLogLevel = m_nLogLevel;

    return S_OK;
}

HRESULT
LogCore::CreateLogRecord(DWORD nType, ILogRecord **pLogRecord)
{
    if(NULL == pLogRecord) {
        return E_INVALIDARG;
    }

    return LogRecord::CreateObject(__uuidof(ILogRecord), (void **)pLogRecord);
}

HRESULT
LogCore::Log(ILogRecord *pLogRecord)
{
    if(NULL == pLogRecord) {
        return E_INVALIDARG;
    }

    ConsumerList::iterator oConsumerIt = m_vConsumers.begin();
    while(oConsumerIt != m_vConsumers.end()) {
        (*oConsumerIt)->OnLog(pLogRecord);
        ++oConsumerIt;
    }

    return S_OK;
}

LogCore::ConsumerList::iterator
LogCore::GetConsumer(ILogConsumer *pConsumer)
{
    ConsumerList::iterator oIt = m_vConsumers.begin();
    while(oIt != m_vConsumers.end()) {
        if((*oIt).p == pConsumer) {
            break;
        }
        ++oIt;
    }

    return oIt;
}