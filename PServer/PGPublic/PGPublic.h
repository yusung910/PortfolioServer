#pragma once

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.

#ifndef NOMINMAX
#	define NOMINMAX
#endif

// Windows

#include <Windows.h>	// include WinSock2.h. 네트워크 부분은 라이브러리에서 사용해서 먼저 Winsock2.h 를 include 하지 않음.
#pragma comment(lib, "ws2_32.lib")	// error LNK2001: 확인할 수 없는 외부 기호 __imp_htonl. // Packet.h 에서 사용.

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

// 자주 바뀌는 부분이지만, 
// 미리 컴파일된 헤더에 넣는게 빌드가 훨씬 빠름
#pragma warning(push)
#pragma warning(disable : 26812 26439)

#include <Protocol_generated.h>
#include <SCCmnPublic.h>
#include <ServiceDivision.h>

#include "EResult.h"

#include "PGPDefine.h"
#include "PConstVars.h"

#include <InnerPacket.h>