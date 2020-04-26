#include "Holiday.h"

Holiday::Holiday()
{
    setName(QString());
    setMonth(MONTH_PRAIOS);
    setDay(1);
    setDayReference(HolidayDayReference::MONTH);
    setDurationReference(HolidayDurationReference::FORWARD);
    setDuration(1);
    setWeekdayCount(1);
}

Holiday::Holiday(const QString& new_name, const Month new_month, const int new_day, const HolidayDayReference new_day_reference, const HolidayDurationReference new_duration_reference, const int new_duration, const int new_weekday_count)
{
    setName(new_name);
    setMonth(new_month);
    setDay(new_day);
    setDayReference(new_day_reference);
    setDurationReference(new_duration_reference);
    setDuration(new_duration);
    setWeekdayCount(new_weekday_count);
}

int Holiday::referenceDay(const int test_year, const Reckoning test_reckoning)
{
    if (dayReference() == HolidayDayReference::MONTH)
        return day();
    else if (dayReference() == HolidayDayReference::WEEKDAY_FIRST) {
        int day_in_month = Calendar::firstWeekdayInMonth(day(), month(), test_year, test_reckoning);
        day_in_month += Calendar::daysInWeek()*(weekdayCount()-1);
        return day_in_month;
    }
    else if (dayReference() == HolidayDayReference::WEEKDAY_LAST) {
        int day_in_month = Calendar::lastWeekdayInMonth(day(), month(), test_year, test_reckoning);
        day_in_month -= Calendar::daysInWeek()*(weekdayCount()-1);
        return day_in_month;
    }
    else
        return 0;
}

bool Holiday::isAtDate(const int test_day, const Month test_month, const int test_year, const Reckoning test_reckoning)
{
    int ref_dh = Calendar::daysAfterStandardReckoning(referenceDay(test_year, test_reckoning), month(), test_year, test_reckoning);
    int test_dh = Calendar::daysAfterStandardReckoning(test_day, test_month, test_year, test_reckoning);
    int dist = (test_dh - ref_dh) * ((durationReference() == HolidayDurationReference::BACKWARD) ? -1 : 1);
    return ( (dist >= 0) && (dist < duration()) );
}
