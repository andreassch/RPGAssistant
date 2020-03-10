#pragma once

#include "TDECalendar.h"
#include <QString>
#include <map>

using namespace TDECalendar;

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
