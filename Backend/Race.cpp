#include "Race.h"

#include <QRandomGenerator>
#include <QDebug>

Race::Race()
{
}

int Race::size()
{
    m_size = m_size_formula.roll();
    qDebug() << m_size_formula.toString() << m_size;
    return m_size;
}

int Race::weight()
{
    if (m_size == -1)
        return -1;
    else
        return m_size - m_size2weight;
}

QString Race::haircolor()
{
    int no_of_colors = m_haircolor_list.size();
    int index = QRandomGenerator::global()->bounded(no_of_colors);
    QString the_haircolor = m_haircolor_list.at(index);
    qDebug() << no_of_colors << index << the_haircolor;
    return the_haircolor;
}

QString Race::eyecolor()
{
    int no_of_colors = m_eyecolor_list.size();
    int index = QRandomGenerator::global()->bounded(no_of_colors);
    QString the_eyecolor = m_eyecolor_list.at(index);
    qDebug() << no_of_colors << index << the_eyecolor;
    return the_eyecolor;
}
