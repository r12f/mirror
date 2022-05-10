#pragma once

class LockCS
{
public:
    LockCS();
    ~LockCS();

    void Acquire();
    BOOL TryAcquire();
    void Release();

private:
    CRITICAL_SECTION m_oCS;
};

template <class T>
class AutoLockT
{
public:
    AutoLockT(T &oLock)
        : m_pLock(&oLock)
    {
        if(NULL != m_pLock) {
            m_pLock->Acquire();
        }
    }

    ~AutoLockT()
    {
        if(NULL != m_pLock) {
            m_pLock->Release();
        }
    }

private:
    T *m_pLock;
};

typedef AutoLockT<LockCS> AutoCS;