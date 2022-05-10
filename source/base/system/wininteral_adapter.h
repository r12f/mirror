/**
 * @brief 提供对Windows未公开的Api的包装
 */

#pragma once


// most of information comes from undocumented.ntinternals.net and windows symbols

typedef LONG NTSTATUS;

#define NT_SUCCESS(Status) (((NTSTATUS)(Status)) >= 0)

typedef struct _MEMORY_CHUNKS {
    ULONG Address;
    PVOID Data;
    ULONG Length;
} MEMORY_CHUNKS, *PMEMORY_CHUNKS;

typedef struct _UNICODE_STRING {
    USHORT Length;
    USHORT MaximumLength;
    PWSTR  Buffer;
} UNICODE_STRING;
typedef UNICODE_STRING *PUNICODE_STRING;
typedef const UNICODE_STRING *PCUNICODE_STRING;

typedef enum _WIN_PROCESS_INFORMATION_CLASS {
    ProcessBasicInformation,
    ProcessQuotaLimits,
    ProcessIoCounters,
    ProcessVmCounters,
    ProcessTimes,
    ProcessBasePriority,
    ProcessRaisePriority,
    ProcessDebugPort,
    ProcessExceptionPort,
    ProcessAccessToken,
    ProcessLdtInformation,
    ProcessLdtSize,
    ProcessDefaultHardErrorMode,
    ProcessIoPortHandlers,
    ProcessPooledUsageAndLimits,
    ProcessWorkingSetWatch,
    ProcessUserModeIOPL,
    ProcessEnableAlignmentFaultFixup,
    ProcessPriorityClass,
    ProcessWx86Information,
    ProcessHandleCount,
    ProcessAffinityMask,
    ProcessPriorityBoost,
    MaxProcessInfoClass
} WIN_PROCESS_INFORMATION_CLASS, *PWIN_PROCESS_INFORMATION_CLASS;

typedef struct _RTL_USER_PROCESS_PARAMETERS {
    BYTE Reserved1[16];
    PVOID Reserved2[5];
    UNICODE_STRING CurrentDirectory;
    PVOID Reserved3[3];
    UNICODE_STRING ImagePathName;
    UNICODE_STRING CommandLine;
} RTL_USER_PROCESS_PARAMETERS, *PRTL_USER_PROCESS_PARAMETERS;

typedef struct _PEB_LDR_DATA {
    BYTE Reserved1[8];
    PVOID Reserved2[3];
    LIST_ENTRY InMemoryOrderModuleList;
} PEB_LDR_DATA, *PPEB_LDR_DATA;

typedef struct _PEB {
    BYTE Reserved1[2];
    BYTE BeingDebugged;
    BYTE Reserved2[1];
    PVOID Reserved3[2];
    PPEB_LDR_DATA Ldr;
    PRTL_USER_PROCESS_PARAMETERS ProcessParameters;
    BYTE Reserved4[104];
    PVOID Reserved5[52];
    PVOID PostProcessInitRoutine;
    BYTE Reserved6[128];
    PVOID Reserved7[1];
    ULONG SessionId;
} PEB, *PPEB;

typedef struct _WIN_PROCESS_BASIC_INFORMATION {
    PVOID Reserved1;
    PPEB PebBaseAddress;
    PVOID Reserved2[2];
    ULONG_PTR UniqueProcessId;
    PVOID Reserved3;
} WIN_PROCESS_BASIC_INFORMATION;
typedef WIN_PROCESS_BASIC_INFORMATION *PWIN_PROCESS_BASIC_INFORMATION;

typedef enum _WIN_THREAD_INFORMATION_CLASS {
    ThreadBasicInformation,
    ThreadTimes,
    ThreadPriority,
    ThreadBasePriority,
    ThreadAffinityMask,
    ThreadImpersonationToken,
    ThreadDescriptorTableEntry,
    ThreadEnableAlignmentFaultFixup,
    ThreadEventPair,
    ThreadQuerySetWin32StartAddress,
    ThreadZeroTlsCell,
    ThreadPerformanceCount,
    ThreadAmILastThread,
    ThreadIdealProcessor,
    ThreadPriorityBoost,
    ThreadSetTlsArrayAddress,
    ThreadIsIoPending,
    ThreadHideFromDebugger
} WIN_THREAD_INFORMATION_CLASS, *PWIN_THREAD_INFORMATION_CLASS;

typedef struct _CLIENT_ID {
    HANDLE UniqueProcess;
    HANDLE UniqueThread;
} CLIENT_ID, *PCLIENT_ID;

typedef struct _W32THREAD {
    PVOID           pEThread;
    unsigned long   RefCount;
    PVOID           ptlW32;
    PVOID           pgdiDcattr;
    PVOID           pgdiBrushAttr;
    PVOID           pUMPDObjs;
    PVOID           pUMPDHeaps;
    unsigned long   dwEngAcquireCount;
    PVOID           pSemTable;
    PVOID           pUMPDObj;
} W32THREAD, *PW32THREAD;

// dt nt!_teb
typedef struct _TEB {
    NT_TIB                  Tib;
    PVOID                   EnvironmentPointer;
    CLIENT_ID               Cid;
    PVOID                   ActiveRpcInfo;
    PVOID                   ThreadLocalStoragePointer;
    PPEB                    Peb;
    ULONG                   LastErrorValue;
    ULONG                   CountOfOwnedCriticalSections;
    PVOID                   CsrClientThread;
    PVOID                   Win32ThreadInfo;
    ULONG                   Win32ClientInfo[0x1F];
    PVOID                   WOW32Reserved;
    ULONG                   CurrentLocale;
    ULONG                   FpSoftwareStatusRegister;
    PVOID                   SystemReserved1[0x36];
    PVOID                   Spare1;
    ULONG                   ExceptionCode;
    ULONG                   SpareBytes1[0x28];
    PVOID                   SystemReserved2[0xA];
    ULONG                   GdiRgn;
    ULONG                   GdiPen;
    ULONG                   GdiBrush;
    CLIENT_ID               RealClientId;
    PVOID                   GdiCachedProcessHandle;
    ULONG                   GdiClientPID;
    ULONG                   GdiClientTID;
    PVOID                   GdiThreadLocaleInfo;
    PVOID                   UserReserved[5];
    PVOID                   GlDispatchTable[0x118];
    ULONG                   GlReserved1[0x1A];
    PVOID                   GlReserved2;
    PVOID                   GlSectionInfo;
    PVOID                   GlSection;
    PVOID                   GlTable;
    PVOID                   GlCurrentRC;
    PVOID                   GlContext;
    NTSTATUS                LastStatusValue;
    UNICODE_STRING          StaticUnicodeString;
    WCHAR                   StaticUnicodeBuffer[0x105];
    PVOID                   DeallocationStack;
    PVOID                   TlsSlots[0x40];
    LIST_ENTRY              TlsLinks;
    PVOID                   Vdm;
    PVOID                   ReservedForNtRpc;
    PVOID                   DbgSsReserved[0x2];
    ULONG                   HardErrorDisabled;
    PVOID                   Instrumentation[0x10];
    PVOID                   WinSockData;
    ULONG                   GdiBatchCount;
    ULONG                   Spare2;
    ULONG                   Spare3;
    ULONG                   Spare4;
    PVOID                   ReservedForOle;
    ULONG                   WaitingOnLoaderLock;
    PVOID                   StackCommit;
    PVOID                   StackCommitMax;
    PVOID                   StackReserved;
} TEB, *PTEB;

typedef struct _THREAD_BASIC_INFORMATION {
    NTSTATUS                ExitStatus;
    PTEB                    TebBaseAddress;
    CLIENT_ID               ClientId;
    KAFFINITY               AffinityMask;
    ULONG                   Priority;
    ULONG                   BasePriority;
} WIN_THREAD_BASIC_INFORMATION, *PWIN_THREAD_BASIC_INFORMATION;

NTSTATUS AdaNtQueryInformationProcess(HANDLE ProcessHandle,
                                      WIN_PROCESS_INFORMATION_CLASS ProcessInformationClass,
                                      PVOID ProcessInformation,
                                      ULONG ProcessInformationLength,
                                      PULONG ReturnLength);

NTSTATUS AdaNtQueryInformationThread(HANDLE ThreadHandle,
                                     WIN_THREAD_INFORMATION_CLASS ThreadInformationClass,
                                     PVOID ThreadInformation,
                                     ULONG ThreadInformationLength,
                                     PULONG ReturnLength);