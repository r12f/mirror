#pragma once

#include "dbghelp.h"
#include "Tlhelp32.h"

template <class TBase>
struct WalkerTrait
{
    typedef void EntryType;
    static DWORD GetSnapType() { return 0; }
    static BOOL WalkFirst(HANDLE hSnapshot, EntryType *pEntry) { return FALSE; }
    static BOOL WalkNext(HANDLE hSnapshot, EntryType *pEntry) { return FALSE; }
};

template <class T>
class SystemSnapshotWalker
{
public:
    typedef typename WalkerTrait<T>::EntryType WalkerEntry;

    SystemSnapshotWalker()
        : m_hSnapshot(NULL)
    {

    }

    virtual ~SystemSnapshotWalker()
    {
        if(NULL != m_hSnapshot) {
            ::CloseHandle(m_hSnapshot);
        }
    }

    WalkerEntry * Get()
    {
        return &m_oEntry;
    }

    BOOL Start(DWORD nPid = 0)
    {
        DWORD nSnapType = WalkerTrait<T>::GetSnapType();
        if(0 == nSnapType) {
            return FALSE;
        }

        m_hSnapshot = ::CreateToolhelp32Snapshot(nSnapType, nPid);
        if(INVALID_HANDLE_VALUE == m_hSnapshot) {
            return FALSE;
        }

        memset(&m_oEntry, 0, sizeof(WalkerEntry));
        m_oEntry.dwSize = sizeof(WalkerEntry);
        return WalkerTrait<T>::WalkFirst(m_hSnapshot, &m_oEntry);
    }

    BOOL WalkNext()
    {
        return WalkerTrait<T>::WalkNext(m_hSnapshot, &m_oEntry);
    }

private:
    HANDLE m_hSnapshot;
    WalkerEntry m_oEntry;
};


class Process32Walker;

template <>
struct WalkerTrait<Process32Walker>
{
    typedef PROCESSENTRY32 EntryType;
    static DWORD GetSnapType() { return TH32CS_SNAPPROCESS; }
    static BOOL WalkFirst(HANDLE hSnapshot, PROCESSENTRY32 *pEntry) { return Process32First(hSnapshot, pEntry); }
    static BOOL WalkNext(HANDLE hSnapshot, PROCESSENTRY32 *pEntry) { return Process32Next(hSnapshot, pEntry); }
};

class Process32Walker : public SystemSnapshotWalker<Process32Walker> {};

class Module32Walker;

template <>
struct WalkerTrait<Module32Walker>
{
    typedef MODULEENTRY32 EntryType;
    static DWORD GetSnapType() { return TH32CS_SNAPMODULE; }
    static BOOL WalkFirst(HANDLE hSnapshot, MODULEENTRY32 *pEntry) { return Module32First(hSnapshot, pEntry); }
    static BOOL WalkNext(HANDLE hSnapshot, MODULEENTRY32 *pEntry) { return Module32Next(hSnapshot, pEntry); }
};

class Module32Walker : public SystemSnapshotWalker<Module32Walker> {};

class Thread32Walker;

template <>
struct WalkerTrait<Thread32Walker>
{
    typedef THREADENTRY32 EntryType;
    static DWORD GetSnapType() { return TH32CS_SNAPTHREAD; }
    static BOOL WalkFirst(HANDLE hSnapshot, THREADENTRY32 *pEntry) { return Thread32First(hSnapshot, pEntry); }
    static BOOL WalkNext(HANDLE hSnapshot, THREADENTRY32 *pEntry) { return Thread32Next(hSnapshot, pEntry); }
};

class Thread32Walker : public SystemSnapshotWalker<Thread32Walker> {};

BOOL GetProcNameByEnum(DWORD nPid, CString &strProcName);