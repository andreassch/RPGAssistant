#pragma once

#include <vector>
#include <QString>
#include <QXmlStreamReader>

enum class Gender
{
    FEMALE,
    MALE
};

enum class SurnameType
{
    LIST, PARTS, PARENTNAME
};

class Names
{
public:
    Names();

    static QString genderString(const Gender& gender);

    inline QString region() const { return m_region; }
    QString firstname(const Gender& gender, const size_t index) const;
    int firstnamesCount(const Gender& gender) const;
    inline SurnameType surnameType() const { return m_surname_type; }
    QString surname(const size_t index) const;
    inline int surnamesCount() const { return m_surnames.size(); }
    QString surnamePrefix(const Gender& gender) const;
    QString surnamePostfix(const Gender& gender) const;

    inline void setRegion(const QString& region_name) { m_region = region_name; }
    void setFirstnames(const std::vector<QString>& name_list, const Gender& gender);
    void addFirstname(const QString& new_name, const Gender& gender);
    inline void setSurnameType(const SurnameType& new_type) { m_surname_type = new_type; }
    inline void setSurnames(const std::vector<QString>& surname_list) { m_surnames = surname_list; }
    inline void setSurnamesPartStart(const std::vector<QString>& part_list) { m_surname_part_begin = part_list; }
    inline void setSurnamesPartEnd(const std::vector<QString>& part_list) { m_surname_part_end = part_list; }
    void addSurname(const QString& new_name);
    void addSurnameStart(const QString& new_start);
    void addSurnameEnding(const QString& new_ending);
    void setSurnamePrefix(const QString& new_prefix, const Gender& gender);
    void setSurnamePostfix(const QString& new_postfix, const Gender& gender);

    QString randomFirstname(const Gender& gender) const;
    QString randomSurname(const Gender& gender = Gender::FEMALE) const;
    QString randomName(const Gender& gender) const;

private:
    QString m_region;
    std::vector<QString> m_firstnames_female;
    std::vector<QString> m_firstnames_male;
    SurnameType m_surname_type;
    std::vector<QString> m_surnames;
    std::vector<QString> m_surname_part_begin;
    std::vector<QString> m_surname_part_end;
    QString m_surname_prefix_female = QString("");
    QString m_surname_prefix_male = QString("");
    QString m_surname_postfix_female = QString("");
    QString m_surname_postfix_male = QString("");
};
