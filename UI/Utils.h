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
