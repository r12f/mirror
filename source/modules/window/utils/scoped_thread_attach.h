#pragma once

class ScopedThreadAttach
{
public:
    ScopedThreadAttach()
        : m_nTidFrom(0)
        , m_nTidTo(0)
    {

    }

    ScopedThreadAttach(DWORD nTidFrom, DWORD nTidTo)
        : m_nTidFrom(0)
        , m_nTidTo(0)
    {
        Attach(nTidFrom, nTidTo);
    }

    ~ScopedThreadAttach()
    {
        Detach();
    }

    VOID Attach(DWORD nTidFrom, DWORD nTidTo)
    {
        ATLASSERT(0 == m_nTidFrom);
        ATLASSERT(0 == m_nTidTo);

        if(nTidFrom == nTidTo) {
            return;
        }

        m_nTidFrom = nTidFrom;
        m_nTidTo = nTidTo;

        if(0 != m_nTidFrom && 0 != m_nTidTo) {
            ::AttachThreadInput(m_nTidFrom, m_nTidTo, TRUE);
        }
    }

    VOID Detach()
    {
        if(0 != m_nTidFrom && 0 != m_nTidTo) {
            ::AttachThreadInput(m_nTidFrom, m_nTidTo, FALSE);
        }

        m_nTidFrom = 0;
        m_nTidTo = 0;
    }

private:
    DWORD m_nTidFrom;
    DWORD m_nTidTo;
};