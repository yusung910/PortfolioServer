#pragma once
#include "ServiceDivision.h"

#ifdef _MSC_VER
#include <cstdint>
#else
#include "stdint.h"
#endif

#ifndef SERVER_BUILD
    #ifndef
        #define CLIENT_ONLY
    #endif
#endif // !SERVER_BUILD


// ���� �������� ����� ������ ����

//UTF-32���� �����ϱ� ���� ���� �� ���� ���ؾ��� ũ��
const int32_t   G_N_CHAR_SIZE = 4;

//���� �ִ� ����
const int32_t   G_N_MAX_LEVEL = 100;

//������ �� ������ ����Ʈ
const int32_t   G_N_LEVEL_UP_STATUS_POINT = 5;
