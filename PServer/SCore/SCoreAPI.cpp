#include "stdafx.hxx"
#if __has_include("BuildVersion.hxx")
#include "BuildVersion.hxx"
#define CORE_BUILD_REV __LIBRARY_GIT_BRANCH_REV__
#else
#define CORE_BUILD_REV 1
#endif

int GetLibraryRev() noexcept
{
    return CORE_BUILD_REV;
}