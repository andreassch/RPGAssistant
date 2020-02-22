#include "PersonsXmlReader.h"

#include <QtDebug>

PersonsXmlReader::PersonsXmlReader(std::vector<Person>* person_list)
    : m_person_list(person_list)
{
}


/* Private methods ***********************************************************/
void PersonsXmlReader::readEntry()
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == entryName());
    Person person;
    QString region = m_xml_reader.attributes().value("region").toString();
    person.setRegion(region);
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == "term") {
            AgePeriod age_period = parseAgePeriod(m_xml_reader.attributes().value("age").toString());
            Gender gender = parseGender(m_xml_reader.attributes().value("gender").toString());
            Gender grammatical_gender = parseGender(m_xml_reader.attributes().value("grammaticalGender").toString());
            QString term_name = m_xml_reader.readElementText();
            person.setTerm(term_name, age_period, gender, grammatical_gender);
        }
        else if (m_xml_reader.name() == "ageLimit") {
            AgePeriod age_period = parseAgePeriod(m_xml_reader.attributes().value("age").toString());
            person.setAgeLimit(m_xml_reader.readElementText().toUInt(), age_period);
        }
        else if (m_xml_reader.name() == "ageRange") {
            person.setAgeRange(m_xml_reader.attributes().value("min").toUInt(), m_xml_reader.attributes().value("max").toUInt());
            m_xml_reader.skipCurrentElement();
        }
        else if (m_xml_reader.name() == "size") {
            person.setSizeFormula(m_xml_reader.readElementText());
        }
        else if (m_xml_reader.name() == "haircolors") {
            readProperty(&person, "color", &Person::addHaircolor);
        }
        else if (m_xml_reader.name() == "appearances") {
            readProperty(&person, "appearance", &Person::addAppearance);
        }
        else if (m_xml_reader.name() == "beards") {
            person.setBeardPercentage(m_xml_reader.attributes().value("percentage").toUInt());
            readProperty(&person, "beard", &Person::addBeard);
        }
        else if (m_xml_reader.name() == "traits") {
            readProperty(&person, "trait", &Person::addTrait);
        }
        else if (m_xml_reader.name() == "professions") {
            readProfession(&person);
        }
        else if (m_xml_reader.name() == "specialties") {
            readProperty(&person, "specialty", &Person::addSpecial);
        }
        else {
            qDebug() << "Unrecognised tag" << m_xml_reader.name();
            m_xml_reader.skipCurrentElement();
        }
    }
    m_person_list->insert(m_person_list->end(), person);
    return;
}

void PersonsXmlReader::readProperty(Person* person, const QString entry_name, void (Person::*addFunction)(const QString&, const unsigned int))
{
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == entry_name) {
            unsigned int weight = m_xml_reader.attributes().value("weight").toUInt();
            QString name = m_xml_reader.readElementText();
            (person->*addFunction)(name, weight);
        }
        else {
            qDebug() << "readProperty: unrecognised tag" << m_xml_reader.name();
            m_xml_reader.skipCurrentElement();
        }
    }
    return;
}

void PersonsXmlReader::readProfession(Person *person)
{
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == "profession") {
            unsigned int weight = m_xml_reader.attributes().value("weight").toUInt();
            int value = m_xml_reader.attributes().value("value").toInt();
            QString name = m_xml_reader.readElementText();
            person->addProfession(name, weight, value);
        }
        else {
            qDebug() << "readProfession: unrecognised tag" << m_xml_reader.name();
            m_xml_reader.skipCurrentElement();
        }
    }
    return;
}

Gender PersonsXmlReader::parseGender(const QString &gender_str)
{
    Gender gender = Gender::NEUTRAL; // initialise with default for case that parsing fails
    if (gender_str.compare("female") == 0)
        gender = Gender::FEMALE;
    else if (gender_str.compare("male") == 0)
        gender = Gender::MALE;
    else if (gender_str.compare("neutral") == 0)
        gender = Gender::NEUTRAL;
    else
        m_xml_reader.raiseError(QObject::tr("Error in persons XML file: incorrect gender attribute value %1.").arg(gender_str));
    return gender;
}

AgePeriod PersonsXmlReader::parseAgePeriod(const QString &age_str)
{
    AgePeriod age_period = AgePeriod::ADULT; // initialise with default for case that parsing fails
    if (age_str.compare("child") == 0)
        age_period = AgePeriod::CHILD;
    else if (age_str.compare("adult") == 0)
        age_period = AgePeriod::ADULT;
    else if (age_str.compare("elder"))
        age_period = AgePeriod::ELDER;
    else
        m_xml_reader.raiseError(QObject::tr("Error in persons XML file: incorrect age attribute value %1.").arg(age_str));
    return age_period;
}
