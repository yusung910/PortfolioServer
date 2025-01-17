#pragma once

#ifndef POCO_STATIC
#define POCO_STATIC 1
#endif

#pragma warning (push)
#pragma warning (disable: 4244 4245 4100 4127)

#include <Poco/Data/Time.h>
#pragma warning (pop)

#include <cstdint>
#include <string>

//
// 년,월,일,시,분,초 데이터를 정수형(ex: 20250117124455)으로 사용할 때
// 각각 년, 월, 일, 시, 분, 초 데이터를 구하기 위한 변수 값
// 나누기(/)를 할 경우 해당 값
// 나머지 연산을 할 경우 해당 데이터를 제외한 데이터
//ex) 20250117124455 / 10000000000 = 2025
//    20250117124455 % 10000000000 = 117124455
// 단점 -> 각각의 데이터를 가져오기 위해선 년, 월, 일, 시, 분, 초 순서로 해야함
// 섞일 경우 데이터가 올바르게 추출이 안됨
constexpr int64_t INTTIME_YEAR = 10000000000;
constexpr int64_t INTTIME_MONTH = 100000000;
constexpr int64_t INTTIME_DAY = 1000000;
constexpr int64_t INTTIME_HOUR = 10000;
constexpr int64_t INTTIME_MINUTE = 100;
constexpr int64_t INTTIME_SECOND = 1;

//특정 기간에 초기화가 필요할 경우 상수 선언한다.


class SCoreAPI PocoTimeUtil
{
public:
    static bool IsZeroTime(const Poco::DateTime& _date);
    static Poco::DateTime GetZeroTime();

    static Poco::DateTime GetLocalTime();
    static Poco::DateTime MakeTime(const int& _year, const int& _month, const int& _day, const int& _hour = 0, const int& _min = 0, const int& _sec = 0);

    static int GetLocalTimezone();

    //int -> Poco::DateTime, _isSuccess(double pointer)
    static int64_t PocoTimeToIntTime(const Poco::DateTime& _time, bool* _isSuccess = nullptr);
    //Poco::Datetime -> int
    static Poco::DateTime PocoTimeToIntTime(const int64_t& _time, bool* _isSuccess = nullptr);

    //int YYYYMMDD -> Poco::DateTime
    static bool ConvertYYYYMMDDToPocoDT(const int& _YYYYMMDD, Poco::DateTime& _dest);

    //Flatbuffer 형태 변환
    static void ConvertPocoDTtoDDateTimeT(const Poco::DateTime& _src, DDateTimeT& _dest);

    static void ConvertDDateTimeTtoPocoDT(const DDateTime* _src, Poco::DateTime& _dest);
    static void ConvertDDateTimeTtoPocoDT(const DDateTimeT& _src, Poco::DateTime& _dest);

};

