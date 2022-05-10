#pragma once

template <class T, class HandleTrait>
class ScopedHandleBase
{
public:
    typedef T HandleType;

public:
    ScopedHandleBase()
        : m_hHandle(NULL)
    {

    }

    ScopedHandleBase(HandleType h)
        : m_hHandle(h)
    {

    }

    ~ScopedHandleBase()
    {
        Reset();
    }

    operator HandleType()
    {
        return m_hHandle;
    }

    HandleType Get()
    {
        return m_hHandle;
    }

    VOID Attach(HandleType h)
    {
        Reset();
        m_hHandle = h;
    }

    HandleType Detach()
    {
        HandleType h = m_hHandle;
        m_hHandle = HandleTrait::NullHandle();
        return h;
    }

    VOID Reset()
    {
        if(HandleTrait::NullHandle() != m_hHandle) {
            HandleTrait::Destroy(m_hHandle);
        }

        m_hHandle = HandleTrait::NullHandle();
    }

    HandleType * operator &()
    {
        return &m_hHandle;
    }

private:
    HandleType m_hHandle;

private:
    HandleType operator = (HandleType h);
    ScopedHandleBase(const ScopedHandleBase<HandleType, HandleTrait> &rhs);
};

struct ScopedHandleTraitHandle {
    static BOOL Destroy(HANDLE h) { return ::CloseHandle(h); }
    static HANDLE NullHandle() { return NULL; }
};

typedef ScopedHandleBase<HANDLE, ScopedHandleTraitHandle> ScopedHandle;

struct ScopedHandleTraitHModule {
    static BOOL Destroy(HMODULE h) { return ::FreeLibrary(h); }
    static HMODULE NullHandle() { return NULL; }
};

typedef ScopedHandleBase<HMODULE, ScopedHandleTraitHModule> ScopedModule;