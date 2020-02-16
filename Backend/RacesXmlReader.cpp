#include "RacesXmlReader.h"
#include <QtDebug>

RacesXmlReader::RacesXmlReader(std::vector<Species>* species_list)
    : m_species(species_list)
{
}

/* Private methods ***********************************************************/
void RacesXmlReader::readEntry()
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == entryName());
    Species species;
    QString name = m_xml_reader.attributes().value(nameAttribute()).toString();
    species.setName(name);
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == raceName())
            species.addRace(readRace());
    }
    m_species->insert(m_species->end(), species);
    return;
}

Race RacesXmlReader::readRace()
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == raceName());
    Race race;
    QString name = m_xml_reader.attributes().value(nameAttribute()).toString();
    if (m_xml_reader.attributes().hasAttribute(subNameAttribute()))
        name = name.append(QString(" (%1)").arg(m_xml_reader.attributes().value(subNameAttribute()).toString()));
    qDebug() << "race" << name;
    race.setName(name);
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == "size")
            race.setSizeFormula(m_xml_reader.readElementText());
        else if (m_xml_reader.name() == "size2weight")
            race.setSizeToWeight(m_xml_reader.readElementText().toInt());
        else if (m_xml_reader.name() == haircolorName())
            readHaircolor(&race);
        else if (m_xml_reader.name() == eyecolorName())
            readEyecolor(&race);
        else
            m_xml_reader.skipCurrentElement();
    }
    return race;
}

void RacesXmlReader::readHaircolor(Race *race)
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == haircolorName());
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == colorName())
            race->addHaircolor(m_xml_reader.readElementText());
        else
            m_xml_reader.skipCurrentElement();
   }
    return;
}

void RacesXmlReader::readEyecolor(Race *race)
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == eyecolorName());
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == colorName())
            race->addEyecolor(m_xml_reader.readElementText());
        else
            m_xml_reader.skipCurrentElement();
   }
    return;
}
