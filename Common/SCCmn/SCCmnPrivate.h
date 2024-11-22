#pragma once


#ifndef SERVER_BUILD
#define CLIENT_ONLY

#endif // !SERVER_BUILD

#include <map>
#include <array>

#include "SCCmnPublic.h"

#define NOMINMAX

#if _WIN32
#include <WinSock2.h>
#else
#define UNREFERENCED_PARAMETER(P)          (P)
#define DBG_UNREFERENCED_PARAMETER(P)      (P)
#define DBG_UNREFERENCED_LOCAL_VARIABLE(V) (V)
#endif // _WIN32

