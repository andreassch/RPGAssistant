#include "CalendarWidget.h"
#include "ui_CalendarWidget.h"

#include "Utils.h"
#include <QtDebug>

/* Public constructors/desctructors ******************************************/
CalendarWidget::CalendarWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::CalendarWidget)
{
    m_ui->setupUi(this);
    setupUi();
}

CalendarWidget::~CalendarWidget()
{
    delete m_ui;
}

/* Public methonds ***********************************************************/
int CalendarWidget::day() const
{
    return m_ui->tableCalendar->currentItem()->data(Qt::DisplayRole).toInt();
}

int CalendarWidget::month() const
{
    return m_ui->comboMonth->currentIndex()+1;
}

int CalendarWidget::year() const
{
    return m_ui->spinYear->value();
}

int CalendarWidget::yearHal() const
{
    return year();
}

int CalendarWidget::daysInMonth(const int month) const
{
    if (month == 13)
        return 5;
    else
        return 30;
}

/* Private methods ***********************************************************/
void CalendarWidget::setupUi()
{
    m_ui->tableCalendar->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_ui->comboMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWidget::onChangeMonth);
    connect(m_ui->spinYear, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeYear);
    connect(m_ui->tableCalendar, &QTableWidget::itemSelectionChanged, this, &CalendarWidget::onChangeDay);
    fillMonth(1, 1);
    return;
}

int CalendarWidget::dayBasis(const int day, const int month, const int year_hal)
{
    int year_basis = year_hal % 28;
    int month_basis = (month-1)*2;
    int dow_basis = year_basis + month_basis + day;
    return dow_basis;
}

int CalendarWidget::dayOfWeek(const int day_basis)
{
    int dow = (day_basis+2) % 7;
    return dow;
}

int CalendarWidget::moonPhase(const int day_basis)
{
    int moon_phase = ((day_basis % 28)-1)/7;
    return moon_phase;
}

int CalendarWidget::dayOfWeek(const int day, const int month, const int year_hal)
{
    int dow_basis = dayBasis(day, month, year_hal);
    return dayOfWeek(dow_basis);
}

int CalendarWidget::moonPhase(const int day, const int month, const int year_halt)
{
    int day_basis = dayBasis(day, month, year_halt);
    return moonPhase(day_basis);
}

void CalendarWidget::fillMonth(const int month, const int year_hal)
{
    int day = 1;
    int col = dayOfWeek(day, month, year_hal)-1;
    if (col < 0)
        col += 7;
    qDebug() << "fillMonth" << year_hal << month << col << daysInMonth(month);
    int row = 0;
    // Clear items before start.
    for (auto col_del=0; col_del<col;col_del++) {
        if (m_ui->tableCalendar->item(row, col_del))
            delete(m_ui->tableCalendar->takeItem(row, col_del));
    }
    // Fill day numbers into table.
    for (; day <= daysInMonth(month); day++) {
        QTableWidgetItem* item = m_ui->tableCalendar->item(row, col);
        if (item)
            item->setData(Qt::DisplayRole,QVariant(day));
        else
            m_ui->tableCalendar->setItem(row, col, UiUtils::createTableWidgetNumericItem<int>(day, Qt::ItemIsSelectable | Qt::ItemIsEnabled));
        col++;
        if (col == m_ui->tableCalendar->columnCount()) {
            col = 0;
            row++;
        }
    }
    // Delete any additional items that may still be present.
    for (; row < m_ui->tableCalendar->rowCount(); row++) {
        for (; col < m_ui->tableCalendar->columnCount(); col++) {
            QTableWidgetItem* item = m_ui->tableCalendar->item(row, col);
            if (item)
                delete(item);
        }
        col = 0;
    }
}

/* Private slots *************************************************************/
void CalendarWidget::onChangeMonth(const int index)
{
    fillMonth(index+1, yearHal());
    onChangeDay();
    return;
}

void CalendarWidget::onChangeYear(const int year)
{
    fillMonth(month(), year);
    onChangeDay();
    return;
}

void CalendarWidget::onChangeDay()
{
    int row = m_ui->tableCalendar->currentRow();
    int column = m_ui->tableCalendar->currentColumn();
    QTableWidgetItem* item = m_ui->tableCalendar->item(row, column);
    if (item) {
        int day = item->data(Qt::DisplayRole).toInt();
        int moon_phase = moonPhase(day, month(), year());
        m_ui->labelMoonPhaseText->setText(m_moon_phase_texts[moon_phase]);
    }
    else {
        m_ui->labelMoonPhaseText->setText("");
    }
    return;
}
