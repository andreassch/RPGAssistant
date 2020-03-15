/**
 * Backend to create random persons for the Role Playing Game Assistant.
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

#include "DiceFormula.h"
#include "Names.h"
#include <QString>
#include <utility>
#include <vector>

enum class AgePeriod {
    CHILD, ADULT, ELDER
};

class Person
{
public:
    Person();

    static QString agePeriodString(AgePeriod age_period);

    inline QString region() { return m_region; }
    QString term(const AgePeriod& age_period, const Gender& gender) const;
    Gender termGrammaticalGender(const AgePeriod& age_period, const Gender& gender) const;
    unsigned int ageLimit(const AgePeriod& age_period) const;
    AgePeriod agePeriod(const unsigned int the_age) const;

    inline Gender gender() const { return m_gender; }
    QString term() const { return term(agePeriod(), gender()); }
    inline Gender termGrammaticalGender() const { return termGrammaticalGender(agePeriod(), gender()); }
    inline unsigned int age() const { return m_age; }
    inline AgePeriod agePeriod() const { return agePeriod(age()); }
    inline unsigned int size() const { return m_size; }
    inline QString haircolor() const { return m_haircolor_list.at(m_ind_haircolor); }
    inline QString appearance() const { return m_appearance_list.at(m_ind_appearance); }
    inline QString beard() const { return (m_has_beard) ? m_beard_list.at(m_ind_beard) : QString(""); }
    inline QString trait() const { return m_trait_list.at(m_ind_trait); }
    QString profession() const;
    QString special() const;
    QString description() const;

    inline void setRegion(const QString& new_region) { m_region = new_region; }
    void setTerm(const QString& name, const AgePeriod& age_period, const Gender& gender, const Gender& grammatical_gender = Gender::NEUTRAL);
    void setAgeLimit(const unsigned int limit, const AgePeriod& age_period);
    void setAgeRange(const unsigned int min_age, const unsigned int max_age) { m_age_min = min_age; m_age_max = max_age; }
    inline void setSizeFormula(const QString& formula) { m_size_formula.fromString(formula); }
    void addHaircolor(const QString& color, const unsigned int weight);
    void addAppearance(const QString& name, const unsigned int weight);
    inline void setBeardPercentage(const unsigned int percentage) { m_beard_percentage = percentage; }
    void addBeard(const QString& name, const unsigned int weight);
    void addTrait(const QString& name, const unsigned int weight);
    void addProfession(const QString& name, const unsigned int weight, const int value);
    void addSpecial(const QString& name, const unsigned int weight);

    void randomPerson(const Gender& gender);
    void randomPerson(const Gender& gender, const AgePeriod& age_period);

private:
    static size_t weightedRandomIndex(const std::vector<unsigned int> weights);
    void randomProperties();

    QString m_region;
    QString m_adult_term[3];
    Gender m_adult_grammatical_gender[3];
    QString m_child_term[3];
    Gender m_child_grammatical_gender[3];
    unsigned int m_adult_age_limit = 0;
    unsigned int m_child_age_limit = 0;
    unsigned int m_age_min = 0;
    unsigned int m_age_max = 0;
    DiceFormula m_size_formula;
    std::vector<QString> m_haircolor_list;
    std::vector<unsigned int> m_haircolor_weights;
    std::vector<QString> m_appearance_list;
    std::vector<unsigned int> m_appearance_weights;
    unsigned int m_beard_percentage;
    std::vector<QString> m_beard_list;
    std::vector<unsigned int> m_beard_weights;
    std::vector<QString> m_trait_list;
    std::vector<unsigned int> m_trait_weights;
    std::vector<QString> m_profession_list;
    std::vector<unsigned int> m_profession_weights;
    std::vector<int> m_profession_values;
    std::vector<QString> m_specials_list;
    std::vector<unsigned int> m_specials_weights;

    Gender m_gender = Gender::FEMALE;
    unsigned int m_age = m_age_min;
    unsigned int m_size = 0;
    size_t m_ind_haircolor = 0;
    size_t m_ind_appearance = 0;
    bool m_has_beard = false;
    size_t m_ind_beard = 0;
    size_t m_ind_trait = 0;
    size_t m_ind_profession = 0;
    size_t m_ind_special = 0;
};
