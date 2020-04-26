#include "Names.h"

#include <QRandomGenerator>

Names::Names()
{
}

QString Names::genderString(const Gender &gender)
{
    switch (gender) {
        case Gender::FEMALE:
            return QObject::tr("female");
        case Gender::MALE:
            return QObject::tr("male");
        case Gender::NEUTRAL:
            return QObject::tr("neutral");
        default:
            assert(false);
    }
    return QString("");
}

Gender Names::parseGender(const QString& gender_name)
{
    if (gender_name.compare("female") == 0)
        return Gender::FEMALE;
    else if (gender_name.compare("male") == 0)
        return Gender::MALE;
    else
        return Gender::NEUTRAL;
}

QString Names::firstname(const Gender& gender, const size_t index) const
{
    if (gender == Gender::FEMALE) {
        if (index < m_firstnames_female.size())
            return m_firstnames_female.at(index);
        else
            return QString();
    }
    else if (gender == Gender::MALE) {
        if (index < m_firstnames_male.size())
            return m_firstnames_male.at(index);
        else
            return QString();
    }
    else
        return QString();
}

QString Names::surname(const size_t index) const
{
    if (index < m_surnames.size())
        return m_surnames.at(index);
    else
        return QString();
}

int Names::firstnamesCount(const Gender& gender) const
{
    if (gender == Gender::FEMALE)
        return m_firstnames_female.size();
    else if (gender == Gender::MALE)
        return m_firstnames_male.size();
    else
        return 0;
}

void Names::setFirstnames(const std::vector<QString>& name_list, const Gender& gender)
{
    if (gender == Gender::FEMALE)
        m_firstnames_female = name_list;
    else if (gender == Gender::MALE)
        m_firstnames_male = name_list;
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

void Names::addSurname(const QString& new_name)
{
    m_surnames.insert(m_surnames.end(), new_name);
    return;
}

QString Names::surnamePrefix(const Gender &gender) const
{
    if (gender == Gender::FEMALE)
        return m_surname_prefix_female;
    else if (gender == Gender::MALE)
        return m_surname_prefix_male;
    else
        return QString();
}

QString Names::surnamePostfix(const Gender &gender) const
{
    if (gender == Gender::FEMALE)
        return m_surname_postfix_female;
    else if (gender == Gender::MALE)
        return m_surname_postfix_male;
    else
        return QString();
}

void Names::setSurnamePrefix(const QString &new_prefix, const Gender &gender)
{
    if (gender == Gender::FEMALE)
        m_surname_prefix_female = new_prefix;
    else if (gender == Gender::MALE)
        m_surname_prefix_male = new_prefix;
    return;
}

void Names::setSurnamePostfix(const QString &new_postfix, const Gender &gender)
{
    if (gender == Gender::FEMALE)
        m_surname_postfix_female = new_postfix;
    else if (gender == Gender::MALE)
        m_surname_postfix_male = new_postfix;
    return;
}

QString Names::randomSurname(const Gender& gender) const
{
    switch (m_surname_type) {
        case SurnameType::LIST:
            if (m_surnames.size() > 0) {
                int index = QRandomGenerator::global()->bounded(static_cast<int>(m_surnames.size()));
                return surname(index);
            }
            else
                return QString();
        case SurnameType::PARTS:
            if ((m_surname_part_begin.size() > 0) && (m_surname_part_end.size() > 0)) {
                int index1 = QRandomGenerator::global()->bounded(static_cast<int>(m_surname_part_begin.size()));
                int index2 = QRandomGenerator::global()->bounded(static_cast<int>(m_surname_part_end.size()));
                QString this_surname = m_surname_part_begin.at(index1);
                return this_surname.append(m_surname_part_end.at(index2));
            }
            else
                return QString();
        case SurnameType::PARENTNAME:
            QString parent_name = randomFirstname(gender);
            return parent_name.prepend(surnamePrefix(gender)).append(surnamePostfix(gender));
    }
    return QString();
}

QString Names::randomName(const Gender& gender) const
{
    QString the_firstname = randomFirstname(gender);
    QString the_surname = randomSurname(gender);

    return the_firstname.append(QString(" ")).append(the_surname);
}
