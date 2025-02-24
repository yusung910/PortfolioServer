#pragma warning(disable : 4819)
#pragma once
#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

//flatbuffer���� min, max �Լ��� �ߺ��� �����ϱ� ���� define
//https://github.com/google/flatbuffers/issues/105
#ifndef NOMINMAX
#	define NOMINMAX
#endif


// Standard Libs
#include <cstdint>

//External
//#include <lz4.h>
#include <lz4hc.h>
#pragma comment(lib, "lz4.lib")