// pch.h: 미리 컴파일된 헤더 파일입니다.
// 아래 나열된 파일은 한 번만 컴파일되었으며, 향후 빌드에 대한 빌드 성능을 향상합니다.
// 코드 컴파일 및 여러 코드 검색 기능을 포함하여 IntelliSense 성능에도 영향을 미칩니다.
// 그러나 여기에 나열된 파일은 빌드 간 업데이트되는 경우 모두 다시 컴파일됨.
// 빌드 소요 시간이 증가될 수 있음.
#pragma warning(disable : 4819)

#ifndef PCH_H
#define PCH_H
// 여기에 미리 컴파일하려는 헤더 추가
#include <memory>
#include <LoggingMacro.h>
#include <Packet.h>

#pragma comment(lib, "ws2_32.lib")

#ifndef POCO_STATIC
#define POCO_STATIC 1
#endif

#include <StringUtil.h>
#include <DefineMacro.h>
//#include "ServiceHelper.h"

#pragma warning(push)
#pragma warning(disable : 26812)
#include <Protocol_generated.h>
#pragma warning(pop)

#include <PGPDefine.h>

#endif //PCH_H
