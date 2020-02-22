#pragma once

#include "DataXmlReader.h"
#include "Person.h"

class PersonsXmlReader : public DataXmlReader
{
public:
    PersonsXmlReader(std::vector<Person>* person_list);

private:
    void readEntry();
    void readProperty(Person* person,  const QString entry_name, void (Person::*addFunction)(const QString&, const unsigned int));
    void readProfession(Person* person);
    Gender parseGender(const QString& gender_str);
    AgePeriod parseAgePeriod(const QString& str);

    inline QString dataName() { return QStringLiteral("persons"); }
    inline QString entryName() { return QStringLiteral("person"); }

    std::vector<Person>* m_person_list;
};
