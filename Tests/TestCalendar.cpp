#include "../Backend/Calendar.h"
#include "../Backend/Holiday.h"

#include <QtDebug>
#include <array>


int main(void)
{
#ifdef RPG_SYSTEM_TDE_AVENTURIA
    qDebug() << "d1";
    Date d1(5, MONTH_PRAIOS, 1, RECKONING_HAL);
    qDebug() << "d1" << d1.dateNum() << d1.toString();
    Date d2(3, MONTH_PRAIOS, 1, RECKONING_HAL);
    qDebug() << "d2" << d2.dateNum() << d2.toString();
    Date d3 = d1 - d2;
    qDebug() << "d1 - d2" << d3.dateNum() << d3.toString();
    Date d4(10, MONTH_RAHJA, -1, RECKONING_HAL);
    qDebug() << "d4" << d4.dateNum() << d4.toString();
    Date d5 = d1 - d4;
    qDebug() << "d1 - d4" << d5.dateNum() << d5.toString();
    Date d6(10, MONTH_EFFERD, -7, RECKONING_HAL);
    qDebug() << "d6" << d6.dateNum() << d6.toString();

    std::array test_days = {-366, -365, -364, -6, -5, -1, 0, 1, 29, 30, 31, 364, 365, 366};
    for (const int day : test_days)
        qDebug() << day << Date(day).toString();
    Date d(1, MONTH_PRAIOS, 0, RECKONING_HAL);
    qDebug() << "d" << d.dateNum() << d.toString();
#endif
    return 0;
}
