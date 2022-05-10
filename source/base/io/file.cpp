#include "stdafx.h"
#include "file.h"

bool 
File::Exists(LPCTSTR strPath)
{
    if (NULL == strPath) {
        return false;
    }

    DWORD nFileAttribute = ::GetFileAttributes(strPath);
    if (nFileAttribute == INVALID_FILE_ATTRIBUTES) {
        return false;
    }

    return (nFileAttribute & FILE_ATTRIBUTE_DIRECTORY) == 0;
}