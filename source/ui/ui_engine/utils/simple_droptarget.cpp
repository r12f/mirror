#include "stdafx.h"
#include "simple_droptarget.h"

VOID
CSimpleDropTarget::InitDropTarget(HWND hWnd)
{
    if (!::IsWindow(hWnd)) {
        return;
    }

    ::DragAcceptFiles(hWnd, TRUE);

    return;
}

VOID
CSimpleDropTarget::OnDropFiles(HDROP hDrop)
{
    if (NULL == hDrop) {
        return;
    }

    UINT nFileCount = ::DragQueryFile(hDrop, (UINT)-1, NULL, 0);
    if (0 == nFileCount) {
        return;
    }

    std::vector<TCHAR> vFilePathBuffer;
    for (UINT nFileIndex = 0; nFileIndex < nFileCount; ++nFileIndex) {
        UINT nFilePathSize = ::DragQueryFile(hDrop, nFileIndex, NULL, 0);
        vFilePathBuffer.resize(nFilePathSize + 1);
        vFilePathBuffer[nFilePathSize] = 0;

        DragQueryFile(hDrop, nFileIndex, &vFilePathBuffer[0], vFilePathBuffer.size());
        OnFileDropped(&vFilePathBuffer[0]);
    }

    return;
}
