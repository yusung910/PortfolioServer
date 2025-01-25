#include "PGPPrivate.h"
#include "PocoTimeUtil.h"

#include <Poco/Timezone.h>
#include <TimeUtil.h>

bool PocoTimeUtil::IsZeroTime(const Poco::DateTime& _date)
{
    if (_date.year() == 1900
        && _date.month() == 1
        && _date.day() == 1)
        return true;

    return false;
}

Poco::DateTime PocoTimeUtil::GetZeroTime()
{
    return Poco::DateTime(1900, 1, 1);
}

Poco::DateTime PocoTimeUtil::GetLocalTime()
{
    Poco::DateTime lTime;
    lTime.makeLocal(Poco::Timezone::tzd());
    return lTime;
}

Poco::DateTime PocoTimeUtil::MakeTime(const int& _year, const int& _month, const int& _day, const int& _hour, const int& _min, const int& _sec)
{
    if (_year < 0 || _year > 9999
        || _month <= 0 || _month > 12
        || _day <= 0 || _day > 31
        || _hour < 0 || _hour >= 24
        || _min < 0 || _min >= 60
        || _sec < 0 || _sec >= 60)
        return Poco::DateTime();

    return Poco::DateTime(_year, _month, _day, _hour, _min, _sec);
}

int PocoTimeUtil::GetLocalTimezone()
{
    //tzd() : utc(세계 협정시) + dst(일광 절약 시간제: 표준 시 보다 1시간 앞당긴 값) 값을 반환
    return Poco::Timezone::tzd() / 3600;
}

int64_t PocoTimeUtil::PocoTimeToIntTime(const Poco::DateTime& _time, bool* _isSuccess)
{
    int64_t lRetVal = 0;
    lRetVal += _time.year() * INTTIME_YEAR;
    lRetVal += _time.month() * INTTIME_MONTH;
    lRetVal += _time.day() * INTTIME_DAY;
    lRetVal += _time.hour() * INTTIME_HOUR;
    lRetVal += _time.minute() * INTTIME_MINUTE;
    lRetVal += _time.second() * INTTIME_SECOND;

    if (nullptr != _isSuccess)
        *_isSuccess = true;

    return 0;
}

Poco::DateTime PocoTimeUtil::IntTimeToPocoTime(const int64_t& _time, bool* _isSuccess)
{
    if (_time < 0)
    {
        if (_isSuccess != nullptr)
            *_isSuccess = false;
        return Poco::DateTime();
    }

    int64_t lWork = _time;

    int lYear = (int)(lWork / INTTIME_YEAR);
    lWork &= INTTIME_YEAR;
    int lMonth = (int)(lWork / INTTIME_MONTH);
    lWork &= INTTIME_MONTH;
    int lDay = (int)(lWork / INTTIME_DAY);
    lWork &= INTTIME_DAY;
    int lHour = (int)(lWork / INTTIME_HOUR);
    lWork &= INTTIME_HOUR;
    int lMin = (int)(lWork / INTTIME_MINUTE);
    lWork &= INTTIME_MINUTE;
    int lSec = (int)(lWork / INTTIME_SECOND);
    lWork &= INTTIME_SECOND;

    if (lYear < 0 || lYear > 9999
        || lMonth <= 0 || lMonth > 12
        || lDay <= 0 || lDay > 31
        || lHour < 0 || lHour >= 24
        || lMin < 0 || lMin >= 60
        || lSec < 0 || lSec >= 60)
    {
        if (nullptr != _isSuccess)
            *_isSuccess = false;
        return Poco::DateTime();
    }

    if (nullptr != _isSuccess)
        *_isSuccess = true;

    return Poco::DateTime(lYear, lMonth, lDay, lHour, lMin, lSec);
}

bool PocoTimeUtil::ConvertYYYYMMDDToPocoDT(const int& _YYYYMMDD, Poco::DateTime& _dest)
{
    if (_YYYYMMDD < 0)
        return false;

    int lYear = _YYYYMMDD / 10000;
    int lMonth = (_YYYYMMDD % 10000) / 100;
    int lDay = _YYYYMMDD % 100;

    if (lYear < 0 || lYear > 9999
        || lMonth <= 0 || lMonth > 12
        || lDay <= 0 || lDay > 31)
        return false;

    _dest.assign(lYear, lMonth, lDay);

    return true;
}

void PocoTimeUtil::ConvertPocoDTtoDDateTimeT(const Poco::DateTime& _src, DDateTimeT& _dest)
{
    _dest.Year = _src.year();
    _dest.Month = _src.month();
    _dest.Day = _src.day();
    _dest.Hour = _src.hour();
    _dest.Minute = _src.minute();
    _dest.Second = _src.second();
}

void PocoTimeUtil::ConvertDDateTimeTtoPocoDT(const DDateTime* _src, Poco::DateTime& _dest)
{
    if (nullptr == _src)
        return;

    _dest.assign(_src->Year(), _src->Month(), _src->Day(), _src->Hour(), _src->Minute(), _src->Second());
}

void PocoTimeUtil::ConvertDDateTimeTtoPocoDT(const DDateTimeT& _src, Poco::DateTime& _dest)
{
    _dest.assign(_src.Year, _src.Month, _src.Day, _src.Hour, _src.Minute, _src.Second);
}

flatbuffers::Offset<DDateTime> PocoTimeUtil::ConvertPocoDateTimeToFBOffset(flatbuffers::FlatBufferBuilder& _fb, const Poco::DateTime& _time)
{
    return CreateDDateTime(_fb, _time.year(), _time.month(), _time.day(), _time.hour(), _time.minute(), _time.second());
}

Poco::DateTime PocoTimeUtil::AddDay(const Poco::DateTime& _time, const int& _day)
{
    return _time + Poco::Timespan(_day, 0, 0, 0, 0);
}

Poco::DateTime PocoTimeUtil::AddHour(const Poco::DateTime& _time, const int& _hour)
{
    return _time + Poco::Timespan(0, _hour, 0, 0, 0);
}

Poco::DateTime PocoTimeUtil::AddSecond(const Poco::DateTime& _time, const int& _sec)
{
    return _time + Poco::Timespan(_sec, 0);
}

std::string PocoTimeUtil::ToString(const Poco::DateTime& _dt)
{
    char lStrTime[32] = { 0, };
    sprintf_s(lStrTime, sizeof(lStrTime), "[%04d-%02d-%02d %02d:%02d:%02d %03d]",
        _dt.year(), _dt.month(), _dt.day(), _dt.hour(), _dt.minute(), _dt.second(), _dt.millisecond());
    return std::string(lStrTime);
}

std::wstring PocoTimeUtil::ToWString(const Poco::DateTime& _dt)
{
    wchar_t lStrTime[32] = { 0, };
    swprintf_s(lStrTime, sizeof(lStrTime), L"[%04d-%02d-%02d %02d:%02d:%02d %03d]",
        _dt.year(), _dt.month(), _dt.day(), _dt.hour(), _dt.minute(), _dt.second(), _dt.millisecond());
    return std::wstring(lStrTime);
}

void PocoTimeUtil::ConvertYYYYMMDDtoPocoDateTime(const int& _YYYYMMDD, Poco::DateTime& _dest)
{
    if (_YYYYMMDD <= 10000)
        return;

    _dest = Poco::DateTime(
        _YYYYMMDD / 10000,
        (_YYYYMMDD % 10000) / 100,
        _YYYYMMDD % 100,
        0, 0, 0
    );
}

Poco::DateTime PocoTimeUtil::SumDateTimeAndSecond(const Poco::DateTime& _dt, const int& _sec)
{
    return _dt + Poco::Timespan(_sec, 0);
}

Poco::DateTime PocoTimeUtil::GetToday0AMTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), lCurTime.day(), 0, 0, 0);
}

Poco::DateTime PocoTimeUtil::GetToday5AMTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    //현재 시각이 05시 이전일 경우 그 전날의 새벽 5시를 반환한다. 
    if (lCurTime.hour() < 5)
        lCurTime = PocoTimeUtil::AddDay(lCurTime, -1);

    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), lCurTime.day(), 5, 0, 0);
}

Poco::DateTime PocoTimeUtil::GetLastDailyInitTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    if (lCurTime.hour() < DAILY_INIT_HOUR)
        lCurTime = PocoTimeUtil::AddDay(lCurTime, -1);
    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), lCurTime.day(), DAILY_INIT_HOUR, 0, 0);
}

Poco::DateTime PocoTimeUtil::GetLastWeeklyInitTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    auto lDayofWeek = lCurTime.dayOfWeek();

    auto lAddDay = 0;
    //초기화 요일이 지나기 전일 경우
    if (lDayofWeek < WEEKLY_INIT_DAY)
    {
        //그 전주의 초기화요일을 반환
        lAddDay = -7 + (WEEKLY_INIT_DAY - lDayofWeek);
    }
    else
    {
        lAddDay = (WEEKLY_INIT_DAY - lDayofWeek);
    }

    lCurTime = PocoTimeUtil::AddDay(lCurTime, lAddDay);

    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), lCurTime.day(), DAILY_INIT_HOUR, 0, 0);
}

Poco::DateTime PocoTimeUtil::GetLastMonthlyInitTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    if (lCurTime.day() < MONTHLY_INIT_DAY)
        lCurTime = PocoTimeUtil::AddDay(lCurTime, -lCurTime.day());

    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), MONTHLY_INIT_DAY, 0, 0, 0);
}

Poco::DateTime PocoTimeUtil::GetLastWeekly10AMInitTime()
{
    auto lCurTime = PocoTimeUtil::GetLocalTime();
    auto lDayOfWeek = lCurTime.dayOfWeek();

    auto lInitDay = WEEKLY_PAYMENT_INIT.first;
    auto lInitHour = WEEKLY_PAYMENT_INIT.second;

    auto lAddDay = 0;
    if (lDayOfWeek < lInitDay
        || (lDayOfWeek == lInitDay && lCurTime.hour() < lInitHour))
        lAddDay = -7 + (lInitDay - lDayOfWeek);
    else
        lAddDay = (lInitDay - lDayOfWeek);

    lCurTime = PocoTimeUtil::AddDay(lCurTime, lAddDay);


    return PocoTimeUtil::MakeTime(lCurTime.year(), lCurTime.month(), lCurTime.day(), lInitHour, 0, 0);
}

int64_t PocoTimeUtil::PocoTimetoTick(const Poco::DateTime& _time)
{
    int64_t lMs = _time.millisecond();
    int lTimeZoneHour = Poco::Timezone::tzd() / 3600;   //second

    return TimeUtil::ConvertyyyyMMddHHmmssToTick64(PocoTimeToIntTime(_time), lTimeZoneHour) + lMs;
}

Poco::DateTime PocoTimeUtil::TicktoPocoTime(const int64_t _tick)
{
    long lMs = static_cast<long>(_tick % 1000);
    int lTimeZoneHour = Poco::Timezone::tzd() / 3600;

    return IntTimeToPocoTime(TimeUtil::ConvertTick64ToyyyyMMddHHmmss(_tick, lTimeZoneHour)) + Poco::Timespan(0, lMs * 1000);
}
