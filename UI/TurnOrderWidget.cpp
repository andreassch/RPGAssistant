#include "TurnOrderWidget.h"
#include "ui_TurnOrderWidget.h"

#ifdef RPG_SYSTEM_TDE_AVENTURIA
#include "../Backend/TDEModifer.h"
using namespace TDEModifier;
#endif

#include "Utils.h"
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QtDebug>

/* Public constructors/desctructors ******************************************/
TurnOrderWidget::TurnOrderWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::TurnOrderWidget)
{
    m_ui->setupUi(this);
    setupUi();
}

TurnOrderWidget::~TurnOrderWidget()
{
    delete m_ui;
}

/* Public methods ************************************************************/
void TurnOrderWidget::addEntry(const QString name, const float ini, const int total_hit_points, const int current_hit_points)
{
    m_ui->tableTurnOrder->setSortingEnabled(false);
    int new_row = m_ui->tableTurnOrder->rowCount();
    m_ui->tableTurnOrder->insertRow(new_row);
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem(name));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI, UiUtils::createTableWidgetNumericItem<float>(ini));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI_MOD, UiUtils::createTableWidgetNumericItem<int>(0));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::cHP, UiUtils::createTableWidgetNumericItem<int>(total_hit_points));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::tHP, UiUtils::createTableWidgetNumericItem<int>(current_hit_points));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::MOD, UiUtils::createTableWidgetNumericItem<int>(computeModifier(current_hit_points, total_hit_points)));
    m_ui->tableTurnOrder->setSortingEnabled(true);
    m_ui->buttonDelete->setEnabled(true);
    m_ui->buttonStart->setEnabled(true);
    m_ui->buttonDamage->setEnabled(true);
    return;
}

/* Private methods ***********************************************************/
void TurnOrderWidget::setupUi()
{
    m_ui->buttonDelete->setEnabled(false);
    m_ui->buttonStart->setEnabled(false);
    m_ui->buttonNext->setEnabled(false);
    m_ui->buttonIniModApply->setEnabled(false);
    m_ui->buttonWait->setEnabled(false);
    m_ui->buttonDamage->setEnabled(false);
    m_ui->tableTurnOrder->horizontalHeader()->setSectionResizeMode(TurnOrderTableColumn::NAME, QHeaderView::Stretch);
    m_ui->tableTurnOrder->sortItems(TurnOrderTableColumn::INI, Qt::DescendingOrder);
    m_ui->tableTurnOrder->setSelectionMode(QAbstractItemView::SingleSelection);
#ifdef ANDROID
    for (auto col = 1; col < m_ui->tableTurnOrder->columnCount(); col++)
        m_ui->tableTurnOrder->horizontalHeader()->resizeSection(col, 140);
#endif

    connect(m_ui->buttonAdd, &QPushButton::clicked, this, &TurnOrderWidget::onAddEntry);
    connect(m_ui->buttonDelete, &QPushButton::clicked, this, &TurnOrderWidget::onDeleteEntry);
    connect(m_ui->buttonStart, &QPushButton::clicked, this, &TurnOrderWidget::onStart);
    connect(m_ui->tableTurnOrder, &QTableWidget::cellChanged, this, &TurnOrderWidget::onChangeEntry);
    connect(m_ui->buttonNext, &QPushButton::clicked, this, &TurnOrderWidget::onNext);
    connect(m_ui->buttonDamage, &QPushButton::clicked, this, &TurnOrderWidget::onDamage);
    connect(m_ui->buttonIniModHide, &QPushButton::clicked, this, &TurnOrderWidget::onHideIniMod);
    connect(m_ui->buttonIniModApply, &QPushButton::clicked, this, &TurnOrderWidget::onApplyIniMod);
    connect(m_ui->buttonWait, &QPushButton::clicked, this, &TurnOrderWidget::onWait);
    connect(m_ui->buttonClear, &QPushButton::clicked, this, &TurnOrderWidget::onClear);
    connect(m_ui->buttonLoad, &QPushButton::clicked, this, &TurnOrderWidget::onLoad);
    connect(m_ui->buttonSave, &QPushButton::clicked, this, &TurnOrderWidget::onSave);

    return;
}

void TurnOrderWidget::activateEntry(const int number)
{
    m_ui->tableTurnOrder->item(number, TurnOrderTableColumn::NAME)->setBackground(color_active);
    return;
}

void TurnOrderWidget::deactivateEntry(const int number)
{
    m_ui->tableTurnOrder->item(number, TurnOrderTableColumn::NAME)->setBackground(QColor(1, 0, 0, 0));
    return;
}

int TurnOrderWidget::activeEntry() const
{
    for (int row=0; row<m_ui->tableTurnOrder->rowCount(); row++) {
        if (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::NAME)->background().color() == color_active)
            return row;
    }
    return -1;
}

int TurnOrderWidget::selectedEntry() const
{
    QItemSelectionModel *select = m_ui->tableTurnOrder->selectionModel();
    if (select->hasSelection())
        return select->selectedIndexes()[0].row(); // Use first entry because the selection mode allows only one single selection.
    else
        return -1;
}

/* Private slots *************************************************************/
void TurnOrderWidget::onAddEntry()
{
    addEntry(m_ui->editName->text(), static_cast<float>(m_ui->spinBoxIni->value()), m_ui->spinBoxLe->value(), m_ui->spinBoxLe->value());
    return;
}

void TurnOrderWidget::onDeleteEntry()
{
    bool is_active = false;
    int row = selectedEntry();
    if (row < 0) { // If no row is selected.
        row = activeEntry();
        is_active = true;
    }
    if (row < 0) { // If also no entry is active.
        QMessageBox::information(this, tr("Turn Order List"), tr("Please select the entry that shall be deleted."));
        return;
    }
    if (row < 0) // If no row is selected.
        return;
    if ((is_active) && (row != m_ui->tableTurnOrder->rowCount()-1))
        onNext();
    m_ui->tableTurnOrder->removeRow(row);
    if ((is_active) && (row == m_ui->tableTurnOrder->rowCount()))
        onStart();
    if (m_ui->tableTurnOrder->rowCount() == 0) {
        m_ui->buttonDelete->setEnabled(false);
        m_ui->buttonStart->setEnabled(false);
        m_ui->buttonNext->setEnabled(false);
        m_ui->buttonDamage->setEnabled(false);
        m_ui->buttonIniModApply->setEnabled(false);
    }
    return;
}

void TurnOrderWidget::onChangeEntry(const int row, const int column)
{
    if ((m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI_MOD) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::tHP) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::MOD) == nullptr))
        return;

    switch(column) {
        case TurnOrderTableColumn::cHP:
        case TurnOrderTableColumn::tHP:
            int current_hit_points = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP)->data(Qt::DisplayRole).toInt();
            int total_hit_points = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::tHP)->data(Qt::DisplayRole).toInt();
            int mod = computeModifier(current_hit_points, total_hit_points);
            m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::MOD)->setData(Qt::DisplayRole,QVariant(mod));
            if (current_hit_points <= 0)
                m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP)->setForeground(QColor("red"));
            else
                m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP)->setForeground(QColor("black"));
            break;
    }

    return;
}

void TurnOrderWidget::onStart()
{
    if (m_ui->tableTurnOrder->rowCount() == 0)
        return;
    // If an entry is currently active, deactivate it.
    int row_active = activeEntry();
    if (row_active != -1)
        deactivateEntry(row_active);
    // Go through table and apply modificators.
    for (int row=0; row<m_ui->tableTurnOrder->rowCount(); row++) {
        float ini = m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toFloat();
        int ini_mod = m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI_MOD)->data(Qt::DisplayRole).toInt();
        m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI_MOD)->setData(Qt::DisplayRole,QVariant(0));
        m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI)->setData(Qt::DisplayRole,QVariant(ini+static_cast<float>(ini_mod)));
    }
    // Start with first turn.
    current_ini = m_ui->tableTurnOrder->item(0, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toInt(); // highest INI in the list
    m_ui->labelCurrentIni->setText(QString("%1").arg(QString::number(current_ini)));
    activateEntry(0);
    m_ui->buttonNext->setEnabled(true);
    m_ui->buttonIniModApply->setEnabled(true);
    m_ui->buttonWait->setEnabled(true);
    m_ui->tableTurnOrder->clearSelection();
    return;
}

void TurnOrderWidget::onNext()
{
    if (m_ui->tableTurnOrder->rowCount() == 0)
        return;
    // Search for next active person.
    int row_active = activeEntry();
    if (row_active == -1) {
        onStart();
        return;
    }
    else {
        deactivateEntry(row_active);
        row_active++;
        if (row_active == m_ui->tableTurnOrder->rowCount())
            onStart();
        else {
            activateEntry(row_active);
            int next_ini = m_ui->tableTurnOrder->item(row_active, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toInt();
            current_ini = next_ini;
            m_ui->labelCurrentIni->setText(QString("%1").arg(QString::number(current_ini)));
        }
        m_ui->tableTurnOrder->clearSelection();
    }
    return;
}

void TurnOrderWidget::onDamage()
{
    int row = selectedEntry();
    if (row < 0) // If no row is selected.
        row = activeEntry();
    if (row < 0) { // If also no entry is active.
        QMessageBox::information(this, tr("Turn Order List"), tr("Please select the entry that shall be damaged."));
        return;
    }
    int current_hit_points = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP)->data(Qt::DisplayRole).toInt();
    int damage = m_ui->spinBoxDamage->value();
    current_hit_points -= damage;
    m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::cHP)->setData(Qt::DisplayRole,QVariant(current_hit_points));
    // The modifier is updated by the onChangeEntry callback.
    return;
}

void TurnOrderWidget::onHideIniMod()
{
    if (m_ui->tableTurnOrder->isColumnHidden(TurnOrderTableColumn::INI_MOD)) {
        m_ui->tableTurnOrder->setColumnHidden(TurnOrderTableColumn::INI_MOD, false);
        m_ui->buttonIniModHide->setText(tr("Hide"));
    }
    else {
        m_ui->tableTurnOrder->setColumnHidden(TurnOrderTableColumn::INI_MOD, true);
        m_ui->buttonIniModHide->setText(tr("Unhide"));
    }
    return;
}

void TurnOrderWidget::onApplyIniMod()
{
    int row = selectedEntry();
    if (row < 0) // If no row is selected.
        row = activeEntry();
    if (row < 0) { // If also no entry is active.
        QMessageBox::information(this, tr("Turn Order List"), tr("Please select the entry for which the initiative modifier shall be changed."));
        return;
    }
    if (row != -1) {
        int ini_mod = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI_MOD)->data(Qt::DisplayRole).toInt();
        ini_mod += m_ui->spinBoxIniMod->value();
        m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI_MOD)->setData(Qt::DisplayRole,QVariant(ini_mod));
    }
    return;
}

void TurnOrderWidget::onWait()
{
    int row_active = activeEntry();
    int row_last = m_ui->tableTurnOrder->rowCount()-1;
    if ((row_active != -1) && (row_active != row_last)) {
        float ini = m_ui->tableTurnOrder->item(row_active, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toFloat();
        int ini_mod = m_ui->tableTurnOrder->item(row_active, TurnOrderTableColumn::INI_MOD)->data(Qt::DisplayRole).toInt();
        float new_ini = std::min(0.0f, m_ui->tableTurnOrder->item(row_last, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toFloat()-1.0f);
        onNext();
        m_ui->tableTurnOrder->setSortingEnabled(false);
        m_ui->tableTurnOrder->item(row_active, TurnOrderTableColumn::INI)->setData(Qt::DisplayRole,QVariant(new_ini));
        m_ui->tableTurnOrder->item(row_active, TurnOrderTableColumn::INI_MOD)->setData(Qt::DisplayRole,QVariant(ini_mod+static_cast<int>(ini-new_ini)));
        m_ui->tableTurnOrder->setSortingEnabled(true);
    }
    return;
}

void TurnOrderWidget::onClear()
{
    m_ui->tableTurnOrder->setRowCount(0); // Delete all rows.
    m_ui->buttonDelete->setEnabled(false);
    m_ui->buttonNext->setEnabled(false);
    m_ui->buttonWait->setEnabled(false);
    m_ui->buttonIniModApply->setEnabled(false);
    m_ui->buttonDamage->setEnabled(false);
    return;
}

void TurnOrderWidget::onLoad()
{
#ifdef _WIN32
QString file_filter = tr("XML files (*.xml);;All files (*.*)");
#else
QString file_filter = tr("XML files (*.xml);;All files (*)");
#endif
#ifdef ANDROID
    QString filename(QFileDialog::getOpenFileName(this,tr("Open turn order list"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), file_filter, &file_filter, QFileDialog::DontUseNativeDialog));
#else
    QString filename(QFileDialog::getOpenFileName(this,tr("Open turn order list"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), file_filter, &file_filter));
#endif
    if (filename.isEmpty())
        return;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Turn Order List"), tr("Cannot open file %1 for reading.").arg(file.errorString()));
        return;
    }
    QXmlStreamReader xml_reader(&file);
    if (xml_reader.readNextStartElement()) {
        if (xml_reader.name() == "turnorderlist") {
            m_ui->tableTurnOrder->setSortingEnabled(false);
            while (xml_reader.readNextStartElement()){
                if (xml_reader.name() == "entry") {
                    int new_row = m_ui->tableTurnOrder->rowCount();
                    m_ui->tableTurnOrder->insertRow(new_row);
                    // Read a row of a turn order table from the file.
                    while (xml_reader.readNextStartElement()) {
                        if (xml_reader.name() == "name") {
                            QString str = xml_reader.readElementText();
                            m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem(str));
                        }
                        else if (xml_reader.name() == "ini") {
                            QString str = xml_reader.readElementText();
                            m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI, UiUtils::createTableWidgetNumericItem<float>(str.toFloat()));
                        }
                        else if ( (xml_reader.name() == "totalHitPoints") || (xml_reader.name() == "le") ) {
                            QString str = xml_reader.readElementText();
                            m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::tHP, UiUtils::createTableWidgetNumericItem<int>(str.toInt()));
                        }
                        else if ( (xml_reader.name() == "currentHitPoints") || (xml_reader.name() == "lep") ) {
                            QString str = xml_reader.readElementText();
                            m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::cHP, UiUtils::createTableWidgetNumericItem<int>(str.toInt()));
                        }
                        else
                            xml_reader.skipCurrentElement();
                    }
                    // Check if all columns have been filled by the file, and create non-existant entries.
                    for (int column=0; column < m_ui->tableTurnOrder->columnCount(); column++) {
                        if (m_ui->tableTurnOrder->item(new_row, column) == nullptr) {
                            switch(column) {
                                case TurnOrderTableColumn::NAME:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem("[unknown]"));
                                    break;
                                case TurnOrderTableColumn::INI:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::tHP, UiUtils::createTableWidgetNumericItem<float>(0.));
                                    break;
                                case TurnOrderTableColumn::INI_MOD:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI_MOD, UiUtils::createTableWidgetNumericItem<int>(0));
                                    break;
                                case TurnOrderTableColumn::tHP:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::tHP, UiUtils::createTableWidgetNumericItem<int>(0));
                                    break;
                                case TurnOrderTableColumn::cHP:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::cHP, UiUtils::createTableWidgetNumericItem<int>((m_ui->tableTurnOrder->item(new_row, TurnOrderTableColumn::tHP) == nullptr) ? 0 : m_ui->tableTurnOrder->item(new_row, TurnOrderTableColumn::tHP)->data(Qt::DisplayRole).toInt()));
                                    break;
                                case TurnOrderTableColumn::MOD:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::MOD, UiUtils::createTableWidgetNumericItem<int>(0));
                                    break;
                            }
                        }
                    }
                }
                else
                    xml_reader.skipCurrentElement();
            }
            m_ui->tableTurnOrder->setSortingEnabled(true);
        }
        else
            xml_reader.raiseError(QObject::tr("Incorrect turn order XML file."));
    }
    file.close();
    if (xml_reader.hasError()) {
        QMessageBox::critical(this, tr("Turn Order List"), tr("Error reading XML file %1: line %2 column %3: %4").arg(filename).arg(QString::number(xml_reader.lineNumber())).arg(QString::number(xml_reader.columnNumber())).arg(xml_reader.errorString()));
    }
    if (m_ui->tableTurnOrder->rowCount() > 0) {
        m_ui->buttonStart->setEnabled(true);
        m_ui->buttonDelete->setEnabled(true);
        m_ui->buttonDamage->setEnabled(true);
    }
    return;
}

void TurnOrderWidget::onSave()
{
#ifdef _WIN32
QString file_filter = tr("XML files (*.xml);;All files (*.*)");
#else
QString file_filter = tr("XML files (*.xml);;All files (*)");
#endif
#ifdef ANDROID
    if (!QDir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation)).exists()) {
        qDebug() << "Creating Documents directory:" << QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
        QDir().mkdir(QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation));
    }
    QString filename(QFileDialog::getSaveFileName(this,tr("Save turn order list"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), file_filter, &file_filter, QFileDialog::DontUseNativeDialog));
#else
    QString filename(QFileDialog::getSaveFileName(this,tr("Save turn order list"), QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation), file_filter, &file_filter));
#endif
    if (filename.isEmpty())
        return;
    if (!filename.endsWith(".xml"))
        filename = filename.append(".xml");
    QFile file(filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("Turn Order List"), tr("Cannot open file %1 for writing.").arg(file.errorString()));
        return;
    }
    QXmlStreamWriter xml_writer(&file);
    xml_writer.setAutoFormatting(true);
    xml_writer.writeStartDocument();
    xml_writer.writeDTD(QStringLiteral("<!DOCTYPE XML>"));
    xml_writer.writeStartElement(QStringLiteral("turnorderlist"));
    for (int row = 0; row < m_ui->tableTurnOrder->rowCount(); row++) {
        xml_writer.writeStartElement("entry");
        xml_writer.writeTextElement("name", m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::NAME)->data(Qt::DisplayRole).toString());
        xml_writer.writeTextElement("ini", m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toString());
        xml_writer.writeTextElement("totalHitPoints", m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::tHP)->data(Qt::DisplayRole).toString());
        xml_writer.writeEndElement();
    }
    xml_writer.writeEndDocument();
    file.close();
    return;
}
