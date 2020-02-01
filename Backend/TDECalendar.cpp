#include "TDECalendar.h"

#include <cassert>
#include <QtDebug>

namespace TDECalendar {

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

    QString Calendar::monthName(int month)
    {
        const QString m_month_names[13] = {
            QObject::tr("Praios"), QObject::tr("Rondra"), QObject::tr("Efferd"), QObject::tr("Travia"),
            QObject::tr("Boron"), QObject::tr("Hesinde"), QObject::tr("Firun"), QObject::tr("Tsa"),
            QObject::tr("Phex"), QObject::tr("Peraine"), QObject::tr("Ingerimm"), QObject::tr("Rahja"),
            QObject::tr("Nameless") };
        assert((month >= 1) && (month <= noOfMonths()));
        return m_month_names[month-1];
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

    int Calendar::moonPhase(const int day_basis)
    {
        int moon_phase = ((day_basis % 28)-1)/7;
        return moon_phase;
    }

    int Calendar::moonPhase(const int day, const int month, const int year_hal)
    {
        int day_basis = dayBasis(day, month, year_hal);
        return moonPhase(day_basis);
    }

    QString Calendar::moonPhaseText(const int moon_phase)
    {
        qDebug() << "moon phase" << moon_phase;
        QString moon_phase_texts[4] = {QObject::tr("waxing"), QObject::tr("full"), QObject::tr("waning"), QObject::tr("new")};
        assert((moon_phase >= 0) && (moon_phase < 4));
        return moon_phase_texts[moon_phase];
    }
}
