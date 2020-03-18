#include "CalendarWidget.h"
#include "ui_CalendarWidget.h"
#include "../Backend/HolidaysXmlReader.h"

#include "Utils.h"
#include "../Backend/Utils.h"
#include <QSettings>
#include <QDir>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QMessageBox>
#include <QtDebug>
#include <cassert>

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
    qDebug() << "Calendar destructor";
    updateDiary();
    saveSettings();
    delete m_ui;
}

/* Public methonds ***********************************************************/
int CalendarWidget::day() const
{
    return m_ui->spinDay->value();
}

Month CalendarWidget::month() const
{
    int month_no = m_ui->comboMonth->currentIndex()+1;
    assert((month_no >= 1) && (month_no <= Calendar::noOfMonths()));
    return static_cast<Month>(month_no);
}

int CalendarWidget::year() const
{
    return m_ui->spinYear->value();
}

Reckoning CalendarWidget::reckoning() const
{
    return static_cast<Reckoning>(m_ui->comboReckoning->currentIndex());
}

void CalendarWidget::setDay(const int new_day)
{
    setDaySpin(new_day);
    setDayTab(new_day);
    onChangeDate();
    return;
}

void CalendarWidget::setMonth(const Month new_month)
{
    assert(m_ui->comboMonth->count() <= Calendar::noOfMonths());
    m_ui->comboMonth->setCurrentIndex(static_cast<int>(new_month)-1);
    return;
}

void CalendarWidget::setYear(const int new_year)
{
    m_ui->spinYear->setValue(new_year);
    return;
}

void CalendarWidget::setReckoning(const Reckoning new_reckoning)
{
    m_ui->comboReckoning->setCurrentIndex(static_cast<int>(new_reckoning));
    m_date = Date(day(), month(), year(), new_reckoning);
    return;
}


/* Private methods ***********************************************************/
void CalendarWidget::setupUi()
{
    // Fill combo box with month names.
    for (int month_no = 1; month_no <= Calendar::noOfMonths(); month_no++)
        m_ui->comboMonth->addItem(Calendar::monthName(static_cast<Month>(month_no)));

    // Fill combo box with reckoning names.
    for (int reckoning_no = 0; reckoning_no < Calendar::noOfReckonings(); reckoning_no++)
        m_ui->comboReckoning->addItem(Calendar::reckoningName(static_cast<Reckoning>(reckoning_no)));

    // Create table columns for week days.
    m_ui->tableCalendar->setColumnCount(Calendar::daysInWeek());
    qDebug() << m_ui->tableCalendar->columnCount();
    for (int col = 0; col < m_ui->tableCalendar->columnCount(); col++) {
        QTableWidgetItem* header = new QTableWidgetItem(Calendar::weekdayAbbreviation(col+1),QTableWidgetItem::Type);
        m_ui->tableCalendar->setHorizontalHeaderItem(col, header);
    }
    // Set resize mode of columns in calendar table.
    m_ui->tableCalendar->setSelectionMode(QAbstractItemView::SingleSelection);
    for (int col=0; col < m_ui->tableCalendar->columnCount(); col++)
        m_ui->tableCalendar->horizontalHeader()->setSectionResizeMode(col, QHeaderView::Stretch);
    // On Android, change height of calendar table.
#ifdef ANDROID
    m_ui->tableCalendar->setMinimumSize(700, 400);
#elif _WIN32
    m_ui->tableCalendar->setMinimumSize(350, 205);
#else
    m_ui->tableCalendar->setMinimumSize(350, 203);
#endif

    // Read holidays from XML file.
    QDir data_dir = Utils::dataDir();
    QFile holidays_file(data_dir.filePath("holidays.xml"));
    if(!holidays_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Calendar"), tr("Cannot open file %1 for reading: %2").arg(holidays_file.fileName()).arg(holidays_file.errorString()));
    }
    else {
        HolidaysXmlReader holidays_reader(&m_holidays);
        holidays_reader.read(&holidays_file);
        holidays_file.close();
        qDebug() << m_holidays.size() << "holidays read.";
    }

    // Connect signals and slots.
    connect(m_ui->comboReckoning, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWidget::onChangeReckoning);
    connect(m_ui->spinYear, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeYear);
    connect(m_ui->comboMonth, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &CalendarWidget::onChangeMonth);
    connect(m_ui->spinDay, QOverload<int>::of(&QSpinBox::valueChanged), this, &CalendarWidget::onChangeDaySpin);
    connect(m_ui->tableCalendar, &QTableWidget::itemSelectionChanged, this, &CalendarWidget::onChangeDayTab);
    connect(m_ui->buttonPreviousMonth, &QPushButton::clicked, this, &CalendarWidget::onPreviousMonth);
    connect(m_ui->buttonPreviousDay, &QPushButton::clicked, this, &CalendarWidget::onPreviousDay);
    connect(m_ui->buttonNextDay, &QPushButton::clicked, this, &CalendarWidget::onNextDay);
    connect(m_ui->buttonNextMonth, &QPushButton::clicked, this, &CalendarWidget::onNextMonth);
    connect(m_ui->buttonNewDiary, &QPushButton::clicked, this, &CalendarWidget::onNewDiary);
    connect(m_ui->buttonCloseDiary, &QPushButton::clicked, this, &CalendarWidget::onCloseDiary);

    // Recover last viewed date from settings.
    QSettings settings;
    int curr_day = settings.value("Calendar/day", QVariant(1)).toInt();
    int curr_month_no = settings.value("Calendar/month", QVariant(1)).toInt();
    assert((curr_month_no >= 1) && (curr_month_no <= Calendar::noOfMonths()));
    Month curr_month = static_cast<Month>(curr_month_no);
    int curr_year = settings.value("Calendar/year", QVariant(1)).toInt();
    QString curr_reckoning_str = settings.value("Calendar/reckoning", QVariant("Hal")).toString();
    Reckoning curr_reckoning = Calendar::parseReckoning(curr_reckoning_str);
    QString diary_filename = settings.value("Calendar/diary", QVariant("")).toString();
    qDebug() << "Diary file:", diary_filename;
    if (!diary_filename.isEmpty())
        loadDiary(diary_filename);
    else
        m_ui->buttonCloseDiary->setEnabled(false);
    fillMonth(curr_month, curr_year, curr_reckoning);
    setReckoning(curr_reckoning);
    setYear(curr_year);
    setMonth(curr_month);
    setDay(curr_day);
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
    int col = Calendar::dayOfWeek(new_day, month(), year(), reckoning()) - 1;
    int row = (Calendar::dayOfWeek(1, month(), year(), reckoning()) + new_day - 2) / Calendar::daysInWeek();
    {
    QSignalBlocker blocker(m_ui->tableCalendar);
    QItemSelectionModel *selection_model = m_ui->tableCalendar->selectionModel();
    QModelIndex index = m_ui->tableCalendar->model()->index(row, col);
    selection_model->select(QItemSelection(index, index), QItemSelectionModel::ClearAndSelect);
    }
    return;
}

void CalendarWidget::fillMonth(const Month month, const int year, const Reckoning reckoning)
{
    int day = 1;
    int col = Calendar::dayOfWeek(day, month, year, reckoning)-1;
    qDebug() << "fillMonth" << reckoning << year << month << col << Calendar::daysInMonth(month);
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

void CalendarWidget::saveSettings()
{
    qDebug() << "CalendarWidget::saveSettings";
    QSettings settings;
    settings.setValue("Calendar/day", day());
    settings.setValue("Calendar/month", static_cast<int>(month()));
    settings.setValue("Calendar/year", year());
    settings.setValue("Calendar/reckoning", Calendar::reckoningSymbolicName(reckoning()));
    settings.setValue("Calendar/diary", m_diary.filename());
    qDebug() << "diary file" << m_diary.filename();
    settings.sync();
    if (!m_diary.isEmpty())
        saveDiary();
    return;
}

void CalendarWidget::updateDiary()
{
    if (m_ui->textDiary->toPlainText().length() > 0)
        m_diary.addEntry(m_date, m_ui->textDiary->toPlainText());
    return;
}

void CalendarWidget::loadDiary(const QString &filename)
{
    m_diary.setFilename(filename);
    bool success = m_diary.readFromFile();
    if (!success)
        QMessageBox::critical(this, tr("Calendar"), tr("Cannot read diary file %1.").arg(filename));
    m_ui->textDiary->setEnabled(true);
    m_ui->textDiary->setText(m_diary.entry(m_date));
    QFileInfo fileinfo(filename);
    m_ui->labelDiary->setText(tr("Diary: %1").arg(fileinfo.fileName()));
    m_ui->buttonCloseDiary->setEnabled(true);
    return;
}

void CalendarWidget::saveDiary()
{
    if (m_diary.isEmpty() || m_diary.filename().isEmpty())
        return;
    qDebug() << "saveDiary";
    bool success = m_diary.writeToFile();
    if (!success)
        QMessageBox::critical(this, tr("Calendar"), tr("Cannot write diary file %1.").arg(m_diary.filename()));
    return;
}

/* Private slots *************************************************************/
void CalendarWidget::onChangeReckoning(const int index)
{
    Reckoning new_reckoning = static_cast<Reckoning>(index);
    int new_year = Calendar::convertReckoning(year(), m_date.reckoning(), new_reckoning);
    qDebug() << "onChangeReckoning" << year() << m_date.reckoning() << new_year << new_reckoning;
    setYear(new_year);
    m_date.setReckoning(new_reckoning);
    return;
}

void CalendarWidget::onChangeYear(const int year)
{
    fillMonth(month(), year, reckoning());
    setDayTab(day());
    onChangeDate();
    return;
}

void CalendarWidget::onChangeMonth(const int index)
{
    Month new_month = static_cast<Month>(index+1); // index is zero based
    fillMonth(new_month, year(), reckoning());
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
    // Care for diary and update date member.
    updateDiary();
    qDebug() << "diary contains" << m_diary.size() << "elements";
    m_ui->textDiary->clear();
    m_date.setDate(day(), month(), year(), reckoning());
    m_ui->textDiary->setText(m_diary.entry(m_date));

    // Change moon phase text and graphics.
    QDir image_dir(":/Graphics");
    MoonPhase moon_phase(day(), month(), year(), reckoning());
    m_ui->labelMoonPhaseText->setText(moon_phase.toString());
    QString filename = image_dir.filePath(moon_phase.graphicsFilename());
    qDebug() << filename;
    m_ui->widgetMoonPhase->load(filename);

    // Check holidays and fill them in the text field.
    m_ui->textHolidays->clear();
    for (std::size_t ind = 0; ind < m_holidays.size(); ind++) {
        if (m_holidays.at(ind).isAtDate(day(), month(), year(), reckoning()))
            m_ui->textHolidays->append(m_holidays.at(ind).name());
    }

    // Under Android, save current date.
#ifdef ANDROID
    saveSettings(); // Save current date all the time under Android, as app can always be killed.
#endif
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
    int new_month = static_cast<int>(month()) - 1;
    if (new_month < 1) {
        onPreviousYear();
        new_month = Calendar::noOfMonths();
    }
    setMonth(static_cast<Month>(new_month));
    return;
}

void CalendarWidget::onNextMonth()
{
    int new_month = static_cast<int>(month()) + 1;
    if (new_month > Calendar::noOfMonths()) {
        onNextYear();
        new_month = 1;
    }
    setMonth(static_cast<Month>(new_month));
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

void CalendarWidget::onNewDiary()
{
    updateDiary();
    if (!m_diary.isEmpty())
        saveDiary();
    QString file_filter = tr("XML files (*.xml);;All files (*)");
    QFileDialog::Options file_options = QFileDialog::DontConfirmOverwrite;
#ifdef ANDROID
    file_options |= QFileDialog::DontUseNativeDialog;
#endif
    QString filename(QFileDialog::getSaveFileName(this,tr("Open diary"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), file_filter, &file_filter, file_options));
    if (!filename.isEmpty()) {
        if (!filename.endsWith(".xml"))
            filename = filename.append(".xml");
        qDebug() << "new diary filename:" << filename;
        m_diary.clear();
        m_diary.setFilename(filename);
        m_ui->textDiary->clear();
        QFileInfo fileinfo(filename);
        m_ui->labelDiary->setText(tr("Diary: %1").arg(fileinfo.fileName()));
        m_ui->textDiary->setEnabled(true);
        if (fileinfo.exists())
            loadDiary(filename);
        m_ui->buttonCloseDiary->setEnabled(true);
        saveSettings();
    }
    return;
}

void CalendarWidget::onCloseDiary()
{
    updateDiary();
    if (!m_diary.isEmpty())
        saveDiary();
    m_diary.clear();
    m_diary.setFilename("");
    m_ui->labelDiary->setText(tr("Diary:"));
    m_ui->textDiary->setEnabled(false);
    m_ui->buttonCloseDiary->setEnabled(false);
    saveSettings();
    return;
}
