/**
 * Name list XML reader class for the Role Playing Game Assistant.
 *
 * Copyright (c) Andreas Schneider (andreas underscore schn at web dot de)
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

#include "Backend/Names.h"
#include "DataXmlReader.h"

class NamesXmlReader : public DataXmlReader
{
public:
    NamesXmlReader(std::vector<Names>* namelists);

    /* Public methods read() and errorString() are inherited. */

private:
    void readEntry();
    std::vector<QString> readNames();

    inline QString dataName() { return QStringLiteral("namelists"); }
    inline QString entryName(){ return QStringLiteral("namelist"); }
    static inline QString regionAttribute() { return QStringLiteral("region"); }
    static inline QString surnamesName() { return QStringLiteral("surnames"); }
    static inline QString typeAttribute() { return QStringLiteral("type"); }
    static inline QString firstnamesName() { return QStringLiteral("firstnames"); }
    static inline QString genderAttribute() { return QStringLiteral("gender"); }
    static inline QString partsName() { return QStringLiteral("namePart"); }
    static inline QString partPlaceAttribute() { return QStringLiteral("place"); }
    static inline QString prefixName() { return QStringLiteral("prefix"); }
    static inline QString postfixName() { return QStringLiteral("postfix"); }
    static inline QString nameName() { return QStringLiteral("n"); }

    std::vector<Names>* m_namelists;
};
