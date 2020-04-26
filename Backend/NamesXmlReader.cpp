#include "NamesXmlReader.h"

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
    namelist.setRegion(region);
    while (m_xml_reader.readNextStartElement()) {
        QString type_attrib;
        if (m_xml_reader.attributes().hasAttribute(typeAttribute()))
            type_attrib = m_xml_reader.attributes().value(typeAttribute()).toString();
        else
            type_attrib = "list";
        if (m_xml_reader.name() == surnamesName()) {
            if (type_attrib == "list") {
                namelist.setSurnameType(SurnameType::LIST);
                namelist.setSurnames(readNames());
            }
            else if (type_attrib == "parts") {
                namelist.setSurnameType(SurnameType::PARTS);
                while (m_xml_reader.readNextStartElement()) {
                    if (m_xml_reader.name() == partsName()) {
                        if (m_xml_reader.attributes().value(partPlaceAttribute()) == "beginning")
                            namelist.setSurnamesPartStart(readNames());
                        else if (m_xml_reader.attributes().value(partPlaceAttribute()) == "end")
                            namelist.setSurnamesPartEnd(readNames());
                        else
                            m_xml_reader.skipCurrentElement();
                    }
                    else
                        m_xml_reader.skipCurrentElement();
                }
            }
            else if (type_attrib == "parentname") {
                namelist.setSurnameType(SurnameType::PARENTNAME);
                while (m_xml_reader.readNextStartElement()) {
                    if (m_xml_reader.name() == prefixName()) {
                        Gender gender = (m_xml_reader.attributes().value(genderAttribute()) == "male") ? Gender::MALE : Gender::FEMALE;
                        namelist.setSurnamePrefix(m_xml_reader.readElementText(), gender);
                    }
                    else if (m_xml_reader.name() == postfixName()) {
                        Gender gender = (m_xml_reader.attributes().value(genderAttribute()) == "male") ? Gender::MALE : Gender::FEMALE;
                        namelist.setSurnamePostfix(m_xml_reader.readElementText(), gender);
                    }
                }
            }
            else if (m_xml_reader.attributes().value(typeAttribute()) == "none")
                m_xml_reader.skipCurrentElement();
            else
                m_xml_reader.raiseError(QObject::tr("The surname tag for region %1 does not contain a valid type attribute.").arg(region));
        }
        else if (m_xml_reader.name() == firstnamesName()) {
            if (m_xml_reader.attributes().value(genderAttribute()) == "female")
                namelist.setFirstnames(readNames(), Gender::FEMALE);
            else if (m_xml_reader.attributes().value(genderAttribute()) == "male")
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
    Q_ASSERT(m_xml_reader.isStartElement() && (m_xml_reader.name() == firstnamesName() || m_xml_reader.name() == surnamesName() || m_xml_reader.name() == partsName()));
    std::vector<QString> names;
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == nameName())
            names.insert(names.end(), m_xml_reader.readElementText());
        else
            m_xml_reader.skipCurrentElement();
    }
    return names;
}


