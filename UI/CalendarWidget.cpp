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
    QItemSelectionModel *select = m_ui->tableCalendar->selectionModel();
    if (select->hasSelection()) {
        int row = select->selectedIndexes()[0].row();
        int col = select->selectedIndexes()[0].column();
        QTableWidgetItem* item = m_ui->tableCalendar->item(row, col);
        if (item)
            return item->data(Qt::DisplayRole).toInt();
        else
            return (row == 0) ? -1 : -2;
    }
    else
        return 0;
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

void CalendarWidget::setDay(const int new_day)
{
    int col = dayOfWeek(new_day, month(), yearHal()) - 1;
    int row = (dayOfWeek(1, month(), yearHal()) + new_day - 2) / 7;
    QItemSelectionModel *selection_model = m_ui->tableCalendar->selectionModel();
    QModelIndex index = m_ui->tableCalendar->model()->index(row, col);
    selection_model->select(QItemSelection(index, index), QItemSelectionModel::ClearAndSelect);
    return;
}

void CalendarWidget::setMonth(const int new_month)
{
    m_ui->comboMonth->setCurrentIndex(new_month-1);
    return;
}

void CalendarWidget::setYear(const int new_year)
{
    m_ui->spinYear->setValue(new_year);
    return;
}

int CalendarWidget::daysInMonth(const int month_no) const
{
    if (month_no == Month::NAMELESS)
        return 5;
    else
        return 30;
}


/* Private methods ***********************************************************/
void CalendarWidget::setupUi()
{
    m_ui->tableCalendar->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int col=0; col < m_ui->tableCalendar->columnCount(); col++)
        m_ui->tableCalendar->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
    connect(m_ui->comboMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWidget::onChangeMonth);
    connect(m_ui->spinYear, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeYear);
    connect(m_ui->tableCalendar, &QTableWidget::itemSelectionChanged, this, &CalendarWidget::onChangeDay);
    connect(m_ui->buttonPreviousMonth, &QPushButton::clicked, this, &CalendarWidget::onPreviousMonth);
    connect(m_ui->buttonPreviousDay, &QPushButton::clicked, this, &CalendarWidget::onPreviousDay);
    connect(m_ui->buttonNextDay, &QPushButton::clicked, this, &CalendarWidget::onNextDay);
    connect(m_ui->buttonNextMonth, &QPushButton::clicked, this, &CalendarWidget::onNextMonth);
    fillMonth(1, 1);
    return;
}

int CalendarWidget::dayBasis(const int day, const int month, const int year_hal) const
{
    int year_basis = year_hal % 28;
    int month_basis = (month-1)*2;
    int dow_basis = year_basis + month_basis + day;
    return dow_basis;
}

int CalendarWidget::dayOfWeek(const int day_basis) const
{
    int dow = (day_basis+1) % 7 + 1;
    return dow;
}

int CalendarWidget::dayOfWeek(const int day, const int month, const int year_hal) const
{
    int dow_basis = dayBasis(day, month, year_hal);
    return dayOfWeek(dow_basis);
}

int CalendarWidget::moonPhase(const int day_basis) const
{
    int moon_phase = ((day_basis % 28)-1)/7;
    return moon_phase;
}

int CalendarWidget::moonPhase(const int day, const int month, const int year_hal) const
{
    int day_basis = dayBasis(day, month, year_hal);
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

void CalendarWidget::onPreviousYear()
{
    setYear(year()-1);
    return;
}

void CalendarWidget::onNextYear()
{
    setYear(year()+1);
    return;
}


void CalendarWidget::onPreviousMonth()
{
    int new_month = month() - 1;
    if (new_month < 1) {
        onPreviousYear();
        new_month = noOfMonths();
    }
    setMonth(new_month);
    return;
}

void CalendarWidget::onNextMonth()
{
    int new_month = month() + 1;
    if (new_month > noOfMonths()) {
        onNextYear();
        new_month = 1;
    }
    setMonth(new_month);
    return;
}

void CalendarWidget::onPreviousDay()
{
    int new_day = day();
    if (new_day == 0) // no day selected
        return;
    if (new_day < 0)
        new_day = (new_day == -1) ? 0 : daysInMonth(month());
    else
        new_day--;
    if (new_day < 1) {
        onPreviousMonth();
        new_day = daysInMonth(month());
    }
    setDay(new_day);
    return;
}

void CalendarWidget::onNextDay()
{
    int new_day = day();
    if (new_day == 0) // no day selected
        return;
    if (new_day < 0)
        new_day = (new_day == -1) ? 1 : daysInMonth(month())+1;
    else
        new_day++;
    if (new_day > daysInMonth(month())) {
        onNextMonth();
        new_day = 1;
    }
    setDay(new_day);
    return;
}
