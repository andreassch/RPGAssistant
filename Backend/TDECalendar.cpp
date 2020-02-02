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

    QString Calendar::reckoningName(const Reckoning::Reckoning reckoning)
    {
        const QString reckoning_names[Reckoning::RECKONING_COUNT] = {QObject::tr("Hal"), QObject::tr("Fall of Bosparan")};
        return reckoning_names[reckoning];
    }

    QString Calendar::reckoningAbbreviation(const Reckoning::Reckoning reckoning)
    {
        const QString reckoning_abbreviations[Reckoning::RECKONING_COUNT] = {QObject::tr("Hal"), QObject::tr("BoF")};
        return reckoning_abbreviations[reckoning];
    }

    QString Calendar::reckoningSymbolicName(const Reckoning::Reckoning reckoning)
    {
        const char* reckoning_symbolic_names[Reckoning::RECKONING_COUNT] = {"Hal", "BF"};
        return reckoning_symbolic_names[reckoning];
    }

    Reckoning::Reckoning Calendar::parseReckoning(const QString &reckoning_symbolic_name)
    {
        for (int reckoning_no = 0; reckoning_no < noOfReckonings(); reckoning_no++) {
            if (QString::compare(reckoning_symbolic_name, reckoningSymbolicName(static_cast<Reckoning::Reckoning>(reckoning_no)), Qt::CaseInsensitive) == 0)
                return static_cast<Reckoning::Reckoning>(reckoning_no);
        }
        qDebug() << "Unknown reckoning" << reckoning_symbolic_name << "reverting to Hal";
        return Reckoning::HAL;
    }

    int Calendar::dayBasis(const int day, const int month, const int year, const Reckoning::Reckoning reckoning)
    {
        int year_hal = toStandardReckoning(year, reckoning);
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

    int Calendar::dayOfWeek(const int day, const int month, const int year, const Reckoning::Reckoning reckoning)
    {
        int dow_basis = dayBasis(day, month, year, reckoning);
        return dayOfWeek(dow_basis);
    }

    int Calendar::fromStandardReckoning(const int year_hal, const Reckoning::Reckoning reckoning)
    {
        int year;
        switch(reckoning) {
            case Reckoning::HAL:
                year = year_hal;
                break;
            case Reckoning::FALL_OF_BOSPARAN:
                year = year_hal + 993;
                break;
        }
        return year;
    }

    int Calendar::toStandardReckoning(const int year, const Reckoning::Reckoning reckoning)
    {
        int year_hal;
        switch(reckoning) {
            case Reckoning::HAL:
                year_hal = year;
                break;
            case Reckoning::FALL_OF_BOSPARAN:
                year_hal = year - 993;
                break;
        }
        return year_hal;
    }

    int Calendar::convertReckoning(const int year, const Reckoning::Reckoning old_reckoning, const Reckoning::Reckoning new_reckoning)
    {
        if (old_reckoning == new_reckoning)
            return year;
        int year_hal = toStandardReckoning(year, old_reckoning);
        return fromStandardReckoning(year_hal, new_reckoning);
    }


    /* Implementation of MoonPhase class *************************************/
    MoonPhase::MoonPhase(const int day_basis)
    {
        setMoonPhase(day_basis);
    }

    MoonPhase::MoonPhase(const int day, const int month, const int year, const Reckoning::Reckoning reckoning)
    {
        setMoonPhase(day, month, year, reckoning);
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

    void MoonPhase::setMoonPhase(const int day, const int month, const int year, const Reckoning::Reckoning reckoning)
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
