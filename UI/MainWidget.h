/**
 * Main window for the Role Playing Game Assistant.
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

private:
    Ui::MainWidget *m_ui;
    TurnOrderWidget* m_turn_order_widget;
    NPCGeneratorWidget* m_npc_generator_widget;
    CalendarWidget* m_calendar_widget;
};
