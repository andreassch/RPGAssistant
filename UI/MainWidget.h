/**
 * Main window of the Role Playing Game Assistant.
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

#include "TurnOrderWidget.h"
#include "NPCGeneratorWidget.h"
#include "CalendarWidget.h"

namespace Ui {
class MainWidget;
}

class MainWidget : public QWidget
{
    Q_OBJECT

public:
    explicit MainWidget(QWidget *parent = nullptr);
    ~MainWidget();

private:
    void SetupUi();
    void onTurnOrder();
    void onNPCGenerator();
    void onCalendar();

    void onRoll();
    void onRollD6();
    void onRollD20();
    void onRoll3D20();

private:
    void saveDiceSettings();
    void loadDiceSettings();

    Ui::MainWidget *m_ui;
    TurnOrderWidget* m_turn_order_widget;
    NPCGeneratorWidget* m_npc_generator_widget;
    CalendarWidget* m_calendar_widget;
    const int m_dices[6] = {4, 6, 8, 10, 12, 20};
};
