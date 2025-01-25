#pragma once

#include "ServiceDivision.h"


#pragma region | 국내 라이브 서비스 전용 Define
#ifdef KR_SERVICE

#endif


#pragma region | 테스트 전용 Define
#ifdef DEV_TEST
#ifdef KR_SERVICE

#endif // KR_SERVICE
#else


#endif // DEV_TEST