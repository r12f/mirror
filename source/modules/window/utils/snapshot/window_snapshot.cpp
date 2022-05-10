#include "stdafx.h"
#include "modules/window/utils/snapshot/window_snapshot.h"
#include "modules/window/utils/snapshot/window_snapshot_impl.h"

CComPtr<IWindowSnapshot>
CreateWindowSnapshot()
{
    CComPtr<IWindowSnapshot> pWindowSnapshot;
    WindowSnapshot::CreateObject(__uuidof(IWindowSnapshot), (void **)&pWindowSnapshot);
    return pWindowSnapshot;
}