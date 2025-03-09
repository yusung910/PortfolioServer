#include "stdafx.hxx"
#include "FileSystem.h"
#include <fstream>

std::string FileSystem::GetExecutableFileName()
{
    auto lFullPath = GetExecutableFullPath();

    char lFileName[MAX_PATH] = { 0, };

    //https://blog.naver.com/PostView.naver?blogId=rjseorl95&logNo=221781349064
    //Fullpath���� ����̺�, ��� ������ ����
    if (0 == _splitpath_s(lFullPath.c_str(), nullptr, 0, nullptr, 0, lFileName, 0, nullptr, 0))
        return lFileName;

    return "";
}

std::string FileSystem::GetExecutableDirName()
{
    auto lFullPath = GetExecutableFullPath();

    char lDrive[MAX_PATH] = { 0, };
    char lDir[MAX_PATH] = { 0, };

    //https://blog.naver.com/PostView.naver?blogId=rjseorl95&logNo=221781349064
    //Fullpath���� ����̺�, ��� ������ ����
    if (0 == _splitpath_s(lFullPath.c_str(), lDrive, MAX_PATH, lDir, MAX_PATH, nullptr, 0, nullptr, 0))
        return std::string(lDrive) + std::string(lDir);

    return "";
}

std::string FileSystem::GetExecutableFullPath()
{
    char lBuffer[MAX_PATH] = { 0, };

    // MSDN ����, return ���� buffer�� ��ϵ� ���ڿ� ���̶�� �����־�����, ������ �����ص� 0�� �ְ��ִ�.
    GetModuleFileNameA(nullptr, lBuffer, MAX_PATH);
    return lBuffer;
}

bool FileSystem::Exist(const std::string& _path)
{
    std::ifstream lPath(_path);
    return lPath.good();
}

std::wstring FileSystem::GetExecutableFileNameW()
{
    auto lFullPath = GetExecutableFullPathW();

    wchar_t lFileName[MAX_PATH] = { 0, };

    //https://blog.naver.com/PostView.naver?blogId=rjseorl95&logNo=221781349064
    //Fullpath���� ����̺�, ��� ������ ����
    if (0 == _wsplitpath_s(lFullPath.c_str(), nullptr, 0, nullptr, 0, lFileName, 0, nullptr, 0))
        return lFileName;

    return L"";
}

std::wstring FileSystem::GetExecutableDirNameW()
{
    auto lFullPath = GetExecutableFullPathW();

    wchar_t lDrive[MAX_PATH] = { 0, };
    wchar_t lDir[MAX_PATH] = { 0, };

    //https://blog.naver.com/PostView.naver?blogId=rjseorl95&logNo=221781349064
    //Fullpath���� ����̺�, ��� ������ ����
    if (0 == _wsplitpath_s(lFullPath.c_str(), lDrive, MAX_PATH, lDir, MAX_PATH, nullptr, 0, nullptr, 0))
        return std::wstring(lDrive) + std::wstring(lDir);

    return L"";
}

std::wstring FileSystem::GetExecutableFullPathW()
{
    wchar_t lBuffer[MAX_PATH] = { 0, };

    // MSDN ����, return ���� buffer�� ��ϵ� ���ڿ� ���̶�� �����־�����, ������ �����ص� 0�� �ְ��ִ�.
    GetModuleFileNameW(nullptr, lBuffer, MAX_PATH);
    return lBuffer;
}

bool FileSystem::Exist(const std::wstring& _path)
{
    std::ifstream lPath(_path);
    return lPath.good();
}
