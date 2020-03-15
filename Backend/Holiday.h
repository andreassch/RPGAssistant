/**
 * Holiday class for the Role Playing Game Assistant.
 *
 * Copyright (c) 2020 Andreas Schneider (andreas underscore schn at web dot de)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 3
 * as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#pragma once

#include "../config.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA
#include "TDECalendar.h"
using namespace TDECalendar;
#endif

#include <QString>


enum class HolidayDayReference
{
    MONTH, WEEKDAY_FIRST, WEEKDAY_LAST
};

enum class HolidayDurationReference
{
    FORWARD, BACKWARD
};

class Holiday
{
public:
    Holiday();
    Holiday(const QString& new_name, const Month new_month, const int new_day, const HolidayDayReference new_day_reference, const HolidayDurationReference new_duration_reference, const int new_duration, const int new_weekday_count = 1);

    inline QString name() const { return m_name; }
    inline Month month() const { return m_month; }
    inline int day() const { return m_day; }
    inline HolidayDayReference dayReference() const { return m_day_reference; }
    inline int weekdayCount() const { return m_weekday_count; }
    inline HolidayDurationReference durationReference() const { return m_duration_reference; }
    inline int duration() const { return m_duration; }

    inline void setName(const QString& new_name) { m_name = new_name; }
    inline void setMonth(const Month new_month) { m_month = new_month; }
    inline void setDay(const int new_day) { m_day = new_day; }
    inline void setDayReference(const HolidayDayReference new_day_reference) { m_day_reference = new_day_reference; }
    inline void setWeekdayCount(const int new_weekday_count) { m_weekday_count = new_weekday_count; }
    inline void setDurationReference(const HolidayDurationReference new_reference) { m_duration_reference = new_reference; }
    inline void setDuration(const int new_duration) { m_duration = new_duration; }

    int referenceDay(const int test_year, const Reckoning test_reckoning);
    bool isAtDate(const int test_day, const Month test_month, const int test_year, const Reckoning test_reckoning);

private:
    QString m_name;
    Month m_month;
    int m_day;
    HolidayDayReference m_day_reference;
    int m_weekday_count;
    HolidayDurationReference m_duration_reference;
    int m_duration;
};
