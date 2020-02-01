#include "TDECalendar.h"

#include <cassert>
#include <QtDebug>

namespace TDECalendar {

    /* Implementation of Calendar class **************************************/
    Calendar::Calendar()
    {
    }

    int Calendar::daysInMonth(const int month)
    {
        if (month == Month::NAMELESS)
            return 5;
        else
            return 30;
    }

    QString Calendar::monthName(const int month)
    {
        const QString m_month_names[13] = {
            QObject::tr("Praios"), QObject::tr("Rondra"), QObject::tr("Efferd"), QObject::tr("Travia"),
            QObject::tr("Boron"), QObject::tr("Hesinde"), QObject::tr("Firun"), QObject::tr("Tsa"),
            QObject::tr("Phex"), QObject::tr("Peraine"), QObject::tr("Ingerimm"), QObject::tr("Rahja"),
            QObject::tr("Nameless") };
        assert((month >= 1) && (month <= noOfMonths()));
        return m_month_names[month-1];
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

    int Calendar::dayBasis(const int day, const int month, const int year_hal)
    {
        int year_basis = year_hal % 28;
        int month_basis = (month-1)*2;
        int dow_basis = year_basis + month_basis + day;
        return dow_basis;
    }

    int Calendar::dayOfWeek(const int day_basis)
    {
        int dow = (day_basis+1) % 7 + 1;
        return dow;
    }

    int Calendar::dayOfWeek(const int day, const int month, const int year_hal)
    {
        int dow_basis = dayBasis(day, month, year_hal);
        return dayOfWeek(dow_basis);
    }


    /* Implementation of MoonPhase class *************************************/
    MoonPhase::MoonPhase(const int day_basis)
    {
        setMoonPhase(day_basis);
    }

    MoonPhase::MoonPhase(const int day, const int month, const int year_hal)
    {
        setMoonPhase(day, month, year_hal);
    }

    void MoonPhase::setMoonPhase(const int day_basis)
    {
        int moon_phase_base = (day_basis-1) % 28;
        int status = moon_phase_base / 14;
        m_status = (status == 0) ? MoonPhaseStatus::WAXING : MoonPhaseStatus::WANING;
        m_phase = moon_phase_base - 14*status;
        qDebug() << "setMoonPhase" << status << m_phase;
        return;
    }

    void MoonPhase::setMoonPhase(const int day, const int month, const int year_hal)
    {
        int day_basis = Calendar::dayBasis(day, month, year_hal);
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
