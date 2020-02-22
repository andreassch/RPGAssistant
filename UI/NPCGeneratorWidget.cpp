#include "NPCGeneratorWidget.h"
#include "ui_NPCGeneratorWidget.h"

#include "../Backend/NamesXmlReader.h"
#include "../Backend/PersonsXmlReader.h"
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

    // Read name lists and fill name region combo box.
    QFile names_file(data_dir.filePath("names.xml"));
    qDebug() << names_file;
    if(!names_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("NPC Generator"), tr("Cannot open file %1 for reading: %2").arg(names_file.fileName()).arg(names_file.errorString()));
        m_ui->comboNameRegion->setEnabled(false);
        m_ui->buttonGenerate->setEnabled(false);
    }
    else {
        NamesXmlReader name_reader(&m_namelists);
        name_reader.read(&names_file);
        names_file.close();
        qDebug() << "Name list contains" << m_namelists.size() << "regions.";
        for (std::size_t ind=0;ind < m_namelists.size(); ind++)
            m_ui->comboNameRegion->addItem(m_namelists.at(ind).region());
    }

    // Read persons.
    QFile persons_file(data_dir.filePath("persons.xml"));
    qDebug() << persons_file;
    if(!persons_file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::critical(this, tr("NPC Generator"), tr("Cannot open file %1 for reading: %2").arg(persons_file.fileName()).arg(persons_file.errorString()));
        m_ui->comboRegion->setEnabled(false);
        m_ui->checkRandomRegion->setEnabled(false);
    }
    else {
        PersonsXmlReader person_reader(&m_persons);
        person_reader.read(&persons_file);
        persons_file.close();
        qDebug() << "Person list contains" << m_persons.size() << "regions.";
        for (std::size_t ind=0;ind < m_persons.size(); ind++)
            m_ui->comboRegion->addItem(m_persons.at(ind).region());
    }

    // Fill gender combo box.
    for (std::underlying_type_t<Gender> gval = 0; gval <= static_cast<std::underlying_type_t<Gender>>(Gender::MALE); gval++)
        m_ui->comboGender->addItem(Names::genderString(static_cast<Gender>(gval)));

    // Fill age combo box.
    for (std::underlying_type_t<AgePeriod> aval = 0; aval <= static_cast<std::underlying_type_t<AgePeriod>>(AgePeriod::ELDER); aval++)
        m_ui->comboAge->addItem(Person::agePeriodString(static_cast<AgePeriod>(aval)));

    // Connect signals and slots.
    connect(m_ui->checkRandomRegion, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomRegion);
    connect(m_ui->checkNameRegion, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeCheckSeparateNameRegion);
    connect(m_ui->checkRandomGender, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomGender);
    connect(m_ui->checkRandomAge, &QCheckBox::stateChanged, this, &NPCGeneratorWidget::onChangeRandomAge);
    connect(m_ui->buttonGenerate, &QPushButton::clicked, this, &NPCGeneratorWidget::onGenerate);

    // Load settings.
    loadSettings();

    return;
}

void NPCGeneratorWidget::saveSettings()
{
    QSettings settings;
    // Save region status.
    if (m_ui->comboRegion->count() > 0) {
        settings.setValue("NPCGenerator/region", m_ui->comboRegion->currentText());
        settings.setValue("NPCGenerator/randomRegion", m_ui->checkRandomRegion->isChecked());
    }
    // Save name region status.
    if (m_ui->comboNameRegion->count() > 0) {
        settings.setValue("NPCGenerator/nameRegion", m_ui->comboNameRegion->currentText());
        settings.setValue("NPCGenerator/nameRegionSeparate", m_ui->checkNameRegion->isChecked());
    }
    // Save gender status.
    settings.setValue("NPCGenerator/gender", m_ui->comboGender->currentIndex());
    settings.setValue("NPCGenerator/randomGender", m_ui->checkRandomGender->isChecked());
    // Save age status.
    settings.setValue("NPCGenerator/agePeriod", m_ui->comboGender->currentIndex());
    settings.setValue("NPCGenerator/randomAgePeriod", m_ui->checkRandomGender->isChecked());
    settings.sync();
    qDebug() << "NPCGenerator::saveSettings";
    return;
}

void NPCGeneratorWidget::loadSettings()
{
    QSettings settings;
    // Load region status.
    if (m_ui->comboRegion->count() > 0) {
        QString region_name = settings.value("NPCGenerator/region", m_ui->comboRegion->itemText(0)).toString();
        int ind_region = searchRegion(region_name);
        if (ind_region != -1)
            m_ui->comboRegion->setCurrentIndex(ind_region);
    }
    // Load name region status.
    m_ui->checkNameRegion->setChecked(settings.value("NPCGenerator/nameRegionSeparate", false).toBool());
    if (m_ui->comboNameRegion->count() > 0) {
        QString region_name = settings.value("NPCGenerator/nameRegion", m_ui->comboNameRegion->itemText(0)).toString();
        int ind_region = searchNameRegion(region_name);
        if (ind_region != -1)
            m_ui->comboNameRegion->setCurrentIndex(ind_region);
    }
    // Load gender status.
    m_ui->checkRandomRegion->setChecked(settings.value("NPCGenerator/randomRegion", false).toBool());
    int ind_gender = settings.value("NPCGenerator/gender", 0).toInt();
    if ((ind_gender >= 0) && (ind_gender < m_ui->comboGender->count()))
        m_ui->comboGender->setCurrentIndex(ind_gender);
    m_ui->checkRandomGender->setChecked(settings.value("NPCGenerator/randomGender", false).toBool());
    // Load age status.
    int ind_age_period = settings.value("NPCGenerator/agePeriod", 0).toInt();
    if ((ind_age_period >= 0) && (ind_age_period < m_ui->comboAge->count()))
        m_ui->comboAge->setCurrentIndex(ind_age_period);
    m_ui->checkRandomAge->setChecked(settings.value("NPCGenerator/randomAgePeriod", false).toBool());
    return;
}

int NPCGeneratorWidget::searchRegion(const QString& region_name)
{
    for (size_t ind = 0; ind < m_persons.size(); ind++) {
        if (m_persons.at(ind).region().compare(region_name) == 0)
            return static_cast<int>(ind);
    }
    return -1;
}

int NPCGeneratorWidget::searchNameRegion(const QString& region_name)
{
    for (size_t ind = 0; ind < m_namelists.size(); ind++) {
        if (m_namelists.at(ind).region().compare(region_name) == 0)
            return static_cast<int>(ind);
    }
    return -1;
}


/* Private slots *************************************************************/
void NPCGeneratorWidget::onChangeRandomRegion(const int state)
{
    if (m_ui->checkRandomRegion->isChecked())
        m_ui->comboRegion->setEnabled(false);
    else
        m_ui->comboRegion->setEnabled(true);
    return;
}

void NPCGeneratorWidget::onChangeCheckSeparateNameRegion(const int state)
{
    if (m_ui->checkNameRegion->isChecked())
        m_ui->comboNameRegion->setEnabled(true);
    else
        m_ui->comboNameRegion->setEnabled(false);
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

void NPCGeneratorWidget::onChangeRandomAge(const int state)
{
    if (m_ui->checkRandomAge->isChecked())
        m_ui->comboAge->setEnabled(false);
    else
        m_ui->comboAge->setEnabled(true);
    return;
}

void NPCGeneratorWidget::onGenerate()
{
    size_t ind_region;
    if (m_ui->checkRandomRegion->isChecked())
        ind_region = QRandomGenerator::global()->bounded(static_cast<int>(m_persons.size()));
    else
        ind_region = m_ui->comboRegion->currentIndex();
    int ind_gender;
    if (m_ui->checkRandomGender->isChecked())
        ind_gender = QRandomGenerator::global()->bounded(2);
    else
        ind_gender = m_ui->comboGender->currentIndex();
    Gender gender = static_cast<Gender>(ind_gender);
    size_t ind_name_region;
    if (m_ui->checkNameRegion->isChecked())
        ind_name_region = m_ui->comboNameRegion->currentIndex();
    else
        ind_name_region = searchNameRegion(m_ui->comboRegion->currentText());
    QString name = m_namelists.at(ind_name_region).randomName(gender);
    Person person = m_persons.at(ind_region);
    if (m_ui->checkRandomAge->isChecked())
        person.randomPerson(gender);
    else
        person.randomPerson(gender, static_cast<AgePeriod>(m_ui->comboAge->currentIndex()));
    QString desc = QString("<B>%1</B><BR>%2").arg(name).arg(person.description());
    desc.append(tr("<BR>Trait: %1").arg(person.trait()));
    desc.append(tr("<BR>Profession: %1").arg(person.profession()));
    m_ui->textDescription->setHtml(desc);
#ifdef ANDROID
    saveSettings();
#endif
    return;
}
