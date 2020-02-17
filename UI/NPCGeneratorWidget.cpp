#include "NPCGeneratorWidget.h"
#include "ui_NPCGeneratorWidget.h"

#include "../Backend/NamesXmlReader.h"
#include "../Backend/RacesXmlReader.h"
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSettings>
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
    saveSettings();
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
        m_ui->comboRegion->setEnabled(false);
        m_ui->checkRandomRegion->setEnabled(false);
        m_ui->buttonGenerate->setEnabled(false);
    }
    else {
        NamesXmlReader name_reader(&m_namelists);
        name_reader.read(&names_file);
        names_file.close();
        qDebug() << "Name list contains" << m_namelists.size() << "regions.";
        for (std::size_t ind=0;ind < m_namelists.size(); ind++)
            m_ui->comboRegion->addItem(m_namelists.at(ind).region());
    }

    // Read species and their races and fill the combo boxes.
    QFile races_file(data_dir.filePath("races.xml"));
    qDebug() << races_file;
    if(!races_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("NPC Generator"), tr("Cannot open file %1 for reading: %2").arg(races_file.fileName()).arg(races_file.errorString()));
        m_ui->comboSpecies->setEnabled(false);
        m_ui->checkRandomSpecies->setEnabled(false);
        m_ui->comboRace->setEnabled(false);
        m_ui->checkRandomRace->setEnabled(false);
        m_ui->buttonGenerate->setEnabled(false);
    }
    else {
        RacesXmlReader race_reader(&m_species);
        race_reader.read(&races_file);
        races_file.close();
        qDebug() << "Species list contains" << m_species.size() << "species.";
        for (std::size_t ind=0;ind < m_species.size(); ind++)
            m_ui->comboSpecies->addItem(m_species.at(ind).name());
        if (m_species.size() > 0)
            onChangeSpecies(0);
    }

    // Fill gender combo box.
    for (std::underlying_type_t<Gender> gval = 0; gval <= static_cast<std::underlying_type_t<Gender>>(Gender::MALE); gval++)
        m_ui->comboGender->addItem(Names::genderString(static_cast<Gender>(gval)));

    // Connect signals and slots.
    connect(m_ui->comboSpecies, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &NPCGeneratorWidget::onChangeSpecies);
    connect(m_ui->checkRandomRegion, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomRegion);
    connect(m_ui->checkRandomSpecies, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomSpecies);
    connect(m_ui->checkRandomRace, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomRace);
    connect(m_ui->checkRandomGender, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomGender);
    connect(m_ui->buttonGenerate, &QPushButton::clicked, this, &NPCGeneratorWidget::onGenerate);

    // Load settings.
    loadSettings();

    return;
}

void NPCGeneratorWidget::saveSettings()
{
    QSettings settings;
    if (m_ui->comboRegion->count() > 0) {
        settings.setValue("NPCGenerator/region", m_ui->comboRegion->currentIndex());
        settings.setValue("NPCGenerator/randomRegion", m_ui->checkRandomRegion->isChecked());
    }
    if (m_ui->comboSpecies->count() > 0) {
        settings.setValue("NPCGenerator/species", m_ui->comboSpecies->currentIndex());
        settings.setValue("NPCGenerator/randomSpecies", m_ui->checkRandomSpecies->isChecked());
    }
    if (m_ui->comboRace->count() > 0) {
        settings.setValue("NPCGenerator/race", m_ui->comboRace->currentIndex());
        settings.setValue("NPCGenerator/randomRace", m_ui->checkRandomRace->isChecked());
    }
    settings.setValue("NPCGenerator/gender", m_ui->comboGender->currentIndex());
    settings.setValue("NPCGenerator/randomGender", m_ui->checkRandomGender->isChecked());
    settings.sync();
    qDebug() << "NPCGenerator::saveSettings";
    return;
}

void NPCGeneratorWidget::loadSettings()
{
    QSettings settings;
    int ind_region = settings.value("NPCGenerator/region", 0).toInt();
    if ((ind_region >= 0) && (ind_region < m_ui->comboRegion->count()))
        m_ui->comboRegion->setCurrentIndex(ind_region);
    int ind_species = settings.value("NPCGenerator/species", 0).toInt();
    if ((ind_species >= 0) && (ind_species < m_ui->comboSpecies->count()))
        m_ui->comboSpecies->setCurrentIndex(ind_species);
    int ind_race = settings.value("NPCGenerator/race", 0).toInt();
    if ((ind_race >= 0) && (ind_race < m_ui->comboRace->count()))
        m_ui->comboRace->setCurrentIndex(ind_race);
    int ind_gender = settings.value("NPCGenerator/gender", 0).toInt();
    if ((ind_gender >= 0) && (ind_gender <= m_ui->comboGender->count()))
        m_ui->comboGender->setCurrentIndex(ind_gender);
    qDebug() << "NPCGeneratorWidget::loadSettings" << ind_region << ind_species << ind_race << ind_gender;
    m_ui->checkRandomRegion->setChecked(settings.value("NPCGenerator/randomRegion", false).toBool());
    m_ui->checkRandomSpecies->setChecked(settings.value("NPCGenerator/randomSpecies", false).toBool());
    m_ui->checkRandomRace->setChecked(settings.value("NPCGenerator/randomRace", false).toBool());
    m_ui->checkRandomGender->setChecked(settings.value("NPCGenerator/randomGender", false).toBool());
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

void NPCGeneratorWidget::onChangeRandomRegion(const int state)
{
    if (m_ui->checkRandomRegion->isChecked())
        m_ui->comboRegion->setEnabled(false);
    else
        m_ui->comboRegion->setEnabled(true);
    return;
}

void NPCGeneratorWidget::onChangeRandomSpecies(const int state)
{
    if (m_ui->checkRandomSpecies->isChecked()) {
        m_ui->comboSpecies->setEnabled(false);
        if (!m_ui->checkRandomRace->isChecked())
            m_ui->checkRandomRace->setChecked(true);
    }
    else
        m_ui->comboSpecies->setEnabled(true);
    return;
}

void NPCGeneratorWidget::onChangeRandomRace(const int state)
{
    if (m_ui->checkRandomRace->isChecked())
        m_ui->comboRace->setEnabled(false);
    else {
        m_ui->comboRace->setEnabled(true);
        if (m_ui->checkRandomSpecies->isChecked())
            m_ui->checkRandomSpecies->setChecked(false);
    }
    return;
}

void NPCGeneratorWidget::onChangeRandomGender(const int state)
{
    if (m_ui->checkRandomGender->isChecked())
        m_ui->comboGender->setEnabled(false);
    else
        m_ui->comboGender->setEnabled(true);
    return;
}

void NPCGeneratorWidget::onGenerate()
{
    int ind_region;
    if (m_ui->checkRandomRegion->isChecked())
        ind_region = QRandomGenerator::global()->bounded(static_cast<int>(m_namelists.size()));
    else
        ind_region = m_ui->comboRegion->currentIndex();
    int ind_gender;
    if (m_ui->checkRandomGender->isChecked())
        ind_gender = QRandomGenerator::global()->bounded(2);
    else
        ind_gender = m_ui->comboGender->currentIndex();
    qDebug() << "Gender" << ind_gender;
    Gender gender = static_cast<Gender>(ind_gender);
    qDebug() << "Name region" << ind_region << m_namelists.at(ind_region).region();
    qDebug() << m_namelists.at(ind_region).surnamesCount() << m_namelists.at(ind_region).firstnamesCount(Gender::FEMALE) << m_namelists.at(ind_region).firstnamesCount(Gender::MALE);
    m_ui->lineName->setText(m_namelists.at(ind_region).randomName(gender));
    m_ui->lineGender->setText(Names::genderString(gender));
    int ind_species;
    if (m_ui->checkRandomSpecies->isChecked())
        ind_species = QRandomGenerator::global()->bounded(static_cast<int>(m_species.size()));
    else
        ind_species = m_ui->comboSpecies->currentIndex();
    int ind_race;
    if (m_ui->checkRandomRace->isChecked())
        ind_race = QRandomGenerator::global()->bounded(static_cast<int>(m_species.at(ind_species).raceCount()));
    else
        ind_race = m_ui->comboRace->currentIndex();
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
#ifdef ANDROID
    saveSettings();
#endif
    return;
}
