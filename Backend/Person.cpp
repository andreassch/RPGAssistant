#include "Person.h"

#include <QRandomGenerator>
#include <QDebug>
#include <cassert>


/* Constructors **************************************************************/
Person::Person()
{
}

/* Public static methods *****************************************************/
QString Person::agePeriodString(AgePeriod age_period)
{
    const QString age_period_names[3] = {QObject::tr("child"), QObject::tr("adult"), QObject::tr("elder")};
    return age_period_names[static_cast<std::underlying_type_t<AgePeriod>>(age_period)];
}


/* Public methods ************************************************************/
void Person::setTerm(const QString& name, const AgePeriod& age_period, const Gender& gender, const Gender& grammatical_gender)
{
    if (age_period == AgePeriod::ADULT) {
        m_adult_term[static_cast<std::underlying_type_t<Gender>>(gender)] = name;
        m_adult_grammatical_gender[static_cast<std::underlying_type_t<Gender>>(gender)] = grammatical_gender;
    }
    else if (age_period == AgePeriod::CHILD) {
        m_child_term[static_cast<std::underlying_type_t<Gender>>(gender)] = name;
        m_child_grammatical_gender[static_cast<std::underlying_type_t<Gender>>(gender)] = grammatical_gender;
    }
    return;
}

QString Person::term(const AgePeriod& age_period, const Gender& gender) const
{
    if (age_period == AgePeriod::CHILD)
        return m_child_term[static_cast<std::underlying_type_t<Gender>>(gender)];
    else if ( (age_period == AgePeriod::ADULT) || (age_period == AgePeriod::ELDER) )
        return m_adult_term[static_cast<std::underlying_type_t<Gender>>(gender)];
    else
        assert(false);
}

Gender Person::termGrammaticalGender(const AgePeriod &age_period, const Gender &gender) const
{
    if (age_period == AgePeriod::CHILD)
        return m_child_grammatical_gender[static_cast<std::underlying_type_t<Gender>>(gender)];
    else if ( (age_period == AgePeriod::ADULT) || (age_period == AgePeriod::ELDER) )
        return m_adult_grammatical_gender[static_cast<std::underlying_type_t<Gender>>(gender)];
    else
        assert(false);
}

void Person::setAgeLimit(const unsigned int limit, const AgePeriod &age_period)
{
    if (age_period == AgePeriod::ADULT)
        m_adult_age_limit = limit;
    else if (age_period == AgePeriod::CHILD)
        m_child_age_limit = limit;
    return;
}

unsigned int Person::ageLimit(const AgePeriod &age_period) const
{
    if (age_period == AgePeriod::ADULT)
        return m_adult_age_limit;
    else if (age_period == AgePeriod::CHILD)
        return m_child_age_limit;
    else
        assert(false);
}

AgePeriod Person::agePeriod(const unsigned int the_age) const
{
    if (the_age <= m_child_age_limit)
        return AgePeriod::CHILD;
    else if (the_age < m_adult_age_limit)
        return AgePeriod::ADULT;
    else
        return AgePeriod::ELDER;
}


void Person::addHaircolor(const QString& color, const unsigned int weight)
{
    m_haircolor_list.insert(m_haircolor_list.end(), color);
    m_haircolor_weights.insert(m_haircolor_weights.end(), weight);
    return;
}

void Person::addAppearance(const QString& name, const unsigned int weight)
{
    m_appearance_list.insert(m_appearance_list.end(), name);
    m_appearance_weights.insert(m_appearance_weights.end(), weight);
    return;
}

void Person::addBeard(const QString& name, const unsigned int weight)
{
    m_beard_list.insert(m_beard_list.end(), name);
    m_beard_weights.insert(m_beard_weights.end(), weight);
    return;
}

void Person::addTrait(const QString& name, const unsigned int weight)
{
    m_trait_list.insert(m_trait_list.end(), name);
    m_trait_weights.insert(m_trait_weights.end(), weight);
    return;
}

void Person::addProfession(const QString& name, const unsigned int weight, const int value)
{
    m_profession_list.insert(m_profession_list.end(), name);
    m_profession_weights.insert(m_profession_weights.end(), weight);
    m_profession_values.insert(m_profession_values.end(), value);
    return;
}

void Person::addSpecial(const QString& name, const unsigned int weight)
{
    m_specials_list.insert(m_specials_list.end(), name);
    m_specials_weights.insert(m_specials_weights.end(), weight);
    return;
}


QString Person::profession() const
{
    QString the_profession = m_profession_list.at(m_ind_profession);
    if (agePeriod() == AgePeriod::CHILD)
        the_profession.append(QObject::tr(" (apprentice)"));
    if (agePeriod() == AgePeriod::ELDER)
        the_profession.append(QObject::tr(" (retired)"));
    return the_profession;
}

QString Person::special() const
{
    return m_specials_list.at(m_ind_special);
}

QString Person::description() const
{
    // These strings are given in German only, because due to flection of the adjectives one English word translates into different German words depending on gender, which can't be handled with tr().
    QString indefinite_article[3] = {QString("Eine"), QString("Ein"), QString("Ein")};
    QString adjective_flection_singular[3] = {QString("e"), QString("er"), QString("es")};
    QString adjective_flextion_plural("en");
    QString the_haircolor = haircolor().append("en");
    QString the_appearance = appearance().append(adjective_flection_singular[static_cast<std::underlying_type_t<Gender>>(termGrammaticalGender())]);
    QString desc = QString("%1 %2 Jahre alt%3, %4 Schritt groß%5, %6 %7 mit %8 Haaren%9.")
            .arg(indefinite_article[static_cast<std::underlying_type_t<Gender>>(termGrammaticalGender())])
            .arg(age())
            .arg(adjective_flection_singular[static_cast<std::underlying_type_t<Gender>>(termGrammaticalGender())])
            .arg(QString::number(static_cast<float>(size())/100.0f))
            .arg(adjective_flection_singular[static_cast<std::underlying_type_t<Gender>>(termGrammaticalGender())])
            .arg(the_appearance)
            .arg(term())
            .arg(the_haircolor)
            .arg((m_has_beard) ? beard().prepend(QString(" und ")) : "");
    return desc;
}

void Person::randomPerson(const Gender& new_gender)
{
    m_gender = new_gender;
    m_age = m_age_min + QRandomGenerator::global()->bounded(m_age_max-m_age_min+1);
    randomProperties();
    return;
}

void Person::randomPerson(const Gender& new_gender, const AgePeriod& age_period)
{
    m_gender = new_gender;
    switch(age_period) {
        case AgePeriod::CHILD:
            m_age = m_age_min + QRandomGenerator::global()->bounded(m_child_age_limit-m_age_min+1);
            break;
        case AgePeriod::ADULT:
            m_age = m_child_age_limit + 1 + QRandomGenerator::global()->bounded(m_adult_age_limit-m_child_age_limit-1);
            break;
        case AgePeriod::ELDER:
            m_age = m_adult_age_limit + QRandomGenerator::global()->bounded(m_age_max-m_adult_age_limit+1);
            break;
    }
    randomProperties();
    return;
}


/* Private methods ***********************************************************/
void Person::randomProperties()
{
    m_size = m_size_formula.roll();
    m_ind_haircolor = weightedRandomIndex(m_haircolor_weights);
    m_ind_appearance = weightedRandomIndex(m_appearance_weights);
    m_has_beard = ((gender() == Gender::MALE) && (agePeriod() != AgePeriod::CHILD)) ? (static_cast<unsigned int>(QRandomGenerator::global()->bounded(100)) < m_beard_percentage) : false;
    qDebug() << "beard percentage" << m_beard_percentage << "->" << m_has_beard;
    m_ind_beard = weightedRandomIndex(m_beard_weights);
    m_ind_trait = weightedRandomIndex(m_trait_weights);
    m_ind_profession = weightedRandomIndex(m_profession_weights);
    m_ind_special = weightedRandomIndex(m_specials_weights);
    return;
}

size_t Person::weightedRandomIndex(const std::vector<unsigned int> weights)
{
    // Compute sum of weights.
    unsigned int sum_of_weights = 0;
    for (auto& n : weights)
        sum_of_weights += n;
    // Compute a random number.
    int rnd = QRandomGenerator::global()->bounded(sum_of_weights);
    qDebug() << "weightedRandomIndex" << sum_of_weights << rnd;
    for (size_t ind = 0; ind < weights.size(); ind++) {
        if (rnd < static_cast<int>(weights.at(ind)))
            return ind;
        rnd -= weights.at(ind);
    }
    assert(false);
    return 0;
}
