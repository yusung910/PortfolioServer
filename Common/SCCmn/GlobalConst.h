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

//게임 최대 명성(level)
const int32_t   G_N_MAX_LEVEL = 9999;

//AccountToken 최대 크기
constexpr int	ACCOUNT_UNIQUE_KEY_MAXSIZE = 256;

//싱크 맞출 MATRIX의 ENTRY 갯수
constexpr int   G_N_MAX_SYNC_ENTRY_COUNT = 9;

#ifdef DEV_TEST
constexpr size_t MAX_USER_CNT_CHANNEL = 50;
constexpr size_t CONFUSION_USER_CNT_CHANNEL = 50;
#else
constexpr size_t MAX_USER_CNT_CHANNEL = 200;
constexpr size_t CONFUSION_USER_CNT_CHANNEL = 100;
#endif // DEV_TEST

