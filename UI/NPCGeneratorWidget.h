/**
 * User interface of the non-Player Character (NPC) generator module of the Role Playing Game Assistant.
 *
 * Copyright (c) 2020 Andreas Schneider (andreas underscore schn at web dot de)
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

#include "../config.h"

#include <QWidget>
#include "Backend/Names.h"
#include "Backend/Person.h"

namespace Ui {
class NPCGeneratorWidget;
}

class NPCGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent
     */
    explicit NPCGeneratorWidget(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~NPCGeneratorWidget();

private:
    /**
     * @brief Setup the user interface.
     * This private function is called by the constructor.
     */
    void setupUi();

    /**
     * @brief Saves current selections to settings.
     */
    void saveSettings();

    /**
     * @brief Loads selections from settings.
     */
    void loadSettings();

    /**
     * @brief Determine the index in the person list for a given region name.
     * @param region Name of the region to look for.
     * @return Index of the region, or -1 if it is not found.
     */
    int searchRegion(const QString& region_name);

    /**
     * @brief Determine the index in the namelist for a given region name.
     * @param region Name of the region to look for.
     * @return Index of the region, or -1 if it is not found.
     */
    int searchNameRegion(const QString& region_name);

private slots:
    /**
     * @brief Callback for the generate button.
     */
    void onGenerate();

    /**
     * @brief Callback on changing the check box to select regions randomly.
     * It enables/disbales the corresponding combo box.
     * @param state The new state of the check box.
     */
    void onChangeRandomRegion(const int state);

    /**
     * @brief Callback on changing the check box to select the name region separately.
     * @param state The new state of the check box.
     */
    void onChangeCheckSeparateNameRegion(const int state);

    /**
     * @brief Callback on changing the check box to select the gender randomly.
     * It enables/disbales the corresponding combo box.
     * @param state The new state of the check box.
     */
    void onChangeRandomGender(const int state);

    /**
     * @brief Callback on changing the check box to select the age randomly.
     * @param state The new state of the check box.
     */
    void onChangeRandomAge(const int state);

    /**
     * @brief Callback for the clear button, which clears the result text edit.
     */
    void onClear();

private:
    Ui::NPCGeneratorWidget* m_ui;
    std::vector<Names> m_namelists;
    std::vector<Person> m_persons;
};
