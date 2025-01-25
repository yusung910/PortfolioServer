#include "pch.h"
#include <iostream>
#include <MiniDump.h>

#include "PGameServerApp.h"

//������ ���μ����� ����ǰ� �ִ��� Ȯ���Ѵ�.
bool IsRedundantExecution([[maybe_unused]] const std::wstring& _prjName = L"PSGame")
{
    HANDLE lMutex = nullptr;

    //���ǰų� ������ ���� mutex ��ü�� �����ϴ� �Լ�.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/synchapi/nf-synchapi-createmutexa
    lMutex = CreateMutex(nullptr, false, L"PGServer");

    if (nullptr == lMutex)
    {
        MessageBox(nullptr, L"Mutex ���� ���� - ���α׷��� �����մϴ�.", _prjName.c_str(), MB_OK);

        return false;
    }

    //������ �̹� ������ ����Ǿ��ִ��� ���θ� Ȯ���Ѵ�.
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(lMutex);
        lMutex = INVALID_HANDLE_VALUE;
        MessageBox(nullptr, L"���α׷��� �̹� ���� ���Դϴ�.", _prjName.c_str(), MB_OK);
        return false;
    }

    return true;
}

int main()
{
    std::cout << "GameServer Start!" << std::endl;

    if (false == IsRedundantExecution())
        return 0;

    MiniDump::SetupExceptionHandler();
    PGameServerApp app;
    if (false == app.Initialize())
        return -1;

    if (false == app.RunLoop())
        return -2;

    return 0;
}