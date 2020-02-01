#include "CalendarWidget.h"
#include "ui_CalendarWidget.h"

#include "Utils.h"
#include <QSettings>
#include <QtDebug>

using namespace TDECalendar;

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
    QSettings settings;
    settings.setValue("calendar/day", day());
    settings.setValue("calendar/month", month());
    settings.setValue("calendar/year", yearStandardReckoning());
    settings.sync();
    delete m_ui;
}

/* Public methonds ***********************************************************/
int CalendarWidget::day() const
{
    return m_ui->spinDay->value();
}

int CalendarWidget::month() const
{
    return m_ui->comboMonth->currentIndex()+1;
}

int CalendarWidget::year() const
{
    return m_ui->spinYear->value();
}

int CalendarWidget::yearStandardReckoning() const
{
    return year();
}

void CalendarWidget::setDay(const int new_day)
{
    setDaySpin(new_day);
    setDayTab(new_day);
    onChangeDate();
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


/* Private methods ***********************************************************/
void CalendarWidget::setupUi()
{
    // Fill combo box with month names.
    for (int month_no = 1; month_no <= Calendar::noOfMonths(); month_no++)
        m_ui->comboMonth->addItem(Calendar::monthName(month_no));

    // Set resize mode in calendar table.
    m_ui->tableCalendar->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int col=0; col < m_ui->tableCalendar->columnCount(); col++)
        m_ui->tableCalendar->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);

    // Connect signals and slots.
    connect(m_ui->comboMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWidget::onChangeMonth);
    connect(m_ui->spinYear, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeYear);
    connect(m_ui->spinDay, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeDaySpin);
    connect(m_ui->tableCalendar, &QTableWidget::itemSelectionChanged, this, &CalendarWidget::onChangeDayTab);
    connect(m_ui->buttonPreviousMonth, &QPushButton::clicked, this, &CalendarWidget::onPreviousMonth);
    connect(m_ui->buttonPreviousDay, &QPushButton::clicked, this, &CalendarWidget::onPreviousDay);
    connect(m_ui->buttonNextDay, &QPushButton::clicked, this, &CalendarWidget::onNextDay);
    connect(m_ui->buttonNextMonth, &QPushButton::clicked, this, &CalendarWidget::onNextMonth);

    // Recover last viewed date from settings.
    QSettings settings;
    int curr_day = settings.value("calendar/day", QVariant(1)).toInt();
    int curr_month = settings.value("calendar/month", QVariant(1)).toInt();
    int curr_year = settings.value("calendar/year", QVariant(1)).toInt();
    fillMonth(curr_month, curr_year);
    setDayTab(curr_day);
    onChangeDate();
    return;
}

void CalendarWidget::setDaySpin(const int new_day)
{
    const QSignalBlocker blocker(m_ui->spinDay);
    m_ui->spinDay->setValue(new_day);
    return;
}

void CalendarWidget::setDayTab(const int new_day)
{
    int col = Calendar::dayOfWeek(new_day, month(), yearStandardReckoning()) - 1;
    int row = (Calendar::dayOfWeek(1, month(), yearStandardReckoning()) + new_day - 2) / 7;
    {
    QSignalBlocker blocker(m_ui->tableCalendar);
    QItemSelectionModel *selection_model = m_ui->tableCalendar->selectionModel();
    QModelIndex index = m_ui->tableCalendar->model()->index(row, col);
    selection_model->select(QItemSelection(index, index), QItemSelectionModel::ClearAndSelect);
    }
    return;
}

void CalendarWidget::fillMonth(const int month, const int year_hal)
{
    int day = 1;
    int col = Calendar::dayOfWeek(day, month, year_hal)-1;
    if (col < 0)
        col += 7;
    qDebug() << "fillMonth" << year_hal << month << col << Calendar::daysInMonth(month);
    int row = 0;
    // Clear items before start.
    for (auto col_del=0; col_del<col;col_del++) {
        if (m_ui->tableCalendar->item(row, col_del))
            delete(m_ui->tableCalendar->takeItem(row, col_del));
    }
    // Fill day numbers into table.
    for (; day <= Calendar::daysInMonth(month); day++) {
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
void CalendarWidget::onChangeYear(const int year)
{
    fillMonth(month(), year);
    setDayTab(day());
    return;
}

void CalendarWidget::onChangeMonth(const int index)
{
    int new_month = index+1; // index is zero based
    fillMonth(new_month, yearStandardReckoning());
    m_ui->spinDay->setRange(1, Calendar::daysInMonth(new_month));
    onChangeDaySpin(day());
    return;
}

void CalendarWidget::onChangeDaySpin(const int new_day)
{
    setDayTab(new_day);
    onChangeDate();
    return;
}

void CalendarWidget::onChangeDayTab()
{
    int row = m_ui->tableCalendar->currentRow();
    int column = m_ui->tableCalendar->currentColumn();
    QTableWidgetItem* item = m_ui->tableCalendar->item(row, column);
    if (item) {
        int new_day = item->data(Qt::DisplayRole).toInt();
        setDaySpin(new_day);
        onChangeDate();
    }
    return;
}

void CalendarWidget::onChangeDate()
{
    int moon_phase = Calendar::moonPhase(day(), month(), year());
    m_ui->labelMoonPhaseText->setText(Calendar::moonPhaseText(moon_phase));
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
        new_month = Calendar::noOfMonths();
    }
    setMonth(new_month);
    return;
}

void CalendarWidget::onNextMonth()
{
    int new_month = month() + 1;
    if (new_month > Calendar::noOfMonths()) {
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
        new_day = (new_day == -1) ? 0 : Calendar::daysInMonth(month());
    else
        new_day--;
    if (new_day < 1) {
        onPreviousMonth();
        new_day = Calendar::daysInMonth(month());
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
        new_day = (new_day == -1) ? 1 : Calendar::daysInMonth(month())+1;
    else
        new_day++;
    if (new_day > Calendar::daysInMonth(month())) {
        onNextMonth();
        new_day = 1;
    }
    setDay(new_day);
    return;
}
