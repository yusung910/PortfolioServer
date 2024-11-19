#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

//flatbuffer���� min, max �Լ��� �ߺ��� �����ϱ� ���� define
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