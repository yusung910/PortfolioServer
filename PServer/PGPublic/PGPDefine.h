#pragma once

#include "ServiceDivision.h"

#define __UNLIKELY__ [[unlikely]]
#define __LIKELY__ [[likely]]

#pragma region | ���� ���̺� ���� ���� Define
#ifdef KR_SERVICE

#endif


#pragma region | �׽�Ʈ ���� Define
#ifdef DEV_TEST
#ifdef KR_SERVICE

#endif // KR_SERVICE
#else


#endif // DEV_TEST