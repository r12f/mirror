#include "stdafx.h"
#include "directory.h"

bool
Directory::Exists(LPCTSTR strPath)
{
    if (NULL == strPath) {
        return false;
    }

    DWORD nFileAttribute = ::GetFileAttributes(strPath);
    if (nFileAttribute == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    return (nFileAttribute & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

bool
Directory::Create(LPCTSTR strPath)
{
    if (NULL == strPath) {
        return false;
    }

    CString strCurrentPath = strPath;
    int nPathLength = strCurrentPath.GetLength();
    if (nPathLength == 0) {
        return false;
    }

    if (strCurrentPath[nPathLength - 1] == '\\') {
        strCurrentPath.Delete(nPathLength - 1, 1);
    }

    int nBasePathEnd = 0;
    std::vector<CString> vPendingPaths;
    while (!Directory::Exists(strCurrentPath)) {
        vPendingPaths.push_back(strCurrentPath);

        nBasePathEnd = strCurrentPath.ReverseFind(_T('\\'));
        if (nBasePathEnd < 0) {
            break;
        }

        strCurrentPath = strCurrentPath.Left(nBasePathEnd);
    }

    for (auto itPendingPath = vPendingPaths.rbegin(); itPendingPath != vPendingPaths.rend(); ++itPendingPath) {
        if (!::CreateDirectory(*itPendingPath, NULL)) {
            return false;
        }
    }

    return true;
}