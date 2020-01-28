/**
 * Calendar module for Role Playing Game Assistant.
 * This module implements the calendar of the fantasy roleplaying game
 * The Dark Eye (German "Das Scwarze Auge").
 *
 * (c) Andreas Schneider andreas underscore schn at web dot de
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

#include <QWidget>
#include <QTableWidgetItem>

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

namespace Ui {
class CalendarWidget;
}

class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent
     */
    explicit CalendarWidget(QWidget *parent = nullptr);

    /**
     * @brief Destructor.
     */
    ~CalendarWidget();

    /**
     * @brief Get the current day of the month.
     * @return Current day of the month.
     */
    int day() const;

    /**
     * @brief Get the current month.
     * @return Number of the current month (1-13).
     */
    int month() const;

    /**
     * @brief Get the current year in the currently active time reckoning.
     * @return Current year.
     */
    int year() const;

    /**
     * @brief Get the current year in the Hal reckoning.
     * @return Current year in Hal reckoning.
     */
    int yearHal() const;

    /**
     * @brief Set the current day of the month.
     * @param new_day New day of the month.
     */
    void setDay(const int new_day);

    /**
     * @brief Set the current month.
     * @param new_month New month (1-13).
     */
    void setMonth(const int new_month);

    /**
     * @brief Set the current year in the current reckoning.
     * @param new_year New year.
     */
    void setYear(const int new_year);

    /**
     * @brief Get the number of days in a month.
     * @param month_no Month to query (1-13).
     * @return Number of days in the month.
     */
    int daysInMonth(const int month_no) const;

    /**
     * @brief Get the number of months in a year.
     * @return Number of months in a year.
     */
    inline int noOfMonths() const {return 13;}

private:
    /**
     * @brief Setup the user interface.
     * This private function is called by the constructor(s).
     */
    void setupUi();

    /**
     * @brief Computes the day basis according to the Aventurian calendar.
     * This is used to compute days of the week and moon phases.
     * @param day Day of the month (1-30)
     * @param month Month (1-13)
     * @param year_hal Year after Hal.
     * @return Day basis.
     */
    int dayBasis(const int day, const int month, const int year_hal) const;

    /**
     * @brief Computes the day of the week (starting at Windsday) from the day basis.
     * @param day_basis Day basis in the Aventurian calendar.
     * @return Day of the week (1 = Windsday, 7 = Waterday).
     */
    int dayOfWeek(const int day_basis) const;

    /**
     * @brief Computes the day of the week (starting at Windsday) from day, month and year.
     * @param day Day of the month (1-30).
     * @param month Month number (1-13).
     * @param year_hal Year after Hal.
     * @return Day of the week (1 = Windsday, 7 = Waterday).
     */
    int dayOfWeek(const int day, const int month, const int year_hal) const;

    /**
     * @brief Compute moon phase from the day basis.
     * @param day_basis Day basis in the Aventurian calendar.
     * @return
     */
    int moonPhase(const int day_basis) const;

    /**
     * @brief Compute moon phase from day, month and year.
     * @param day Day of the month (1-30)
     * @param month Month (1-13)
     * @param year_hal Year after Hal.
     * @return
     */
    int moonPhase(const int day, const int month, const int year_hal) const;

    /**
     * @brief Fills in days of a month into the weekday table.
     * @param month Month (1-13)
     * @param year_hal Year after Hal.
     */
    void fillMonth(const int month, const int year_hal);

private slots:
    /**
     * @brief Slot activated when the month is changed. It re-fills the calendar view accordingly.
     * @param index The new index selected in the combo box.
     */
    void onChangeMonth(const int index);

    /**
     * @brief Slot activated when the year is changed. It re-fills the calendar view accordingly.
     * @param year The new number selected in the spin box.
     */
    void onChangeYear(const int year);

    /**
     * @brief Callback activated when the current day is changed.
     */
    void onChangeDay();

    /**
     * @brief Goes back one year.
     */
    void onPreviousYear();

    /**
     * @brief Goes to the next year.
     */
    void onNextYear();

    /**
     * @brief Goes back one month.
     */
    void onPreviousMonth();

    /**
     * @brief Goes to the next month.
     */
    void onNextMonth();

    /**
     * @brief Goes to the previous day.
     */
    void onPreviousDay();

    /**
     * @brief Goes to the next day.
     */
    void onNextDay();

private:
    Ui::CalendarWidget *m_ui;
    QString m_moon_phase_texts[4] = {tr("waxing"), tr("full"), tr("waning"), tr("new")};
};
