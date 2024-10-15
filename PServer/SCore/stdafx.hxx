#pragma once

// System
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <MSWSock.h>
#include <Windows.h>

// Standard Libs
#include <cstdint>

//External
#include <lz4.h>
#pragma comment(lib, "lz4.lib")

#include "DefineMacro.h"
#include "StringUtil.h"
#include "LoggingMacro.h"
//
#include "BuildConfig.hxx"