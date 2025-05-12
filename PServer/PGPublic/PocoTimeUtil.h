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
// ��,��,��,��,��,�� �����͸� ������(ex: 20250117124455)���� ����� ��
// ���� ��, ��, ��, ��, ��, �� �����͸� ���ϱ� ���� ���� ��
// ������(/)�� �� ��� �ش� ��
// ������ ������ �� ��� �ش� �����͸� ������ ������
//ex) 20250117124455 / 10000000000 = 2025
//    20250117124455 % 10000000000 = 117124455
// ���� -> ������ �����͸� �������� ���ؼ� ��, ��, ��, ��, ��, �� ������ �ؾ���
// ���� ��� �����Ͱ� �ùٸ��� ������ �ȵ�
constexpr int64_t INTTIME_YEAR = 10000000000;
constexpr int64_t INTTIME_MONTH = 100000000;
constexpr int64_t INTTIME_DAY = 1000000;
constexpr int64_t INTTIME_HOUR = 10000;
constexpr int64_t INTTIME_MINUTE = 100;
constexpr int64_t INTTIME_SECOND = 1;

// Ư�� �Ⱓ�� �ʱ�ȭ�� �ʿ��� ��� ��� �����Ѵ�.
// �ϰ� �ʱ�ȭ �ð�
constexpr int DAILY_INIT_HOUR = 0; 
// �ְ� �ʱ�ȭ ����
constexpr Poco::DateTime::DaysOfWeek WEEKLY_INIT_DAY = Poco::DateTime::MONDAY;
// ���� �ʱ�ȭ ��¥
constexpr int MONTHLY_INIT_DAY = 1;

//DAY, HOUR
constexpr std::pair<Poco::DateTime::DaysOfWeek, int> WEEKLY_PAYMENT_INIT = std::make_pair(Poco::DateTime::WEDNESDAY, 10);	// �ְ� ���� �ݾ� �ʱ�ȭ 
class PocoTimeUtil
{
public:
    static bool IsZeroTime(const Poco::DateTime& _date);
    static Poco::DateTime GetZeroTime();

    static Poco::DateTime GetLocalTime();
    static Poco::DateTime MakeTime(const int& _year, const int& _month, const int& _day, const int& _hour = 0, const int& _min = 0, const int& _sec = 0);

    static int GetLocalTimezone();

    //Poco::Datetime -> int
    static int64_t PocoTimeToIntTime(const Poco::DateTime& _time, bool* _isSuccess = nullptr);
    //int -> Poco::DateTime, _isSuccess(double pointer)
    static Poco::DateTime IntTimeToPocoTime(const int64_t& _time, bool* _isSuccess = nullptr);

    //int YYYYMMDD -> Poco::DateTime
    static bool ConvertYYYYMMDDToPocoDT(const int& _YYYYMMDD, Poco::DateTime& _dest);

    //Poco::DateTime <-> Flatbuffer �� ��ȯ
    static void ConvertPocoDTtoDDateTimeT(const Poco::DateTime& _src, DDateTimeT& _dest);

    static void ConvertDDateTimeTtoPocoDT(const DDateTime* _src, Poco::DateTime& _dest);
    static void ConvertDDateTimeTtoPocoDT(const DDateTimeT& _src, Poco::DateTime& _dest);

    static flatbuffers::Offset<DDateTime> ConvertPocoDateTimeToFBOffset(flatbuffers::FlatBufferBuilder& _fb, const Poco::DateTime& _time);

    static Poco::DateTime AddDay(const Poco::DateTime& _time, const int& _day);
    static Poco::DateTime AddHour(const Poco::DateTime& _time, const int& _hour);
    static Poco::DateTime AddSecond(const Poco::DateTime& _time, const int& _sec);

    static std::string ToString(const Poco::DateTime& _dt);
    static std::wstring ToWString(const Poco::DateTime& _dt);

    static void ConvertYYYYMMDDtoPocoDateTime(const int& _YYYYMMDD, Poco::DateTime& _dest);

    static Poco::DateTime SumDateTimeAndSecond(const Poco::DateTime& _dt, const int& _sec);

    static Poco::DateTime GetToday0AMTime();
    static Poco::DateTime GetToday5AMTime();

    //GameContents Init Time
    static Poco::DateTime GetLastDailyInitTime();
    static Poco::DateTime GetLastWeeklyInitTime();
    static Poco::DateTime GetLastMonthlyInitTime();

    //
    static Poco::DateTime GetLastWeekly10AMInitTime();


    static int64_t PocoTimetoTick(const Poco::DateTime& _time);
    static Poco::DateTime TicktoPocoTime(const int64_t _tick);


};

