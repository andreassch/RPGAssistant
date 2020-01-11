#pragma once

#include "Backend/Names.h"
#include "DataXmlReader.h"

class NamesXmlReader : public DataXmlReader
{
public:
    NamesXmlReader(std::vector<Names>* namelists);

    /* Public methods read() and errorString() are inherited. */

private:
    void readEntry();
    std::vector<QString> readNames();

    inline QString dataName() { return QStringLiteral("namelists"); }
    inline QString entryName(){ return QStringLiteral("namelist"); }
    static inline QString regionAttribute() { return QStringLiteral("region"); }
    static inline QString surnamesName() { return QStringLiteral("surnames"); }
    static inline QString firstnamesName() { return QStringLiteral("firstnames"); }
    static inline QString genderAttribute() { return QStringLiteral("gender"); }
    static inline QString nameName() { return QStringLiteral("n"); }

    std::vector<Names>* m_namelists;
};
