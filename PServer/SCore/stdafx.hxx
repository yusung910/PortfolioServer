#pragma once
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

//flatbuffer에서 min, max 함수명 중복을 방지하기 위한 define
//https://github.com/google/flatbuffers/issues/105
#ifndef NOMINMAX
#	define NOMINMAX
#endif

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
#include "LoggingMacro.h"
#include "StringUtil.h"
//
#include "BuildConfig.hxx"