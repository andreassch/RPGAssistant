/**
 * Non-Player Character (NPC) generator module for Role Playing Game Assistant.
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
#include "Backend/Names.h"
#include "Backend/Species.h"

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
     * @brief Loads names and races from XML data files.
     */
    void loadData();

private slots:
    /**
     * @brief Callback for the generate button.
     */
    void onGenerate();

    /**
     * @brief Callback when the species is changed, which updates the races available for the species.
     */
    void onChangeSpecies(const int index);

    /**
     * @brief Callback on changing the check box to select regions randomly.
     * It enables/disbales the corresponding combo box.
     * @param state The new state of the check box.
     */
    void onChangeRandomRegion(const int state);

    /**
     * @brief Callback on changing the check box to select species randomly.
     * It enables/disbales the corresponding combo box and updates the state of random race selection, if necessary.
     * @param state The new state of the check box.
     */
    void onChangeRandomSpecies(const int state);

    /**
     * @brief Callback on changing the check box to select races randomly.
     * It enables/disbales the corresponding combo box and updates the state of random species selection, if necessary.
     * @param state The new state of the check box.
     */
    void onChangeRandomRace(const int state);

    /**
     * @brief Callback on changing the check box to select the gender randomly.
     * It enables/disbales the corresponding combo box.
     * @param state The new state of the check box.
     */
    void onChangeRandomGender(const int state);

private:
    Ui::NPCGeneratorWidget* m_ui;
    std::vector<Names> m_namelists;
    std::vector<Species> m_species;
};
