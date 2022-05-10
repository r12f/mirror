#include "stdafx.h"
#include "base/threading/lock.h"

LockCS::LockCS()
{
    ::InitializeCriticalSectionAndSpinCount(&m_oCS, 1000);
}

LockCS::~LockCS()
{
    ::DeleteCriticalSection(&m_oCS);
}

void
LockCS::Acquire()
{
    ::EnterCriticalSection(&m_oCS);
}

BOOL
LockCS::TryAcquire()
{
    return ::TryEnterCriticalSection(&m_oCS);
}

void
LockCS::Release()
{
    ::LeaveCriticalSection(&m_oCS);
}
