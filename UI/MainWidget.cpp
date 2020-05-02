#include "MainWidget.h"
#include "ui_MainWidget.h"

#include <QSettings>

MainWidget::MainWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::MainWidget)
{
    setWindowIcon(QIcon(":/Graphics/rpgassistant.ico"));
    m_ui->setupUi(this);
    SetupUi();
}

MainWidget::~MainWidget()
{
    saveDiceSettings();
    delete(m_turn_order_widget);
    delete(m_npc_generator_widget);
    delete(m_calendar_widget);
    delete m_ui;
}

void MainWidget::SetupUi()
{
    // Set up version information.
    m_ui->labelVersion->setText(tr("Version ").append(VERSION " build " __DATE__));

    // Set up tools.
    m_turn_order_widget = new TurnOrderWidget();
    m_npc_generator_widget = new NPCGeneratorWidget();
    m_calendar_widget = new CalendarWidget();
    connect(m_ui->buttonTurnOrderList, &QPushButton::clicked, this, &MainWidget::onTurnOrder);
    connect(m_ui->buttonNPCGenerator, &QPushButton::clicked, this, &MainWidget::onNPCGenerator);
    connect(m_ui->buttonCalendar, &QPushButton::clicked, this, &MainWidget::onCalendar);

    // Set up dices.
    for (size_t ind = 0; ind < sizeof(m_dices)/sizeof(*m_dices); ind++)
        m_ui->comboDice->addItem(tr("D%1").arg(m_dices[ind]));
    loadDiceSettings();
    connect(m_ui->buttonD6, &QPushButton::clicked, this, &MainWidget::onRollD6);
    connect(m_ui->buttonD20, &QPushButton::clicked, this, &MainWidget::onRollD20);
    connect(m_ui->button3D20, &QPushButton::clicked, this, &MainWidget::onRoll3D20);
    connect(m_ui->buttonRoll, &QPushButton::clicked, this, &MainWidget::onRoll);
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

void MainWidget::onRoll()
{
    int dice_num = m_ui->spinDiceNumber->value();
    Dices dice(m_dices[m_ui->comboDice->currentIndex()], dice_num);
    const int constant = m_ui->spinConstant->value();
    QString roll_text;
    int sum = dice.roll(&roll_text) + constant;
    if (constant != 0)
        roll_text.append(QString(" + %1").arg(constant));
    if ((dice_num > 1) || (constant != 0))
        roll_text.append(QString(" = %2").arg(sum));
    m_ui->textDiceResult->append(roll_text);
#ifdef ANDROID
    saveDiceSettings();
#endif
    return;
}

void MainWidget::onRollD6()
{
    Dices dice(6);
    QString roll_text;
    dice.roll(&roll_text);
    m_ui->textDiceResult->append(roll_text);
    return;
}

void MainWidget::onRollD20()
{
    Dices dice(20);
    QString roll_text;
    dice.roll(&roll_text);
    m_ui->textDiceResult->append(roll_text);
    return;
}

void MainWidget::onRoll3D20()
{
    Dices dice(20, 3);
    QString roll_text;
    int sum = dice.roll(&roll_text);
    roll_text.append(QString(" = %1").arg(QString::number(sum)));
    m_ui->textDiceResult->append(roll_text);
    return;
}

void MainWidget::loadDiceSettings()
{
    QSettings settings;
    int dice_count = settings.value("Dices/count", QVariant(1)).toInt();
    int dice_faces = settings.value("Dices/faces", QVariant(6)).toInt();
    int offset = settings.value("Dices/offset", QVariant(0)).toInt();
    if (dice_count >= 1)
        m_ui->spinDiceNumber->setValue(dice_count);
    for (size_t ind = 0; ind < sizeof(m_dices)/sizeof(*m_dices); ind++) {
        if (dice_faces == m_dices[ind])
            m_ui->comboDice->setCurrentIndex(ind);
    }
    m_ui->spinConstant->setValue(offset);
    return;
}

void MainWidget::saveDiceSettings()
{
    QSettings settings;
    settings.setValue("Dices/count", m_ui->spinDiceNumber->value());
    settings.setValue("Dices/faces", m_dices[m_ui->comboDice->currentIndex()]);
    settings.setValue("Dices/offset", m_ui->spinConstant->value());
    return;
}
