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


// ���� �������� ����� ������ ����

//UTF-32���� �����ϱ� ���� ���� �� ���� ���ؾ��� ũ��
const int32_t   G_N_CHAR_SIZE = 4;

//���� �ִ� ��(level)
const int32_t   G_N_MAX_LEVEL = 9999;

//AccountToken �ִ� ũ��
constexpr int	ACCOUNT_UNIQUE_KEY_MAXSIZE = 256;

//��ũ ���� MATRIX�� ENTRY ����
constexpr int   G_N_MAX_SYNC_ENTRY_COUNT = 9;

#ifdef DEV_TEST
constexpr size_t MAX_USER_CNT_CHANNEL = 50;
constexpr size_t CONFUSION_USER_CNT_CHANNEL = 50;
#else
constexpr size_t MAX_USER_CNT_CHANNEL = 200;
constexpr size_t CONFUSION_USER_CNT_CHANNEL = 100;
#endif // DEV_TEST

