#include "NPCGeneratorWidget.h"
#include "ui_NPCGeneratorWidget.h"

#include "../Backend/NamesXmlReader.h"
#include "../Backend/RacesXmlReader.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QtDebug>

/* Public constructors/desctructors ******************************************/
NPCGeneratorWidget::NPCGeneratorWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::NPCGeneratorWidget)
{
    m_ui->setupUi(this);
    setupUi();
}

NPCGeneratorWidget::~NPCGeneratorWidget()
{
    delete m_ui;
}

/* Private methods ***********************************************************/
void NPCGeneratorWidget::setupUi()
{
    // Get data path.
#ifdef ANDROID
    QDir data_dir("assets:/data");
#else
    QDir data_dir = QDir("."); // TODO: don't hardcode path
#endif
    qDebug() << data_dir;
    // Read name lists and fill region combo box.
    QFile names_file(data_dir.filePath("names.xml"));
    qDebug() << names_file;
    if(!names_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("NPC Generator"), tr("Cannot open file %1 for reading: %2").arg(names_file.fileName()).arg(names_file.errorString()));
        return;
    }
    NamesXmlReader name_reader(&m_namelists);
    name_reader.read(&names_file);
    names_file.close();
    qDebug() << "Name list contains" << m_namelists.size() << "regions.";
    for (std::size_t ind=0;ind < m_namelists.size(); ind++)
        m_ui->comboRegion->addItem(m_namelists.at(ind).region());

    // Read species and their races and fill the combo boxes.
    QFile races_file(data_dir.filePath("races.xml"));
    qDebug() << races_file;
    if(!races_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("NPC Generator"), tr("Cannot open file %1 for reading: %2").arg(races_file.fileName()).arg(races_file.errorString()));
        return;
    }
    RacesXmlReader race_reader(&m_species);
    race_reader.read(&races_file);
    races_file.close();
    qDebug() << "Species list contains" << m_species.size() << "species.";
    for (std::size_t ind=0;ind < m_species.size(); ind++)
        m_ui->comboSpecies->addItem(m_species.at(ind).name());
    if (m_species.size() > 0)
        onChangeSpecies(0);

    connect(m_ui->comboSpecies, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NPCGeneratorWidget::onChangeSpecies);
    connect(m_ui->buttonGenerate, &QPushButton::clicked, this, &NPCGeneratorWidget::onGenerate);

    return;
}

/* Private slots *************************************************************/
void NPCGeneratorWidget::onChangeSpecies(const int index)
{
    qDebug() << "onChangeSpecies" << m_ui->comboSpecies->currentIndex() << index;
    m_ui->comboRace->clear();
    Species species = m_species.at(index);
    for (int ind=0; ind < species.raceCount(); ind++) {
        m_ui->comboRace->addItem(species.race(ind).name());
    }
    return;
}

void NPCGeneratorWidget::onGenerate()
{
    int ind_region = m_ui->comboRegion->currentIndex();
    Gender gender = (m_ui->comboGender->currentIndex() == 0) ? Gender::FEMALE : Gender::MALE;
    qDebug() << "Name region" << ind_region << m_namelists.at(ind_region).region();
    qDebug() << m_namelists.at(ind_region).surnamesCount() << m_namelists.at(ind_region).firstnamesCount(Gender::FEMALE) << m_namelists.at(ind_region).firstnamesCount(Gender::MALE);
    m_ui->lineName->setText(m_namelists.at(ind_region).randomName(gender));
    int ind_species = m_ui->comboSpecies->currentIndex();
    int ind_race = m_ui->comboRace->currentIndex();
    Species species = m_species.at(ind_species);
    qDebug() << "Species" << ind_species << species.name();
    Race race = species.race(ind_race);
    qDebug() << "Race" << ind_race << race.name();
    QString haircolor = race.haircolor();
    QString eyecolor = race.eyecolor();
    m_ui->lineHairColor->setText(haircolor);
    m_ui->lineEyeColor->setText(eyecolor);
    int size = race.size();
    int weight = race.weight();
    m_ui->lineSize->setText(QString::number(size));
    m_ui->lineWeight->setText(QString::number(weight));
    return;
}
