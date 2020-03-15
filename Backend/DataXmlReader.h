/**
 * Data XML reader base class for the Role Playing Game Assistant.
 *
 * Copyright (c) 2020 Andreas Schneider (andreas underscore schn at web dot de)
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

#include <QXmlStreamReader>

class DataXmlReader
{
public:
    DataXmlReader();

    /**
     * @brief Read an XML list file.
     * @param device
     * @return true on success, otherwise false
     */
    bool read(QIODevice *device);

    /**
     * @brief Gives a string representing the error that occurred.
     * @return string representing the error
     */
    QString errorString() const;

    /**
     * @brief Purely virtual function to read an entry in the data list.
     */
    virtual void readEntry() = 0;

protected:
    /**
     * @return Name of the root XML tag of the list.
     */
    virtual inline QString dataName() = 0;
    /**
     * @return Name of the XML tag of an entry.
     */
    virtual inline QString entryName() = 0;

    QXmlStreamReader m_xml_reader;
};
