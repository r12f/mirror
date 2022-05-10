#pragma once

class Directory
{
public:
    static bool Exists(LPCTSTR strPath);
    static bool Create(LPCTSTR strPath);
};