#pragma once

#include <vector>
#include <QString>
#include <QXmlStreamReader>

enum class Gender
{
    FEMALE,
    MALE
};

class Names
{
public:
    Names();

    inline QString region() const { return m_region; }
    QString surname(const size_t index) const;
    QString firstname(const Gender& gender, const size_t index) const;
    inline int surnamesCount() const { return m_surnames.size(); }
    int firstnamesCount(const Gender& gender) const;

    inline void setRegion(const QString& region_name) { m_region = region_name; }
    inline void setSurnames(const std::vector<QString>& surname_list) { m_surnames = surname_list; }
    void setFirstnames(const std::vector<QString>& name_list, const Gender& gender);
    void addSurname(const QString& new_name);
    void addFirstname(const QString& new_name, const Gender& gender);

    QString randomFirstname(const Gender& gender) const;
    QString randomSurname() const;
    QString randomName(const Gender& gender) const;

private:
    QString m_region;
    std::vector<QString> m_surnames;
    std::vector<QString> m_firstnames_female;
    std::vector<QString> m_firstnames_male;
};
