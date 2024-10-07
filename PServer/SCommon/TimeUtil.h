#pragma once
#include <ctime>
#include <cstdint>

const long long WINDOWS_XNIX_TIME_GAP = 11644473600000ll;
/// <summary>
/// Unix(timestamp), Windows(Tick), 문자열 시간(yyyyMMdd 등) 간의 데이터 변환을 위한 클래스
/// </summary>
class TimeUtil
{
	/// <summary>
	/// 인자값으로 전달받은 tick(64bit)을 timestamp(time_t)로 변환
	/// </summary>
	/// <param name="val">tick</param>
	/// <returns>Timestamp</returns>
	static time_t ConvertTick64ToTimestamp(const int64_t& _tick)
	{
		if (_tick == 0ll)
			return 0;

		// '11644473600000' 값을 사용하는 이유
		// timestamp는 1970-01-01을 기준으로 시작하는 데이터
		// tick는 1601-01-01을 기준으로 시작하는 데이터
		// 1601-01-01 부터 1970-01-01까지의 날짜를 초(second)로 계산하면 11644473600000
		// tick은 millisec이기 때문에 계산한 값에서 1000을 나눈다
		//출처 : https://sunshine2k.blogspot.com/2014/08/where-does-116444736000000000-come-from.html
		time_t localRet = (_tick - WINDOWS_XNIX_TIME_GAP) / 1000ll;
		return localRet;
	}
		
	/// <summary>
	/// 인자값으로 전달받은 Timestamp(time_t)를 tick(int64_t)로 변환
	/// </summary>
	/// <param name="_timestamp">The timestamp.</param>
	/// <returns>tick</returns>
	static int64_t ConvertTimestampToTick64(const time_t& _timestamp)
	{
		if (_timestamp == 0)
			return 0ll;

		int64_t localRet = (_timestamp * 1000ll) + WINDOWS_XNIX_TIME_GAP;
		return localRet;
	}

	
	/// <summary>
	/// 인자값으로 전달받은 date(yyyyMMddHHmmss)를 Timestamp로 변환
	/// GMT+0 그리니치 천문대 기준으로 default값 지정
	/// 시차가 그리니치 천문대보다 9시간이 빠를 경우 (GMT+9) _inputTimeZone = 9
	/// </summary>
	/// <param name="val">Timestamp</param>
	/// <param name="inputTimeZone">시차계산을 위한 timezone</param>
	/// <returns></returns>
	static time_t ConvertyyyyMMddHHmmssToTimestamp(const int64_t& _date, const int& _inputTimeZone = 0)
	{
		if (_date <= 0)
			return 0;

		if (_inputTimeZone < -12
			|| _inputTimeZone > 12)
			return 0;

		std::tm tmTemp = {};
		tmTemp.tm_year = (int)(_date / 10000000000) - 1900;
		tmTemp.tm_mon = (int)((_date % 10000000000) / 100000000) - 1;
		tmTemp.tm_mday = (int)((_date % 100000000) / 1000000);
		tmTemp.tm_hour = (int)((_date % 1000000) / 10000);
		tmTemp.tm_min = (int)((_date % 10000) / 100);
		tmTemp.tm_sec = (int)(_date % 100);

		// Check Values
		if (tmTemp.tm_year < 0
			|| tmTemp.tm_mon < 0
			|| tmTemp.tm_mon >= 12
			|| tmTemp.tm_mday <= 0
			|| tmTemp.tm_mday > 31
			|| tmTemp.tm_hour < 0
			|| tmTemp.tm_hour >= 24
			|| tmTemp.tm_min < 0
			|| tmTemp.tm_min >= 60
			|| tmTemp.tm_sec < 0
			|| tmTemp.tm_sec >= 60)
			return 0;
		time_t tRet = _mkgmtime(&tmTemp);
		return tRet - (time_t)(_inputTimeZone * 3600ll);
	}
	
	/// <summary>
	/// 인자값으로 전달받은 날짜(date)를 tick으로 변환
	/// </summary>
	/// <param name="_date">날짜</param>
	/// <param name="inputTimeZone">timezone</param>
	/// <returns></returns>
	static int64_t ConvertyyyyMMddHHmmssToTick64(const int64_t& _date, const int& _inputTimeZone = 0)
	{
		return ConvertTimestampToTick64(ConvertyyyyMMddHHmmssToTimestamp(_date, _inputTimeZone));
	}

		
	/// <summary>
	/// 인자값으로 전달받은 Timestamp를 yyyyMMddHHmmss형식의 date로 변환한다
	/// </summary>
	/// <param name="_timestamp">timestamp</param>
	/// <param name="_outputTimeZone">The output time zone.</param>
	/// <returns></returns>
	static int64_t ConvertTimestampToyyyyMMddHHmmss(const time_t& _timestamp, const int& _outputTimeZone = 0)
	{
		time_t tTemp = _timestamp + (_outputTimeZone * 3600ll);
		int64_t nRet = 0;

#ifdef _MSC_VER
		std::tm tmTemp;
		if (0 != gmtime_s(&tmTemp, &tTemp))
			return nRet;
		std::tm* pTM = &tmTemp;
#else
		std::tm* pTM = gmtime(&tTemp);
		if (nullptr == pTM)
			return nRet;
#endif
		nRet = (pTM->tm_year + 1900ll) * 10000000000ll
			+ (pTM->tm_mon + 1ll) * 100000000ll
			+ pTM->tm_mday * 1000000ll
			+ pTM->tm_hour * 10000ll
			+ pTM->tm_min * 100ll
			+ pTM->tm_sec;

		return nRet;
	}
	
	/// <summary>
	/// 인자값으로 전달받은 tick값을 yyyyMMddHHmmss 형식인 date로 변환한다
	/// </summary>
	/// <param name="_tick">The tick.</param>
	/// <param name="_outputTimeZone">The output time zone.</param>
	/// <returns></returns>
	static int64_t ConvertTick64ToyyyyMMddHHmmss(const int64_t& _tick, const int& _outputTimeZone = 0)
	{
		return ConvertTimestampToyyyyMMddHHmmss(ConvertTick64ToTimestamp(_tick), _outputTimeZone);
	}


};