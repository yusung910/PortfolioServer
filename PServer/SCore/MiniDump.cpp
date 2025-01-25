#include "stdafx.hxx"
#include "MiniDump.h"

#pragma warning(disable : 26812)	// enum class......

// MiniDump
#include <BugTrap.h>
#pragma warning(disable : 4091)	// dbghelp.h typedef 경고.
#include <dbghelp.h>
#pragma warning(disable : 4091)	// dbghelp.h typedef 경고.

#include <filesystem>


#ifdef _M_X64 
#	ifdef _DEBUG
#		pragma comment(lib, "BugTrapUD-x64.lib")
#	else
#		pragma comment(lib, "BugTrapU-x64.lib")
#	endif
#else
#	ifdef _DEBUG
#		pragma comment(lib, "BugTrapUD.lib")
#	else
#		pragma comment(lib, "BugTrapU.lib")
#	endif
#endif

void MiniDump::SetupExceptionHandler(bool _autoRestart)
{
    // 현재 실행파일(.exe) 이름찾기
    wchar_t appName[MAX_PATH] = {};
    GetModuleFileNameW(nullptr, appName, MAX_PATH);

    std::filesystem::path fullPath(appName);

    DWORD lDumpType = MiniDumpWithFullMemory // the contents of every readable page in the process address space is included in the dump.
        | MiniDumpWithHandleData // includes info about all handles in the process handle table.
        | MiniDumpWithThreadInfo // includes thread times, start address and affinity.
        | MiniDumpWithProcessThreadData // includes contents of process and thread environment blocks.
        | MiniDumpWithFullMemoryInfo // includes info on virtual memory layout.
        | MiniDumpWithUnloadedModules // includes info from recently unloaded modules if supported by OS.
        | MiniDumpWithFullAuxiliaryState // requests that aux data providers include their state in the dump.
        | MiniDumpIgnoreInaccessibleMemory // ignore memory read failures.
        | MiniDumpWithTokenInformation // includes security token related data.
        ;

    // BugTrap 등록
    BT_InstallSehFilter();
    BT_SetAppName(fullPath.stem().c_str());

    if (true == _autoRestart)
        BT_SetFlags(BTF_DETAILEDMODE | BTF_RESTARTAPP);
    else
        BT_SetFlags(BTF_DETAILEDMODE);

    BT_SetActivityType(BTA_SAVEREPORT);
    BT_SetDumpType(lDumpType);

    std::wstring DumpPath = fullPath.parent_path().c_str();
    DumpPath.append(L".\\Dump\\");

    BT_SetReportFilePath(DumpPath.c_str());
    BT_SetReportFormat(BTRF_XML);
}

#pragma warning(default : 26812)
