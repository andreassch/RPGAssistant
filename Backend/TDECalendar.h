#pragma once

#include <QString>
#include <QObject>

namespace TDECalendar {

    /**
     * Enumeration of the different months in the year of the Aventurian calendar.
     */
    namespace Month {
        enum Month {
            PRAIOS = 1,
            RONDRA = 2,
            EFFERD = 3,
            TRAVIA = 4,
            BORON = 5,
            HESINDE = 6,
            FIRUN = 7,
            TSA = 8,
            PHEX = 9,
            PERAINE = 10,
            INGERIMM = 11,
            RAHJA = 12,
            NAMELESS = 13
        };
    }

    namespace Reckoning {
        enum Reckoning {HAL,FALL_OF_BOSPARAN};
        const int RECKONING_COUNT = 2;
    }

    /**
     * @brief Calendar computations of the Aventurian calendar.
     */
    class Calendar
    {
    public:
        Calendar();

        /**
         * @brief Get the number of days in a month.
         * @param month_no Month to query (1-13).
         * @return Number of days in the month.
         */
        static int daysInMonth(const int month);

        /**
         * @brief Get the number of months in a year.
         * @return Number of months in a year.
         */
        inline static int noOfMonths() { return 13; }

        /**
         * @brief Gets the number of days in a week.
         * @return Number of days in a week.
         */
        static int daysInWeek() { return 7; }

        /**
         * @brief Get name of a given month.
         * @param month Number of the month (1-13).
         * @return Name of the given month.
         */
        static QString monthName(const int month);

        /**
         * @brief Get name of a day in the week.
         * @param weekday Number of the day in the week.
         * @return Name of the day in the week.
         */
        static QString weekdayName(const int weekday);

        /**
         * @brief Get abbreviation of a day in the week.
         * @param weekday Number of the day in the week.
         * @return Abbreviation of the day in the week.
         */
        static QString weekdayAbbreviation(const int weekday);

        /**
         * @brief Get name of a reckoning.
         * @param reckoning Reckoning enum entry.
         * @return Name of the reckoning.
         */
        static QString reckoningName(const Reckoning::Reckoning reckoning);

        /**
         * @brief Get the abbreviation of a reckoning.
         * @param reckoning Reckoning enum entry.
         * @return Abbreviation of the reckoning.
         */
        static QString reckoningAbbreviation(const Reckoning::Reckoning reckoning);

        /**
         * @brief Get symbolic name of a reckoning. This is independent of locale.
         * @return Symbolic name of a reckoning
         */
        static QString reckoningSymbolicName(const Reckoning::Reckoning reckoning);

        /**
         * @brief Obtain a reckoning from a string.
         * @param reckoning_name String describing a reckoning (one of the reckoning symbolic names).
         * @return The reckoning as Reckoning enum entry.
         */
        static Reckoning::Reckoning parseReckoning(const QString& reckoning_name);

        /**
         * @brief Get number of different reckonings supported by the calendar.
         * This should coincide with the number of entries in the Reckoning enum.
         * @return Number of reckonings.
         */
        inline static int noOfReckonings() { return Reckoning::RECKONING_COUNT; }

        /**
         * @brief Computes the day basis according to the Aventurian calendar.
         * This is used to compute days of the week and moon phases.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Day basis.
         */
        static int dayBasis(const int day, const int month, const int year, const Reckoning::Reckoning reckoning);

        /**
         * @brief Computes the day of the week (starting at Windsday) from the day basis.
         * @param day_basis Day basis in the Aventurian calendar.
         * @return Day of the week (1 = Windsday, 7 = Waterday).
         */
        static int dayOfWeek(const int day_basis);

        /**
         * @brief Computes the day of the week (starting at Windsday) from day, month and year.
         * @param day Day of the month (1-30).
         * @param month Month number (1-13).
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Day of the week (1 = Windsday, 7 = Waterday).
         */
        static int dayOfWeek(const int day, const int month, const int year, const Reckoning::Reckoning reckoning);

        /**
         * @brief Convert year from standard reckoning (Hal) to given reckoning.
         * @param year Year in the standard reckoning (Hal).
         * @param reckoning Reckoning to convert to.
         * @return Year in the given reckoning.
         */
        static int fromStandardReckoning(const int year_hal, const Reckoning::Reckoning reckoning);

        /**
         * @brief Convert year from given reckoning to standard reckoning (Hal).
         * @param year Year in the given reckoning.
         * @param reckoning Reckoning the year is given in.
         * @return Year in standard reckoning (Hal).
         */
        static int toStandardReckoning(const int year, const Reckoning::Reckoning reckoning);

        /**
         * @brief Convert year from one reckoning to another.
         * @param year A year in the old reckoning.
         * @param old_reckoning The reckoning to convert from.
         * @param new_reckoning The reckoning to convert to.
         * @return The year in the new reckoning.
         */
        static int convertReckoning(const int year, const Reckoning::Reckoning old_reckoning, const Reckoning::Reckoning new_reckoning);
    };

    namespace MoonPhaseStatus {
        enum MoonPhaseStatus {WAXING, WANING};
    }

    /**
     * @brief Moon phases in Aventurica.
     */
    class MoonPhase
    {
    public:
        MoonPhase(const int day_basis);
        MoonPhase(const int day, const int month, const int year, const Reckoning::Reckoning reckoning);

        /**
         * @brief Compute moon phase from the day basis.
         * @param day_basis Day basis in the Aventurian calendar.
         */
        void setMoonPhase(const int day_basis);

        /**
         * @brief Compute moon phase from day, month and year.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year Year.
         * @param reckoning Reckoning.
         */
        void setMoonPhase(const int day, const int month, const int year, const Reckoning::Reckoning reckoning);

        /**
         * @brief Textual name of moon phase.
         * @return Text of moon phase.
         */
        QString toString() const;

        /**
         * @brief Gets file name for graphics corresponding to the moon phase.
         * @return File name.
         */
        QString graphicsFilename() const;

    private:
        MoonPhaseStatus::MoonPhaseStatus m_status;
        int m_phase;
    };
}
