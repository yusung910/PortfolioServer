#pragma once
#include <cstdint>


//Server에서만 사용되는 상수 모음
constexpr int	    TRAFFIC_CHECK_TIME_MS = 30000; //30 sec
constexpr int		POOL_CHECK_TIME_MS = 300000;	// 5min;	
constexpr int		TITLE_CHECK_TIME_MS = 5000;	// 5 sec;	
constexpr int		UPDATE_TIMER_1_SECOND_MS = 1000;	// 1초
constexpr int		UPDATE_TIMER_1_MINUTE_MS = 60000;	// 1분