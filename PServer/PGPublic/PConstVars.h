#pragma once
#include <cstdint>


//Server에서만 사용되는 상수 모음
constexpr int	    PG_TRAFFIC_CHECK_TIME_MS = 10000; //30 sec
constexpr int		PG_POOL_CHECK_TIME_MS = 300000;	// 5min;	
constexpr int		PG_TITLE_CHECK_TIME_MS = 5000;	// 5 sec;	
constexpr int		PG_UPDATE_TIMER_1_SECOND_MS = 1000;	// 1초
constexpr int		PG_UPDATE_TIMER_1_MINUTE_MS = 60000;	// 1분
constexpr int       PG_CHECK_RECONNECT_TIME_MS = 1000;

constexpr int		PG_N_ZONE_SYNC_INTERVAL = 166;
constexpr int		PG_N_ZONE_ATTACK_INTERVAL = 33;	// (1000/30) 공격에 필요한 최소1프레임시간(30프레임가정)

