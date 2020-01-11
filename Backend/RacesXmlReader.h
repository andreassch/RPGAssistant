#pragma once

#include "DataXmlReader.h"
#include "Species.h"

class RacesXmlReader : public DataXmlReader
{
public:
    RacesXmlReader(std::vector<Species>* species_list);

private:
    void readEntry();
    Race readRace();
    void readHaircolor(Race* race);
    void readEyecolor(Race* race);

    inline QString dataName() { return QStringLiteral("races"); }
    inline QString entryName() { return QStringLiteral("species"); }
    static inline QString raceName() { return QStringLiteral("race"); }
    static inline QString haircolorName() { return QStringLiteral("haircolor"); }
    static inline QString eyecolorName() { return QStringLiteral("eyecolor"); }
    static inline QString colorName() { return QStringLiteral("color"); }
    static inline QString nameAttribute() { return QStringLiteral("name"); }

    std::vector<Species>* m_species;
};
