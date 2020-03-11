/**
 * Turn order list module for Role Playing Game Assistant.
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

#include "../config.h"

#include <QWidget>
#include <QTableWidgetItem>
#include <QColor>

/**
 * Enumeration to access the different columns in the turn order list by name.
 */
namespace TurnOrderTableColumn {
    enum TurnOrderTableColumn
    {
        NAME,
        INI,
        INI_MOD,
        LeP,
        LE,
        MOD
    };
}

QT_BEGIN_NAMESPACE
namespace Ui { class TurnOrderWidget; }
QT_END_NAMESPACE

class TurnOrderWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor.
     * @param parent The parent widget.
     */
    TurnOrderWidget(QWidget *parent = nullptr);

    /**
     * @brief TurnOrderWidget::~TurnOrderWidget
     * Destructor.
     */
    ~TurnOrderWidget();

    /**
     * @brief Adds an entry to the turn order list.
     * @param name The name of the new character.
     * @param ini The initiative value.
     * @param le The total hit points.
     */
    void addEntry(const QString name, const float ini, const int le, const int lep);

    /**
     * @brief Gets the number of entries in the turn order list.
     * @return Number of entries (rows in the table)
     */
    int entryCount() const;

private:
    /**
     * @brief Setup the user interface.
     * This private function is called by the constructor.
     */
    void setupUi();

    /**
     * @brief Activate an entry in the turn order list.
     * This function encapsulates the design of an active entry.
     * @param number The row of the entry in the turn order list that shall be activated.
     */
    void activateEntry(const int number);

    /**
     * @brief Deactivate an entry in the turn order list.
     * Undos the marking performed by activateEntry().
     * @param number The row of the entry in the turn order list that shall be deactivated.
     */
    void deactivateEntry(const int number);

    /**
     * @brief Determines the active entry in the turn order list.
     * Uses knowledge about the marking performed by activateEntry.
     * @return (First) row with a marked entry, or -1 if no entry is marked.
     */
    int activeEntry() const;

    /**
     * @brief Gets the row of the selected cell.
     * @return Row of the selected cell.
     */
    int selectedEntry() const;

private slots:
    /**
     * @brief Callback to add an entry to the turn order list.
     */
    void onAddEntry();

    /**
     * @brief Callback to delete the currently selected entry from the turn order list.
     */
    void onDeleteEntry();

    /**
     * @brief Callback executed when an entry in the turn order list is changed.
     * @param row The row of the entry that has been changed.
     * @param column The column of the entry that has been changed.
     */
    void onChangeEntry(const int row, const int column);

    /**
     * @brief Starts a new turn-based event.
     * Starts with the highest initiative present and marks the first person is the turn order list.
     */
    void onStart();

    /**
     * @brief Moves on the the next person in the turn order list.
     */
    void onNext();

    /**
     * @brief Give damage to the selected person.
     */
    void onDamage();

    /**
     * @brief Hides/unhides the column with the INI modifier.
     */
    void onHideIniMod();

    /**
     * @brief Applies the INI modifier selected in the spin box.
     */
    void onApplyIniMod();

    /**
     * @brief Allows a person to wait. Sets the initiative temporarily to zero and the modifier to undo that after the current round.
     */
    void onWait();

    /**
     * @brief Clears the current turn order list.
     */
    void onClear();

    /**
     * @brief Load a turn order list from an XML file.
     */
    void onLoad();

    /**
     * @brief Save the current turn order list to an XML file.
     */
    void onSave();

private:
    Ui::TurnOrderWidget *m_ui;
    int current_ini = 0;
    QColor color_active = QColor("green");
};
