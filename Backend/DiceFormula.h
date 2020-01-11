#pragma once

#include <variant>
#include <vector>
#include <QString>

class Dices
{
public:
    Dices(int faces = 6, int count = 1);
    inline int faceCount() const { return m_face_count; }
    inline int diceCount() const { return m_dice_count; }
    inline void setFaceCount(const int face_count) { m_face_count = face_count; }
    inline void setDiceCount(const int dice_count) { m_dice_count = dice_count; }
    int roll() const;

private:
    int m_face_count;
    int m_dice_count;
};

using Sumand = std::variant<int, Dices>;

class DiceFormula
{
public:
    DiceFormula() {}
    DiceFormula(const QString& string);
    bool fromString(const QString& string);
    QString toString() const;
    int roll() const;

private:
    std::vector<Sumand> m_formula;
};
