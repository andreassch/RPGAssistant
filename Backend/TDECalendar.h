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
         * @brief Computes the day basis according to the Aventurian calendar.
         * This is used to compute days of the week and moon phases.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year_hal Year after Hal.
         * @return Day basis.
         */
        static int dayBasis(const int day, const int month, const int year_hal);

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
         * @param year_hal Year after Hal.
         * @return Day of the week (1 = Windsday, 7 = Waterday).
         */
        static int dayOfWeek(const int day, const int month, const int year_hal);
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
        MoonPhase(const int day, const int month, const int year_hal);

        /**
         * @brief Compute moon phase from the day basis.
         * @param day_basis Day basis in the Aventurian calendar.
         */
        void setMoonPhase(const int day_basis);

        /**
         * @brief Compute moon phase from day, month and year.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year_hal Year in Hal reckoning.
         */
        void setMoonPhase(const int day, const int month, const int year_hal);

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
