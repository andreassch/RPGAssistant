/**
 * Diary class for the Role Playing Game Assistant.
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

#include "Calendar.h"

#include <QString>
#include <map>


class Diary
{
public:
    Diary() {}
    Diary(const QString& filename);

    void addEntry(const Date& new_date, const QString& new_text);
    QString entry(const Date& the_date);
    //bool hasEntry(const Date& the_date);
    bool isEmpty() const;
    void clear();
    inline size_t size() const { return m_entries.size(); }
    inline void setFilename(const QString& filename) { m_filename = filename; }
    inline QString filename() const { return m_filename; }

    bool readFromFile();
    bool writeToFile() const;

private:
    std::map<Date, QString> m_entries;
    QString m_filename;
};
