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

#include "../Backend/TDECalendar.h"
#include <QWidget>
#include <QTableWidgetItem>


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
    int yearStandardReckoning() const;

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

private:
    /**
     * @brief Setup the user interface.
     * This private function is called by the constructor(s).
     */
    void setupUi();

    /**
     * @brief Private function to set the day in the spin box.
     * @param new_day The new day.
     */
    void setDaySpin(const int new_day);

    /**
     * @brief Private function to set a new day in the calendar table.
     * @param new_day The new day.
     */
    void setDayTab(const int new_day);

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
     * @brief Callback activated when the current day is changed via the spin box.
     */
    void onChangeDaySpin(const int new_day);

    /**
     * @brief Callback activated when the current day is changed via the calendar table.
     */
    void onChangeDayTab();

    /**
     * @brief General activities done when the date is changed.
     */
    void onChangeDate();

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
};
