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
#include "../Backend/Holiday.h"
#include "../Backend/Diary.h"
#include <QWidget>
#include <QTableWidgetItem>
#include <vector>


namespace Ui {
class CalendarWidget;
}

using namespace TDECalendar;

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
    Month month() const;

    /**
     * @brief Get the current year in the currently active time reckoning.
     * @return Current year.
     */
    int year() const;

    /**
     * @brief Get the current reckoning.
     * @return Current reckoning.
     */
    Reckoning reckoning() const;

    /**
     * @brief Set the current day of the month.
     * @param new_day New day of the month.
     */
    void setDay(const int new_day);

    /**
     * @brief Set the current month.
     * @param new_month New month (1-13).
     */
    void setMonth(const Month new_month);

    /**
     * @brief Set the current year in the current reckoning.
     * @param new_year New year.
     */
    void setYear(const int new_year);

    /**
     * @brief Set the current reckoning.
     * @param new_reckoning New reckoning.
     */
    void setReckoning(const Reckoning new_reckoning);

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
     * @param year Year.
     * @param reckoning Reckoning.
     */
    void fillMonth(const Month month, const int year, const Reckoning reckoning);

    /**
     * @brief Saves the current date in the settings for persistant storage.
     */
    void saveSettings();

    /**
     * @brief Updates the diary object with the contents from the text edit.
     */
    void updateDiary();

    /**
     * @brief Loads a new diary from file.
     * @param filename The filename of the new diary to load.
     */
    void loadDiary(const QString& filename);

    /**
     * @brief Saves the current diary to the filename stored in the diary object.
     */
    void saveDiary();

private slots:
    /**
     * @brief Slot activated when the reckoning is changed.
     * It recomputes the year in the new reckoning so that the overall date stays the same.
     * @param index
     */
    void onChangeReckoning(const int index);

    /**
     * @brief Slot activated when the year is changed. It re-fills the calendar view accordingly.
     * @param year The new number selected in the spin box.
     */
    void onChangeYear(const int year);

    /**
     * @brief Slot activated when the month is changed. It re-fills the calendar view accordingly.
     * @param index The new index selected in the combo box.
     */
    void onChangeMonth(const int index);

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

    /**
     * @brief Starts a new diary.
     */
    void onNewDiary();

    /**
      * @brief Closes the current diary.
     */
    void onCloseDiary();

private:
    Ui::CalendarWidget *m_ui;
    Date m_date;
    Reckoning m_reckoning = static_cast<Reckoning>(0);
    std::vector<Holiday> m_holidays;
    Diary m_diary;
};
