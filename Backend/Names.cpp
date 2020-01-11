#include "Names.h"

#include <QRandomGenerator>
#include <QDebug>

Names::Names()
{
}

QString Names::surname(const size_t index) const
{
    if (index < m_surnames.size())
        return m_surnames.at(index);
    else
        return QString();
}

QString Names::firstname(const Gender& gender, const size_t index) const
{
    if (gender == Gender::FEMALE) {
        if (index < m_firstnames_female.size())
            return m_firstnames_female.at(index);
        else
            return QString();
    }
    else {
        if (index < m_firstnames_male.size())
            return m_firstnames_male.at(index);
        else
            return QString();
    }
}

int Names::firstnamesCount(const Gender& gender) const
{
    if (gender == Gender::FEMALE)
        return m_firstnames_female.size();
    else
        return m_firstnames_male.size();
}

void Names::setFirstnames(const std::vector<QString>& name_list, const Gender& gender)
{
    if (gender == Gender::FEMALE)
        m_firstnames_female = name_list;
    else if (gender == Gender::MALE)
        m_firstnames_male = name_list;
    return;
}

void Names::addSurname(const QString& new_name)
{
    m_surnames.insert(m_surnames.end(), new_name);
    return;
}

void Names::addFirstname(const QString& new_name, const Gender& gender)
{
    if (gender == Gender::FEMALE)
        m_firstnames_female.insert(m_firstnames_female.end(), new_name);
    else if (gender == Gender::MALE)
        m_firstnames_male.insert(m_firstnames_male.end(), new_name);
    return;
}

QString Names::randomFirstname(const Gender& gender) const
{
    if (firstnamesCount(gender) > 0) {
        int index = QRandomGenerator::global()->bounded(firstnamesCount(gender));
        return firstname(gender, index);
    }
    else
        return QString();
}

QString Names::randomSurname() const
{
    if (surnamesCount() > 0) {
        int index = QRandomGenerator::global()->bounded(surnamesCount());
        return surname(index);
    }
    else
        return QString();
}

QString Names::randomName(const Gender& gender) const
{
    QString the_firstname = randomFirstname(gender);
    QString the_surname = randomSurname();

    return the_firstname.append(QString(" ")).append(the_surname);
}
