#pragma once

#include <QString>
#include <QObject>

namespace TDECalendar {

    /**
     * Enumeration of the different months in the year of the Aventurian calendar.
     */
    enum Month {
        MONTH_PRAIOS = 1,
        MONTH_RONDRA = 2,
        MONTH_EFFERD = 3,
        MONTH_TRAVIA = 4,
        MONTH_BORON = 5,
        MONTH_HESINDE = 6,
        MONTH_FIRUN = 7,
        MONTH_TSA = 8,
        MONTH_PHEX = 9,
        MONTH_PERAINE = 10,
        MONTH_INGERIMM = 11,
        MONTH_RAHJA = 12,
        MONTH_NAMELESS = 13
    };

    enum Reckoning {RECKONING_HAL,RECKONING_FALL_OF_BOSPARAN};
    const int RECKONING_COUNT = 2;

    /*************************************************************************/
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
        static int daysInMonth(const Month month);

        /**
         * @brief Get the number of months in a year.
         * @return Number of months in a year.
         */
        inline static int noOfMonths() { return 13; }

        /**
         * @brief Get the number of days in a year.
         * @return  Number of days in a year.
         */
        inline static int daysInYear() { return 365; }

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
        static QString monthName(const Month month);

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
        static QString reckoningName(const Reckoning reckoning);

        /**
         * @brief Get the abbreviation of a reckoning.
         * @param reckoning Reckoning enum entry.
         * @return Abbreviation of the reckoning.
         */
        static QString reckoningAbbreviation(const Reckoning reckoning);

        /**
         * @brief Get symbolic name of a reckoning. This is independent of locale.
         * @return Symbolic name of a reckoning
         */
        static QString reckoningSymbolicName(const Reckoning reckoning);

        /**
         * @brief Obtain a reckoning from a string.
         * @param reckoning_name String describing a reckoning (one of the reckoning symbolic names).
         * @return The reckoning as Reckoning enum entry.
         */
        static Reckoning parseReckoning(const QString& reckoning_name);

        /**
         * @brief Get number of different reckonings supported by the calendar.
         * This should coincide with the number of entries in the Reckoning enum.
         * @return Number of reckonings.
         */
        inline static int noOfReckonings() { return RECKONING_COUNT; }

        /**
         * @brief Computes the day basis according to the Aventurian calendar.
         * This is used to compute days of the week and moon phases.
         * @param day Day of the month (1-30)
         * @param month Month (1-13)
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Day basis.
         */
        static int dayBasis(const int day, const Month month, const int year, const Reckoning reckoning);

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
        static int dayOfWeek(const int day, const Month month, const int year, const Reckoning reckoning);

        /**
         * @brief Computes the day in the month of the first weekday (e.g. Praiosday) in a given month.
         * @param weekday Day of the week (1 = Windsday, 7 = Waterday).
         * @param month Month number (1-13).
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Day in the month.
         */
        static int firstWeekdayInMonth(const int weekday, const Month month, const int year, const Reckoning reckoning);

        /**
         * @brief Computes the day in the month of the last weekday (e.g. Praiosday) in a given month.
         * @param weekday Day of the week (1 = Windsday, 7 = Waterday).
         * @param month Month number (1-13).
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Day in the month.
         */
        static int lastWeekdayInMonth(const int weekday, const Month month, const int year, const Reckoning reckoning);

        /**
         * @brief Computes the day in the year (1-365) from month and day in the month.
         * @param day Day in the month.
         * @param month Month.
         * @return Day of the year.
         */
        static int dayOfYear(const int day, const Month month);

        /**
         * @brief Convert year from standard reckoning (Hal) to given reckoning.
         * @param year Year in the standard reckoning (Hal).
         * @param reckoning Reckoning to convert to.
         * @return Year in the given reckoning.
         */
        static int fromStandardReckoning(const int year_hal, const Reckoning reckoning);

        /**
         * @brief Convert year from given reckoning to standard reckoning (Hal).
         * @param year Year in the given reckoning.
         * @param reckoning Reckoning the year is given in.
         * @return Year in standard reckoning (Hal).
         */
        static int toStandardReckoning(const int year, const Reckoning reckoning);

        /**
         * @brief Convert year from one reckoning to another.
         * @param year A year in the old reckoning.
         * @param old_reckoning The reckoning to convert from.
         * @param new_reckoning The reckoning to convert to.
         * @return The year in the new reckoning.
         */
        static int convertReckoning(const int year, const Reckoning old_reckoning, const Reckoning new_reckoning);

        /**
         * @brief Computes the number of days since the beginning of the standard reckoning (i.e. Hal) for a given date.
         * @param day Day.
         * @param month Month.
         * @param year Year.
         * @param reckoning Reckoning.
         * @return Days after Hal corresponding to the given date.
         */
        static int daysAfterStandardReckoning(const int day, const Month month, const int year, const Reckoning reckoning);
    };


    /*************************************************************************/
    class Date
    {
    public:
        Date(const Date& other);
        Date(const int new_day, const Month new_month, const int new_year, const Reckoning new_reckoning);
        Date(const int date_num = 0);

        void setDate(const int new_day, const Month new_month, const int new_year, const Reckoning new_reckoning);
        void date(int* the_day, Month* the_month, int* the_year, Reckoning* the_reckoning);
        inline void setDateNum(const int date_num) { m_days_after_hal = date_num; }
        inline int dateNum() const { return m_days_after_hal; }
        inline void setReckoning(const Reckoning new_reckoning) { m_reckoning = new_reckoning; }
        inline Reckoning reckoning() const { return m_reckoning; }

        Date operator++();
        Date operator--();
        Date operator+(const Date& other);
        Date operator-(const Date& other);
        bool operator>(const Date& other) const;
        bool operator<(const Date& other) const;
        bool operator>=(const Date& other) const;
        bool operator<=(const Date& other) const;
        bool operator==(const Date& other) const;

        QString toString();

    protected:
        int m_days_after_hal;
        Reckoning m_reckoning = RECKONING_HAL;
    };


    /*************************************************************************/
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
        MoonPhase(const int day, const Month month, const int year, const Reckoning reckoning);

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
        void setMoonPhase(const int day, const Month month, const int year, const Reckoning reckoning);

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
