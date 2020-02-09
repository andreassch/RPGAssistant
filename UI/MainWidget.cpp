#include "MainWidget.h"
#include "ui_MainWidget.h"
#include "../version.h"

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MainWidget)
{
    setWindowIcon(QIcon(":/icons/Images/rpgassistant.ico"));
    m_ui->setupUi(this);
    SetupUi();
}

MainWidget::~MainWidget()
{
    delete(m_turn_order_widget);
    delete(m_npc_generator_widget);
    delete(m_calendar_widget);
    delete m_ui;
}

void MainWidget::SetupUi()
{
    m_ui->labelVersion->setText(tr("Version ").append(VERSION_STR));
    m_turn_order_widget = new TurnOrderWidget();
    m_npc_generator_widget = new NPCGeneratorWidget();
    m_calendar_widget = new CalendarWidget();
    connect(m_ui->buttonTurnOrderList, &QPushButton::clicked, this, &MainWidget::onTurnOrder);
    connect(m_ui->buttonNPCGenerator, &QPushButton::clicked, this, &MainWidget::onNPCGenerator);
    connect(m_ui->buttonCalendar, &QPushButton::clicked, this, &MainWidget::onCalendar);
}

void MainWidget::onTurnOrder()
{
    m_turn_order_widget->show();
    return;
}

void MainWidget::onNPCGenerator()
{
    m_npc_generator_widget->show();
    return;
}

void MainWidget::onCalendar()
{
    m_calendar_widget->show();
    return;
}
