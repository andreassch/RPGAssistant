#include "TDECalendar.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA

#include <cassert>
#include <QtDebug>

namespace TDECalendar {

    /* Implementation of Calendar class **************************************/
    Calendar::Calendar()
    {
    }

    int Calendar::daysInMonth(const Month month)
    {
        if (month == MONTH_NAMELESS)
            return 5;
        else
            return 30;
    }

    QString Calendar::monthName(const Month month)
    {
        const QString m_month_names[13] = {
            QObject::tr("Praios"), QObject::tr("Rondra"), QObject::tr("Efferd"), QObject::tr("Travia"),
            QObject::tr("Boron"), QObject::tr("Hesinde"), QObject::tr("Firun"), QObject::tr("Tsa"),
            QObject::tr("Phex"), QObject::tr("Peraine"), QObject::tr("Ingerimm"), QObject::tr("Rahja"),
            QObject::tr("Nameless") };
        assert((month >= 1) && (month <= noOfMonths()));
        return m_month_names[static_cast<int>(month-1)];
    }

    QString Calendar::weekdayName(const int weekday)
    {
        const QString weekday_names[7] = {
            QObject::tr("Windsday"), QObject::tr("Earthday"), QObject::tr("Marketday"), QObject::tr("Praiosday"),
            QObject::tr("Rohalsday"), QObject::tr("Fireday"), QObject::tr("Waterday")};
        assert((weekday >= 1) && (weekday <= daysInWeek()));
        return weekday_names[weekday-1];
    }

    QString Calendar::weekdayAbbreviation(const int weekday)
    {
        const QString weekday_abbreviations[7] = {
            QObject::tr("Wi"), QObject::tr("Ea"), QObject::tr("Ma"), QObject::tr("Pr"), QObject::tr("Ro"), QObject::tr("Fi"), QObject::tr("Wa")
        };
        assert((weekday >= 1) && (weekday <= daysInWeek()));
        return weekday_abbreviations[weekday-1];
    }

    QString Calendar::reckoningName(const Reckoning reckoning)
    {
        const QString reckoning_names[RECKONING_COUNT] = {QObject::tr("Hal"), QObject::tr("Fall of Bosparan")};
        return reckoning_names[reckoning];
    }

    QString Calendar::reckoningAbbreviation(const Reckoning reckoning)
    {
        const QString reckoning_abbreviations[RECKONING_COUNT] = {QObject::tr("Hal"), QObject::tr("BoF")};
        return reckoning_abbreviations[reckoning];
    }

    QString Calendar::reckoningSymbolicName(const Reckoning reckoning)
    {
        const char* reckoning_symbolic_names[RECKONING_COUNT] = {"Hal", "BF"};
        return reckoning_symbolic_names[reckoning];
    }

    Reckoning Calendar::parseReckoning(const QString &reckoning_symbolic_name)
    {
        for (int reckoning_no = 0; reckoning_no < noOfReckonings(); reckoning_no++) {
            if (QString::compare(reckoning_symbolic_name, reckoningSymbolicName(static_cast<Reckoning>(reckoning_no)), Qt::CaseInsensitive) == 0)
                return static_cast<Reckoning>(reckoning_no);
        }
        qDebug() << "Unknown reckoning" << reckoning_symbolic_name << "reverting to Hal";
        return RECKONING_HAL;
    }

    int Calendar::dayBasis(const int day, const Month month, const int year, const Reckoning reckoning)
    {
        int year_hal = toStandardReckoning(year, reckoning);
        int year_basis = year_hal % 28;
        int month_basis = (static_cast<int>(month)-1)*2;
        int dow_basis = year_basis + month_basis + day;
        return dow_basis;
    }

    int Calendar::dayOfWeek(const int day_basis)
    {
        int dow = (day_basis+1) % 7 + 1;
        return dow;
    }

    int Calendar::dayOfWeek(const int day, const Month month, const int year, const Reckoning reckoning)
    {
        int dow_basis = dayBasis(day, month, year, reckoning);
        return dayOfWeek(dow_basis);
    }

    int Calendar::firstWeekdayInMonth(const int weekday, const Month month, const int year, const Reckoning reckoning)
    {
        int weekday_1st = dayOfWeek(1, month, year, reckoning);
        int day = 1 + weekday - weekday_1st;
        if (day <= 0)
            day += 7;
        if (day >= daysInMonth(month)) // If the month does not contain such a weekday. Can happen in the nameless days, as that month has less days than a week.
            return 0;
        else
            return day;
    }

    int Calendar::lastWeekdayInMonth(const int weekday, const Month month, const int year, const Reckoning reckoning)
    {
        int  weekday_last = dayOfWeek(daysInMonth(month), month, year, reckoning);
        int day_offset = weekday - weekday_last;
        if (day_offset > 0)
            day_offset -= 7;
        if (-day_offset > daysInMonth(month)) // If the month does not contain such a weekday. Can happen in the nameless days, as that month has less days than a week.
            return 0;
        else
            return daysInMonth(month) + day_offset;
    }

    int Calendar::dayOfYear(const int day, const Month month)
    {
        return (static_cast<int>(month)-1)*daysInMonth(MONTH_PRAIOS) + day;
    }

    int Calendar::fromStandardReckoning(const int year_hal, const Reckoning reckoning)
    {
        int year;
        switch(reckoning) {
            case RECKONING_HAL:
                year = year_hal;
                break;
            case RECKONING_FALL_OF_BOSPARAN:
                year = year_hal + 993;
                break;
            default:
                assert(false);
        }
        return year;
    }

    int Calendar::toStandardReckoning(const int year, const Reckoning reckoning)
    {
        int year_hal;
        switch(reckoning) {
            case RECKONING_HAL:
                year_hal = year;
                break;
            case RECKONING_FALL_OF_BOSPARAN:
                year_hal = year - 993;
                break;
            default:
                assert(false);
        }
        return year_hal;
    }

    int Calendar::convertReckoning(const int year, const Reckoning old_reckoning, const Reckoning new_reckoning)
    {
        if (old_reckoning == new_reckoning)
            return year;
        int year_hal = toStandardReckoning(year, old_reckoning);
        return fromStandardReckoning(year_hal, new_reckoning);
    }

    int Calendar::daysAfterStandardReckoning(const int day, const Month month, const int year, const Reckoning reckoning)
    {
        int year_hal = Calendar::toStandardReckoning(year, reckoning);
        return Calendar::dayOfYear(day, month) + year_hal*Calendar::daysInYear() - 1;
    }


    /* Implementation of Date class ******************************************/
    Date::Date(const Date& other)
        : m_days_after_hal(other.m_days_after_hal), m_reckoning(other.m_reckoning)
    {
    }

    Date::Date(const int new_day, const Month new_month, const int new_year, const Reckoning new_reckoning)
    {
        setDate(new_day, new_month, new_year, new_reckoning);
    }

    Date::Date(const int date_num)
        : m_days_after_hal(date_num), m_reckoning(RECKONING_HAL)
    {
    }

    void Date::setDate(const int new_day, const Month new_month, const int new_year, const Reckoning new_reckoning)
    {
        m_days_after_hal = Calendar::daysAfterStandardReckoning(new_day, new_month, new_year, new_reckoning);
        return;
    }

    void Date::date(int *the_day, Month *the_month, int *the_year, Reckoning *the_reckoning)
    {
        int year_hal, day_of_year;
        if (m_days_after_hal >= 0) {
            year_hal = m_days_after_hal / Calendar::daysInYear();
            day_of_year = m_days_after_hal % Calendar::daysInYear(); // zero-based day of year
        }
        else {
            year_hal = (m_days_after_hal+1) / Calendar::daysInYear() - 1;
            day_of_year = m_days_after_hal - year_hal*Calendar::daysInYear(); // zero-based day of year
        }
        int month_no = day_of_year/Calendar::daysInMonth(MONTH_PRAIOS) + 1;
        *the_day = day_of_year - (month_no-1)*Calendar::daysInMonth(MONTH_PRAIOS) + 1;
        *the_month = static_cast<Month>(month_no);
        *the_reckoning = m_reckoning;
        *the_year = Calendar::fromStandardReckoning(year_hal, m_reckoning);
        return;
    }

    Date Date::operator++()
    {
        m_days_after_hal++;
        return Date(*this);
    }

    Date Date::operator--()
    {
        m_days_after_hal--;
        return Date(*this);
    }

    Date Date::operator+(const Date& other)
    {
        Date date_sum(other);
        date_sum.m_days_after_hal += m_days_after_hal;
        return date_sum;
    }

    Date Date::operator-(const Date &other)
    {
        Date date_diff(m_days_after_hal);
        date_diff.m_days_after_hal -= other.m_days_after_hal;
        return date_diff;
    }

    bool Date::operator<(const Date &other) const
    {
        return m_days_after_hal < other.m_days_after_hal;
    }

    bool Date::operator>(const Date &other) const
    {
        return m_days_after_hal > other.m_days_after_hal;
    }

    bool Date::operator<=(const Date &other) const
    {
        return m_days_after_hal <= other.m_days_after_hal;
    }

    bool Date::operator>=(const Date &other) const
    {
        return m_days_after_hal >= other.m_days_after_hal;
    }

    bool Date::operator==(const Date& other) const
    {
        return m_days_after_hal == other.m_days_after_hal;
    }

    QString Date::toString()
    {
        int the_day;
        Month the_month;
        int the_year;
        Reckoning the_reckoning;
        date(&the_day, &the_month, &the_year, &the_reckoning);
        return QObject::tr("%1 %2 %3 %4").arg(QString::number(the_day)).arg(Calendar::monthName(the_month)).arg(QString::number(the_year)).arg(Calendar::reckoningName(the_reckoning));
    }


    /* Implementation of MoonPhase class *************************************/
    MoonPhase::MoonPhase(const int day_basis)
    {
        setMoonPhase(day_basis);
    }

    MoonPhase::MoonPhase(const int day, const Month month, const int year, const Reckoning reckoning)
    {
        setMoonPhase(day, month, year, reckoning);
    }

    void MoonPhase::setMoonPhase(const int day_basis)
    {
        int moon_phase_base = (day_basis-1) % 28;
        int status = moon_phase_base / 14;
        m_status = (status == 0) ? MoonPhaseStatus::WAXING : MoonPhaseStatus::WANING;
        m_phase = moon_phase_base - 14*status;
        return;
    }

    void MoonPhase::setMoonPhase(const int day, const Month month, const int year, const Reckoning reckoning)
    {
        int day_basis = Calendar::dayBasis(day, month, year, reckoning);
        setMoonPhase(day_basis);
        return;
    }

    QString MoonPhase::toString() const
    {
        const QString status_texts[2] = {QObject::tr("waxing"), QObject::tr("waning")};
        const QString full_texts[2] = {QObject::tr("full"), QObject::tr("new")};
        if (m_phase == 13)
            return full_texts[m_status];
        else
            return status_texts[m_status];
    }

    QString MoonPhase::graphicsFilename() const
    {
        QString status;
        if (m_status == MoonPhaseStatus::WAXING)
            status = "Waxing";
        else
            status = "Waning";
        return QString("MoonPhase%1%2.svg").arg(status).arg(QString::number(m_phase+1));
    }
}

#endif // RPG_SYSTEM_TDE_AVENTURIA
