#include "DiceFormula.h"
#include <QRandomGenerator>
#include <QStringList>
#include <QtDebug>

Dices::Dices(int faces, int count)
    : m_face_count(faces), m_dice_count(count)
{
}

int Dices::roll() const
{
    qDebug() << "Rolling" << diceCount() << "D" << faceCount();
    int value = 0;
    for (auto ind=0; ind < diceCount(); ind++) {
        value += QRandomGenerator::global()->bounded(faceCount())+1;
        qDebug() << value;
    }
    return value;
}

DiceFormula::DiceFormula(const QString& string)
{
    fromString(string);
}

bool DiceFormula::fromString(const QString& string)
{
    const auto items = string.split("+");
    m_formula.clear();
    QRegExp constant("[0-9]+");
    QRegExp dice("([0-9]*)[Ww]([0-9]+)");
    for (const auto& item : items)
    {
        if (dice.indexIn(item) > -1)
        {
            int count = dice.cap(1).toInt();
            if (count == 0)
                count = 1;
            int faces = dice.cap(2).toInt();
            m_formula.emplace_back(Dices(faces, count));
        }
        else if (constant.indexIn(item) > -1)
        {
            m_formula.emplace_back(item.toInt());
        }
        else
        {
           return false; // or throw
        }
    }
    return true;
}

QString DiceFormula::toString() const
{
    QString string;
    for (const auto& sumand : m_formula)
    {
       if (std::holds_alternative<int>(sumand))
       {
           if (string.length() > 0)
               string.append("+");
           string.append(QString::number(std::get<int>(sumand)));
       }
       else if (std::holds_alternative<Dices>(sumand))
       {
           if (string.length() > 0)
               string.append("+");
           Dices dices = std::get<Dices>(sumand);
           string.append(QString("%1D%2").arg(QString::number(dices.diceCount())).arg(QString::number(dices.faceCount())));
       }
    }
    return string;
}

int DiceFormula::roll() const
{
   int value = 0;
   for (const auto& sumand : m_formula)
   {
      if (std::holds_alternative<int>(sumand))
      {
         value += std::get<int>(sumand);
      }
      else if (std::holds_alternative<Dices>(sumand))
      {
          value += std::get<Dices>(sumand).roll();
      }
   }
   return value;
}
