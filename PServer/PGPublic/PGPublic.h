#pragma once

#define WIN32_LEAN_AND_MEAN             // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.

#ifndef NOMINMAX
#	define NOMINMAX
#endif

// Windows

#include <Windows.h>	// include WinSock2.h. ��Ʈ��ũ �κ��� ���̺귯������ ����ؼ� ���� Winsock2.h �� include ���� ����.
#pragma comment(lib, "ws2_32.lib")	// error LNK2001: Ȯ���� �� ���� �ܺ� ��ȣ __imp_htonl. // Packet.h ���� ���.

// end, Windows

// STL Containers
#include <vector>
#include <deque>
#include <queue>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <array>
#include <set>
// end, Containers

// STL I/O
#include <filesystem>
#include <iostream>
#include <fstream>
// end, STL I/O

// STL Utilities
#include <chrono>
#include <utility>
#include <algorithm>
#include <functional>
#include <memory>
#include <mutex>
#include <type_traits>
#include <initializer_list>
#include <regex>
#include <string>
// end, STL Utilites

// Legacy Include 
#include <cmath>
#include <cstdint>
// end, Legacy Include

#include <LoggingMacro.h>
#include <Packet.h>

#include <StringUtil.h>
#include <DefineMacro.h>
#include <Clock.h>

// ���� �ٲ�� �κ�������, 
// �̸� �����ϵ� ����� �ִ°� ���尡 �ξ� ����
#pragma warning(push)
#pragma warning(disable : 26812 26439)

#include <Protocol_generated.h>
#include <SCCmnPublic.h>
#include <ServiceDivision.h>

#include "EResult.h"

#include "PGPDefine.h"
#include "PConstVars.h"

#include <InnerPacket.h>