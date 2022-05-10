#pragma once

class ComRefPersistent
{
public:
    ComRefPersistent() {}
    ~ComRefPersistent() {}

    ULONG AddRef(void) { return 1; }
    ULONG Release(void) { return 1; }
};

class ComRef
{
public:
    ComRef() : m_nRefCount(1) {}
    ~ComRef() {}

    ULONG AddRef(void) { return ++m_nRefCount; }
    ULONG Release(void) { return --m_nRefCount; }

private:
    int m_nRefCount;
};

// Not implement
class ComThreadSafeRef
{
public:
    ComThreadSafeRef() : m_nRefCount(1) {}
    ~ComThreadSafeRef() {}

    ULONG AddRef(void) { return (ULONG)::InterlockedIncrement(&m_nRefCount); }
    ULONG Release(void) { return (ULONG)::InterlockedDecrement(&m_nRefCount); }

private:
    DWORD m_nRefCount;
};
