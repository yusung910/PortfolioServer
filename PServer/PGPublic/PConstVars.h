#pragma once
#include <cstdint>


//Server������ ���Ǵ� ��� ����
constexpr int	    PG_TRAFFIC_CHECK_TIME_MS = 10000; //30 sec
constexpr int		PG_POOL_CHECK_TIME_MS = 300000;	// 5min;	
constexpr int		PG_TITLE_CHECK_TIME_MS = 5000;	// 5 sec;	
constexpr int		PG_UPDATE_TIMER_1_SECOND_MS = 1000;	// 1��
constexpr int		PG_UPDATE_TIMER_1_MINUTE_MS = 60000;	// 1��
constexpr int       PG_CHECK_RECONNECT_TIME_MS = 1000;

constexpr int		PG_N_ZONE_SYNC_INTERVAL = 166;
constexpr int		PG_N_ZONE_ATTACK_INTERVAL = 33;	// (1000/30) ���ݿ� �ʿ��� �ּ�1�����ӽð�(30�����Ӱ���)

