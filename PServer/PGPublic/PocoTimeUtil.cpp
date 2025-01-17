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
    if(_year < 0 || _year > 9999
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

Poco::DateTime PocoTimeUtil::PocoTimeToIntTime(const int64_t& _time, bool* _isSuccess)
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
