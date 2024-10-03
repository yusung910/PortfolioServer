#pragma once
#include <ctime>
#include <cstdint>

const long long WINDOWS_XNIX_TIME_GAP = 11644473600000ll;
/// <summary>
/// Unix(timestamp), Windows(Tick), ���ڿ� �ð�(yyyyMMdd ��) ���� ������ ��ȯ�� ���� Ŭ����
/// </summary>
class TimeUtil
{
	/// <summary>
	/// ���ڰ����� ���޹��� tick(64bit)�� timestamp(time_t)�� ��ȯ
	/// </summary>
	/// <param name="val">tick</param>
	/// <returns>Timestamp</returns>
	static time_t ConvertTick64ToTimestamp(const int64_t& _tick)
	{
		if (_tick == 0ll)
			return 0;

		// '11644473600000' ���� ����ϴ� ����
		// timestamp�� 1970-01-01�� �������� �����ϴ� ������
		// tick�� 1601-01-01�� �������� �����ϴ� ������
		// 1601-01-01 ���� 1970-01-01������ ��¥�� ��(second)�� ����ϸ� 11644473600000
		// tick�� millisec�̱� ������ ����� ������ 1000�� ������
		//��ó : https://sunshine2k.blogspot.com/2014/08/where-does-116444736000000000-come-from.html
		time_t localRet = (_tick - WINDOWS_XNIX_TIME_GAP) / 1000ll;
		return localRet;
	}
		
	/// <summary>
	/// ���ڰ����� ���޹��� Timestamp(time_t)�� tick(int64_t)�� ��ȯ
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
	/// ���ڰ����� ���޹��� date(yyyyMMddHHmmss)�� Timestamp�� ��ȯ
	/// GMT+0 �׸���ġ õ���� �������� default�� ����
	/// ������ �׸���ġ õ���뺸�� 9�ð��� ���� ��� (GMT+9) _inputTimeZone = 9
	/// </summary>
	/// <param name="val">Timestamp</param>
	/// <param name="inputTimeZone">��������� ���� timezone</param>
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
	/// ���ڰ����� ���޹��� ��¥(date)�� tick���� ��ȯ
	/// </summary>
	/// <param name="_date">��¥</param>
	/// <param name="inputTimeZone">timezone</param>
	/// <returns></returns>
	static int64_t ConvertyyyyMMddHHmmssToTick64(const int64_t& _date, const int& _inputTimeZone = 0)
	{
		return ConvertTimestampToTick64(ConvertyyyyMMddHHmmssToTimestamp(_date, _inputTimeZone));
	}

		
	/// <summary>
	/// ���ڰ����� ���޹��� Timestamp�� yyyyMMddHHmmss������ date�� ��ȯ�Ѵ�
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
	/// ���ڰ����� ���޹��� tick���� yyyyMMddHHmmss ������ date�� ��ȯ�Ѵ�
	/// </summary>
	/// <param name="_tick">The tick.</param>
	/// <param name="_outputTimeZone">The output time zone.</param>
	/// <returns></returns>
	static int64_t ConvertTick64ToyyyyMMddHHmmss(const int64_t& _tick, const int& _outputTimeZone = 0)
	{
		return ConvertTimestampToyyyyMMddHHmmss(ConvertTick64ToTimestamp(_tick), _outputTimeZone);
	}


};