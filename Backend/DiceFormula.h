/**
 * Dice formulas for the Role Playing Game Assistant.
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
    int roll(QString* strptr = nullptr) const;

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
