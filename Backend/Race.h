#pragma once

#include "DiceFormula.h"
#include <QString>
#include <vector>

class Race
{
public:
    Race();

    inline QString name() const { return m_name; }
    int size();
    int weight();
    QString haircolor();
    QString eyecolor();

    inline void setName(const QString& new_name) { m_name = new_name; }
    inline void setSizeFormula(const QString& formula) { m_size_formula.fromString(formula); }
    inline void setSizeToWeight(const int modifier) { m_size2weight = modifier; }
    void addHaircolor(const QString& color) { m_haircolor_list.insert(m_haircolor_list.end(), color); }
    void addEyecolor(const QString& color) { m_eyecolor_list.insert(m_eyecolor_list.end(), color); }

private:
    QString m_name;
    DiceFormula m_size_formula;
    int m_size = -1;
    int m_size2weight;
    std::vector<QString> m_haircolor_list;
    std::vector<QString> m_eyecolor_list;
};
