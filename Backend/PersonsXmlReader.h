/**
 * Class to read persons' attribute lists from XML file.
 * This module is part of the Role Playing Game Assistant.
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
