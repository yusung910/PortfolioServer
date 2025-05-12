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


