#pragma once

#include <WinInet.h>
#include "base/scoped_handle.h"

struct ScopedHandleTraitHINTERNET {
    static BOOL Destroy(HINTERNET h) { return ::InternetCloseHandle(h); }
    static HINTERNET NullHandle() { return NULL; }
};

typedef ScopedHandleBase<HINTERNET, ScopedHandleTraitHINTERNET> ScopedINetHandle;