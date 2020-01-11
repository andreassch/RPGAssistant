#include "NamesXmlReader.h"

#include <QtDebug>

NamesXmlReader::NamesXmlReader(std::vector<Names>* namelists)
    : m_namelists(namelists)
{
}

/* Private methods ***********************************************************/
void NamesXmlReader::readEntry()
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == entryName());
    Names namelist;
    QString region = m_xml_reader.attributes().value(regionAttribute()).toString();
    qDebug() << "Names for region " << region;
    namelist.setRegion(region);
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == surnamesName())
            namelist.setSurnames(readNames());
        else if (m_xml_reader.name() == firstnamesName()) {
            if (m_xml_reader.attributes().value(genderAttribute()) == "female")
                namelist.setFirstnames(readNames(), Gender::FEMALE);
            else
                namelist.setFirstnames(readNames(), Gender::MALE);
        }
        else
            m_xml_reader.skipCurrentElement();
    }
    m_namelists->insert(m_namelists->end(), namelist);
    return;
}

std::vector<QString> NamesXmlReader::readNames()
{
    Q_ASSERT(m_xml_reader.isStartElement() && (m_xml_reader.name() == surnamesName() || m_xml_reader.name() == firstnamesName()));
    qDebug() << "Reading names: " << m_xml_reader.name();
    std::vector<QString> names;
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == nameName())
            names.insert(names.end(), m_xml_reader.readElementText());
        else
            m_xml_reader.skipCurrentElement();
    }
    qDebug() << QString("Read %1 names.").arg(names.size());
    return names;
}
