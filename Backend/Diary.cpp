#include "Diary.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QtDebug>

Diary::Diary(const QString& filename)
    : m_filename(filename)
{
    readFromFile();
}

void Diary::addEntry(const Date &new_date, const QString &new_text)
{
    auto search_entry = m_entries.find(new_date);
    if (search_entry == m_entries.end()) // not found
        m_entries.insert({new_date, new_text});
    else
        search_entry->second = new_text;
    return;
}

QString Diary::entry(const Date& the_date)
{
    auto search_entry = m_entries.find(the_date);
    if (search_entry == m_entries.end()) // not found
        return QString("");
    else
        return search_entry->second;
}

//bool Diary::hasEntry(const Date &the_date)
//{
//    return m_entries.contains(the_date);
//}

bool Diary::isEmpty() const
{
    return m_entries.empty();
}

void Diary::clear()
{
    m_entries.clear();
    return;
}

bool Diary::readFromFile()
{
    QFile file(m_filename);
    if(!file.open(QFile::ReadOnly | QFile::Text)) {
        return false;
    }
    QXmlStreamReader xml_reader(&file);
    m_entries.clear();
    if (xml_reader.readNextStartElement()) {
        if (xml_reader.name() == "diary") {
            while(xml_reader.readNextStartElement()){
                if(xml_reader.name() == "entry") {
                    int date_num = xml_reader.attributes().value("date").toInt();
                    QString text = xml_reader.readElementText();
                    m_entries.insert({Date(date_num), text});
                }
                else
                    xml_reader.skipCurrentElement();
            }
        }
    }
    return true;
}

bool Diary::writeToFile() const
{
    QFile file(m_filename);
    if(!file.open(QFile::WriteOnly | QFile::Text)) {
        return false;
    }
    QXmlStreamWriter xml_writer(&file);
    xml_writer.setAutoFormatting(true);
    xml_writer.writeStartDocument();
    xml_writer.writeDTD(QStringLiteral("<!DOCTYPE XML>"));
    xml_writer.writeStartElement(QStringLiteral("diary"));
    for(auto it = m_entries.begin(); it != m_entries.end(); it++) {
        Date the_date = it->first;
        xml_writer.writeStartElement("entry");
        xml_writer.writeAttribute("date", QString::number(the_date.dateNum()));
        xml_writer.writeCharacters(it->second);
        xml_writer.writeEndElement();
    }
    xml_writer.writeEndDocument();
    file.close();
    return true;
}
