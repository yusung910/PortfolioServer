#include "PGPPrivate.h"
#include "RevUtil.h"

#if __has_include("BuildVersion.hxx")
#include "BuildVersion.hxx"
#include "RevUtil.h"
#define SERVER_BUILD_REV __LIBRARY_GIT_BRANCH_REV__
#else
#define SERVER_BUILD_REV 1
#endif

int GetServerVer() noexcept
{
	return SERVER_BUILD_REV;
}

int GetServerLibVer() noexcept
{
	return GetServerVer();
}