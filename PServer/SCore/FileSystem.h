#pragma once
#include "SCoreAPI.h"
#include <string>

class SCoreAPI FileSystem
{
public:
    static std::string GetExecutableFileName();
    static std::string GetExecutableDirName();
    static std::string GetExecutableFullPath();
    static bool Exist(const std::string& _path);

    static std::wstring GetExecutableFileNameW();
    static std::wstring GetExecutableDirNameW();
    static std::wstring GetExecutableFullPathW();
    static bool Exist(const std::wstring& _path);

private:
    FileSystem() = delete;
    ~FileSystem() = delete;
};

