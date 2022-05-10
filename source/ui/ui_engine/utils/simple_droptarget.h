#pragma once

class CSimpleDropTarget
{
public:
    BEGIN_MSG_MAP_EX(CSimpleDropTarget)
        MSG_WM_DROPFILES(OnDropFiles)
    END_MSG_MAP()

private:
    VOID OnDropFiles(HDROP hDrop);

protected:
    VOID InitDropTarget(HWND hWnd);
    virtual VOID OnFileDropped(LPCTSTR pFilePath) {}
};