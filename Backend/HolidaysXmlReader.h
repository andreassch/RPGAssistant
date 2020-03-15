/**
 * Holiday XML reader class for the Role Playing Game Assistant.
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
#include "Holiday.h"
#include <vector>

class HolidaysXmlReader : public DataXmlReader
{
public:
    HolidaysXmlReader(std::vector<Holiday>* holiday_list);

    /* Public methods read() and errorString() are inherited. */

private:
    void readEntry();

    inline QString dataName() { return QStringLiteral("holidays"); }
    inline QString entryName() { return QStringLiteral("holiday"); }

    std::vector<Holiday>* m_holiday_list;
};
