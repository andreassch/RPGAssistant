#pragma once

#include <QString>
#include <QObject>

namespace TDECalendar {

    /**
     * Enumeration of the different months in the year.
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
         * @brief Get name of a given month.
         * @param month Number of the month (1-13).
         * @return Name of the given month.
         */
        static QString monthName(int month);

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

        /**
         * @brief Compute moon phase from the day basis.
         * @param day_basis Day basis in the Aventurian calendar.
         * @return Moon phase
         */
        static int moonPhase(const int day_basis);

        /**
         * @brief Compute moon phase from day, month and year.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year_hal Year in Hal reckoning.
         * @return Moon phase
         */
        static int moonPhase(const int day, const int month, const int year_hal);

        /**
         * @brief Textual name of moon phase.
         * @param moon_phase Moon phase.
         * @return Text of moon phase.
         */
        static QString moonPhaseText(const int moon_phase);
    };
}
