#pragma once
#include "ServiceDivision.h"

#ifdef _MSC_VER
#include <cstdint>
#else
#include "stdint.h"
#endif

#ifndef SERVER_BUILD
#ifndef CLIENT_ONLY
#define CLIENT_ONLY
#endif
#endif // !SERVER_BUILD


// 서버 전역에서 사용할 변수들 선언

//UTF-32까지 지원하기 위해 글자 수 계산시 곱해야할 크기
const int32_t   G_N_CHAR_SIZE = 4;

//게임 최대 레벨
const int32_t   G_N_MAX_LEVEL = 100;

//레벨업 시 증가할 포인트
const int32_t   G_N_LEVEL_UP_STATUS_POINT = 5;


//AccountUKey 최대 크기
constexpr int	ACCOUNT_UNIQUE_KEY_MAXSIZE = 256;