#pragma once

#include "Race.h"

class Species
{
public:
    Species() {}

    inline QString name() const { return m_name; }
    inline int raceCount() const { return m_races.size(); }
    inline Race race(int index) const { return m_races.at(index); }

    inline void setName(const QString& new_name) { m_name = new_name; }
    inline void addRace(const Race& new_race) { m_races.insert(m_races.end(), new_race); }

private:
    QString m_name;
    std::vector<Race> m_races;
};
