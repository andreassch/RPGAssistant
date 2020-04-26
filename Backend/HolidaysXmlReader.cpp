#include "HolidaysXmlReader.h"

HolidaysXmlReader::HolidaysXmlReader(std::vector<Holiday>* holiday_list)
    : m_holiday_list(holiday_list)
{
}

/* Private methods ***********************************************************/
void HolidaysXmlReader::readEntry()
{
    Q_ASSERT(m_xml_reader.isStartElement() && m_xml_reader.name() == entryName());
    Holiday holiday;
    QString name = m_xml_reader.attributes().value("name").toString();
    holiday.setName(name);
    QString type_str = m_xml_reader.attributes().value("durationType").toString();
    if (type_str.compare("forward") == 0)
        holiday.setDurationReference(HolidayDurationReference::FORWARD);
    else if (type_str.compare("backward") == 0)
        holiday.setDurationReference(HolidayDurationReference::BACKWARD);
    while (m_xml_reader.readNextStartElement()) {
        if (m_xml_reader.name() == "day") {
            QString reference_str = m_xml_reader.attributes().value("reference").toString();
            if (reference_str.compare("month") == 0)
                holiday.setDayReference(HolidayDayReference::MONTH);
            else if (reference_str.compare("firstWeekday") == 0)
                holiday.setDayReference(HolidayDayReference::WEEKDAY_FIRST);
            else if (reference_str.compare("lastWeekday") == 0)
                holiday.setDayReference(HolidayDayReference::WEEKDAY_LAST);
            holiday.setWeekdayCount(m_xml_reader.attributes().value("number").toInt());
            holiday.setDay(m_xml_reader.readElementText().toInt());
        }
        else if (m_xml_reader.name() == "month") {
            int month = m_xml_reader.readElementText().toInt();
            if ((month < 1) || (month > Calendar::noOfMonths()))
                m_xml_reader.raiseError(QObject::tr("Error in holiday XML file: month %1 outside range.").arg(QString::number(month)));
            holiday.setMonth(static_cast<Month>(month));
        }
        else if (m_xml_reader.name() == "duration")
            holiday.setDuration(m_xml_reader.readElementText().toInt());
        else
            m_xml_reader.skipCurrentElement();
    }
    m_holiday_list->insert(m_holiday_list->end(), holiday);
    return;
}
