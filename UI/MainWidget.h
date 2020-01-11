#pragma once

#include <QWidget>

#include "TurnOrderWidget.h"
#include "NPCGeneratorWidget.h"

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

private:
    Ui::MainWidget *m_ui;
    TurnOrderWidget* m_turn_order_widget;
    NPCGeneratorWidget* m_npc_generator_widget;
};
