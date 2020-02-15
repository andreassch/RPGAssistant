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
