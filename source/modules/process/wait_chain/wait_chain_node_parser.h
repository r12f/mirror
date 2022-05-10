#pragma once

#include "modules/process/wait_chain/wait_chain_api.h"

class WaitChainNodeParser
{
public:
    WaitChainNodeParser(WAITCHAIN_NODE_INFO *pNode) { Parse(pNode); }
    ~WaitChainNodeParser() {}

    const CString & Info() const { return m_strInfo; }

protected:
    VOID Parse(WAITCHAIN_NODE_INFO *pNode);

private:
    CString m_strInfo;
};