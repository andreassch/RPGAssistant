/**
 * Utility functions for the Role Playing Game Assistant.
 *
 * Copyright (c) 2019, 2020 Andreas Schneider (andreas underscore schn at web dot de)
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

#include <QTableWidgetItem>

namespace UiUtils {
    /**
     * @brief Private template helper function to create a TableWidgetItem with given contents of T.
     * Typical examples for T are int or float.
     * @param number An object of type T that will be inserted in a QTableWidgetItem.
     * @return A QTableWidgetItem containing the entry of type T specified by the parameter number.
     */
    template<class T> QTableWidgetItem* createTableWidgetNumericItem(const T number, const Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsUserCheckable, const int alignment = Qt::AlignCenter)
    {
        QTableWidgetItem *item = new QTableWidgetItem;
        item->setData(Qt::EditRole, number);
        item->setTextAlignment(alignment);
        item->setFlags(flags);
        return item;
    }
}
