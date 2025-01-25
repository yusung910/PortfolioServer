#include "pch.h"
#include <iostream>
#include <MiniDump.h>

#include "PGameServerApp.h"

//기존에 프로세스가 실행되고 있는지 확인한다.
bool IsRedundantExecution([[maybe_unused]] const std::wstring& _prjName = L"PSGame")
{
    HANDLE lMutex = nullptr;

    //명명되거나 명명되지 않은 mutex 객체를 생성하는 함수.
    //https://learn.microsoft.com/ko-kr/windows/win32/api/synchapi/nf-synchapi-createmutexa
    lMutex = CreateMutex(nullptr, false, L"PGServer");

    if (nullptr == lMutex)
    {
        MessageBox(nullptr, L"Mutex 생성 실패 - 프로그램을 종료합니다.", _prjName.c_str(), MB_OK);

        return false;
    }

    //기존에 이미 서버가 실행되어있는지 여부를 확인한다.
    if (::GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(lMutex);
        lMutex = INVALID_HANDLE_VALUE;
        MessageBox(nullptr, L"프로그램이 이미 실행 중입니다.", _prjName.c_str(), MB_OK);
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