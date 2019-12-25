#include "TurnOrderWidget.h"
#include "ui_TurnOrderWidget.h"

#include <cmath>
#include <QFile>
#include <QFileDialog>
#include <QStandardPaths>
#include <QXmlStreamReader>
#include <QtDebug>

/**
 * @brief TurnOrderWidget::TurnOrderWidget
 * Constructor.
 * @param parent The parent widget.
 */
TurnOrderWidget::TurnOrderWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::TurnOrderWidget)
{
    m_ui->setupUi(this);
    setupUi();
}

/**
 * @brief TurnOrderWidget::~TurnOrderWidget
 * Destructor.
 */
TurnOrderWidget::~TurnOrderWidget()
{
    delete m_ui;
}

/**
 * @brief TurnOrderWidget::setupUi
 * Setup the user interface.
 * This private function is called by the constructor.
 */
void TurnOrderWidget::setupUi()
{
    m_ui->buttonDelete->setEnabled(false);
    m_ui->buttonStart->setEnabled(false);
    m_ui->buttonNext->setEnabled(false);
    m_ui->tableTurnOrder->horizontalHeader()->setSectionResizeMode(TurnOrderTableColumn::NAME, QHeaderView::Stretch);
    m_ui->tableTurnOrder->sortItems(TurnOrderTableColumn::INI, Qt::DescendingOrder);
    m_ui->tableTurnOrder->setSelectionMode(QAbstractItemView::SingleSelection);

    connect(m_ui->buttonAdd, &QPushButton::clicked, this, &TurnOrderWidget::onAddEntry);
    connect(m_ui->buttonDelete, &QPushButton::clicked, this, &TurnOrderWidget::onDeleteEntry);
    connect(m_ui->buttonStart, &QPushButton::clicked, this, &TurnOrderWidget::onStart);
    connect(m_ui->tableTurnOrder, &QTableWidget::cellChanged, this, &TurnOrderWidget::onChangeEntry);
    connect(m_ui->buttonNext, &QPushButton::clicked, this, &TurnOrderWidget::onNext);
    connect(m_ui->buttonDamage, &QPushButton::clicked, this, &TurnOrderWidget::onDamage);
    connect(m_ui->buttonClear, &QPushButton::clicked, this, &TurnOrderWidget::onClear);
    connect(m_ui->buttonLoad, &QPushButton::clicked, this, &TurnOrderWidget::onLoad);
    connect(m_ui->buttonSave, &QPushButton::clicked, this, &TurnOrderWidget::onSave);

    return;
}

/**
 * @brief TurnOrderWidget::createTableWidgetNumericItem
 * Private template helper function to create a TableWidgetItem with given contents of T
 * Typical examples for T are int or float.
 * @param number An object of type T that will be inserted in a QTableWidgetItem.
 * @return A QTableWidgetItem containing the entry of type T specified by the parameter number.
 */
template<class T> QTableWidgetItem* TurnOrderWidget::createTableWidgetNumericItem(const T number) const
{
    QTableWidgetItem *item = new QTableWidgetItem;
    item->setData(Qt::EditRole, number);
    return item;
}

/**
 * @brief TurnOrderWidget::activateEntry
 * Activate an entry in the turn order list.
 * This function encapsulates the design of an active entry.
 * @param number The row of the entry in the turn order list that shall be activated.
 */
void TurnOrderWidget::activateEntry(const int number)
{
    qDebug() << m_ui->tableTurnOrder->item(number, TurnOrderTableColumn::NAME)->background().color();
    m_ui->tableTurnOrder->item(number, TurnOrderTableColumn::NAME)->setBackgroundColor(color_active);
    return;
}

/**
 * @brief TurnOrderWidget::deactivateEntry
 * Deactivate an entry in the turn order list.
 * Undos the marking performed by activateEntry().
 * @param number The row of the entry in the turn order list that shall be deactivated.
 */
void TurnOrderWidget::deactivateEntry(const int number)
{
    m_ui->tableTurnOrder->item(number, TurnOrderTableColumn::NAME)->setBackgroundColor(QColor(1, 0, 0, 0));
    return;
}

/**
 * @brief TurnOrderWidget::activeEntry
 * Determines the active entry in the turn order list.
 * Uses knowledge about the marking performed by activateEntry.
 * @return (First) row with a marked entry, or -1 if no entry is marked.
 */
int TurnOrderWidget::activeEntry() const
{
    for (int row=0; row<m_ui->tableTurnOrder->rowCount(); row++) {
        if (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::NAME)->background().color() == color_active)
            return row;
    }
    return -1;
}

/**
 * @brief TurnOrderWidget::computeModifierTDE
 * Compute modifier for the German rpg The Dark Eye ("Das Schwarze Auge"), fith edition.
 * @param lep Current hit points ("Lebenspunkte") of the character.
 * @param le Maximal hit points ("Lebensenergie") of the character.
 * @return Modificator due to bad health.
 */
int TurnOrderWidget::computeModifierTDE(const int lep, const int le) const
{
    if (lep <= 5)
        return 4;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.25))
        return 3;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.5))
        return 2;
    else if (static_cast<float>(lep) <= round(static_cast<float>(le)*0.75))
        return 1;
    else
        return 0;
}

/**
 * @brief TurnOrderWidget::onAddEntry
 * Adds an entry to the turn order list.
 */
void TurnOrderWidget::onAddEntry()
{
    m_ui->tableTurnOrder->setSortingEnabled(false);
    int new_row = m_ui->tableTurnOrder->rowCount();
    m_ui->tableTurnOrder->insertRow(new_row);
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem(m_ui->editName->text()));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI, createTableWidgetNumericItem<float>(static_cast<float>(m_ui->spinBoxIni->value())));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI_MOD, createTableWidgetNumericItem<int>(0));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LeP, createTableWidgetNumericItem<int>(m_ui->spinBoxLe->value()));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LE, createTableWidgetNumericItem<int>(m_ui->spinBoxLe->value()));
    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::MOD, createTableWidgetNumericItem<int>(0));
    m_ui->tableTurnOrder->setSortingEnabled(true);
    m_ui->buttonDelete->setEnabled(true);
    m_ui->buttonStart->setEnabled(true);
    return;
}

/**
 * @brief TurnOrderWidget::onDeleteEntry
 * Deletes the currently selected entry from the turn order list.
 */
void TurnOrderWidget::onDeleteEntry()
{
    int row = m_ui->tableTurnOrder->currentRow();
    qDebug() << "onDeleteEntry: row" << row;
    if (row < 0) // If no row is active (selected).
        return;
    m_ui->tableTurnOrder->removeRow(row);
    if (m_ui->tableTurnOrder->rowCount() == 0) {
        m_ui->buttonDelete->setEnabled(false);
        m_ui->buttonStart->setEnabled(false);
        m_ui->buttonNext->setEnabled(false);
    }
    return;
}

/**
 * @brief TurnOrderWidget::onChangeEntry
 * Callback executed when an entry in the turn order list is changed.
 * @param row The row of the entry that has been changed.
 * @param column The column of the entry that has been changed.
 */
void TurnOrderWidget::onChangeEntry(const int row, const int column)
{
    if ((m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::INI_MOD) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LE) == nullptr) ||
        (m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::MOD) == nullptr))
        return;

    qDebug() << "onChangeEntry" << row << column;
    switch(column) {
        case TurnOrderTableColumn::LeP:
        case TurnOrderTableColumn::LE:
            int lep = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP)->data(Qt::DisplayRole).toInt();
            int le = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LE)->data(Qt::DisplayRole).toInt();
            int mod = computeModifierTDE(lep, le);
            m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::MOD)->setData(Qt::DisplayRole,QVariant(mod));
            if (lep <= 0)
                m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP)->setForeground(QColor("red"));
            else
                m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP)->setForeground(QColor("black"));
            break;
    }

    return;
}

/**
 * @brief TurnOrderWidget::onStart
 * Starts a new turn-based event.
 * Starts with the highest initiative present and marks the first person is the turn order list.
 */
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
        qDebug() << ini << ini_mod;
        m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI_MOD)->setData(Qt::DisplayRole,QVariant(0));
        m_ui->tableTurnOrder->item(row,TurnOrderTableColumn::INI)->setData(Qt::DisplayRole,QVariant(ini+static_cast<float>(ini_mod)));
    }
    // Start with first turn.
    current_ini = m_ui->tableTurnOrder->item(0, TurnOrderTableColumn::INI)->data(Qt::DisplayRole).toInt(); // highest INI in the list
    qDebug() << "onStart: curren_ini" << current_ini;
    m_ui->labelCurrentIni->setText(QString("%1").arg(QString::number(current_ini)));
    activateEntry(0);
    m_ui->buttonNext->setEnabled(true);
    return;
}

/**
 * @brief TurnOrderWidget::onNext
 * Moves on the the next person in the turn order list.
 */
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
            qDebug() << "onNext()" << current_ini << next_ini;
            current_ini = next_ini;
            m_ui->labelCurrentIni->setText(QString("%1").arg(QString::number(current_ini)));
        }
    }
    return;
}

/**
 * @brief TurnOrderWidget::onDamage
 * Give damage to the selected person.
 */
void TurnOrderWidget::onDamage()
{
    int row = m_ui->tableTurnOrder->currentRow();
    qDebug() << "onDamage: row" << row;
    if (row < 0) // If no row is active (selected).
        return; // Do nothing.
    int lep = m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP)->data(Qt::DisplayRole).toInt();
    int damage = m_ui->spinBoxDamage->value();
    qDebug() << "onDamage()" << lep << damage;
    lep -= damage;
    m_ui->tableTurnOrder->item(row, TurnOrderTableColumn::LeP)->setData(Qt::DisplayRole,QVariant(lep));
    // The modifier is updated by the onChangeEntry callback.
    return;
}

/**
 * @brief TurnOrderWidget::onClear
 * Clears the current turn order list.
 */
void TurnOrderWidget::onClear()
{
    m_ui->tableTurnOrder->setRowCount(0); // Delete all rows.
    return;
}
/**
 * @brief TurnOrderWidget::onLoad
 * Load a turn order list from an XML file.
 */
void TurnOrderWidget::onLoad()
{
    QString filename(QFileDialog::getOpenFileName(this,tr("Open turn order list"), QStandardPaths::writableLocation(QStandardPaths::HomeLocation), tr("XML files (*.xml)")));
    qDebug() << filename;
    if (filename.isEmpty())
        return;
    QFile file(filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        qDebug() << "Cannot open file for reading:" << file.errorString();
        return;
    }
    QXmlStreamReader xml_reader(&file);
    if (xml_reader.readNextStartElement()) {
            if (xml_reader.name() == "turnorderlist") {
                qDebug() << "Reading turn order list from xml file.";
                onClear();
                m_ui->tableTurnOrder->setSortingEnabled(false);
                while(xml_reader.readNextStartElement()){
                    if(xml_reader.name() == "entry") {
                        int new_row = m_ui->tableTurnOrder->rowCount();
                        m_ui->tableTurnOrder->insertRow(new_row);
                        // Read a row of a turn order table from the file.
                        while(xml_reader.readNextStartElement()) {
                            if(xml_reader.name() == "name") {
                                QString str = xml_reader.readElementText();
                                m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem(str));
                            }
                            else if(xml_reader.name() == "ini") {
                                QString str = xml_reader.readElementText();
                                m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI, createTableWidgetNumericItem<float>(str.toFloat()));
                            }
                            else if(xml_reader.name() == "le") {
                                QString str = xml_reader.readElementText();
                                m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LE, createTableWidgetNumericItem<int>(str.toInt()));
                            }
                            else if(xml_reader.name() == "lep") {
                                QString str = xml_reader.readElementText();
                                m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LeP, createTableWidgetNumericItem<int>(str.toInt()));
                            }
                            else
                                xml_reader.skipCurrentElement();
                        }
                        // Check if all columns have been filled by the file, and create non-existant entries.
                        for (int column=0; column < m_ui->tableTurnOrder->columnCount(); column++) {
                            if (m_ui->tableTurnOrder->item(new_row, column) == nullptr) {
                                qDebug() << "Creating column that is not in the file:" << column;
                                switch(column) {
                                case TurnOrderTableColumn::NAME:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::NAME, new QTableWidgetItem("[unknown]"));
                                    break;
                                case TurnOrderTableColumn::INI:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LE, createTableWidgetNumericItem<float>(0.));
                                    break;
                                case TurnOrderTableColumn::INI_MOD:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::INI_MOD, createTableWidgetNumericItem<int>(0));
                                    break;
                                case TurnOrderTableColumn::LE:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LE, createTableWidgetNumericItem<int>(0));
                                    break;
                                case TurnOrderTableColumn::LeP:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::LeP, createTableWidgetNumericItem<int>((m_ui->tableTurnOrder->item(new_row, TurnOrderTableColumn::LE) == nullptr) ? 0 : m_ui->tableTurnOrder->item(new_row, TurnOrderTableColumn::LE)->data(Qt::DisplayRole).toInt()));
                                    break;
                                case TurnOrderTableColumn::MOD:
                                    m_ui->tableTurnOrder->setItem(new_row, TurnOrderTableColumn::MOD, createTableWidgetNumericItem<int>(0));
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
                xml_reader.raiseError(QObject::tr("Incorrect file"));
    }
    if (xml_reader.hasError()) {
        // do error handling
        qDebug() << "Error reading file:" << xml_reader.lineNumber() << xml_reader.columnNumber() << xml_reader.errorString();
    }
    if (m_ui->tableTurnOrder->rowCount() > 0) {
        m_ui->buttonStart->setEnabled(true);
        m_ui->buttonDelete->setEnabled(true);
    }
    return;
}

/**
 * @brief TurnOrderWidget::onSave
 * Save the current turn order list to an XML file.
 */
void TurnOrderWidget::onSave()
{
    // Not yet implemented.
    return;
}
