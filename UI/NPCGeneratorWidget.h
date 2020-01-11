#pragma once

#include <QWidget>
#include "Backend/Names.h"
#include "Backend/Species.h"

namespace Ui {
class NPCGeneratorWidget;
}

class NPCGeneratorWidget : public QWidget
{
    Q_OBJECT

public:
    /**
     * @brief Constructor
     * @param parent
     */
    explicit NPCGeneratorWidget(QWidget *parent = nullptr);

    /**
     * Destructor
     */
    ~NPCGeneratorWidget();

private:
    /**
     * @brief Setup the user interface.
     * This private function is called by the constructor.
     */
    void setupUi();

    /**
     * @brief Loads names and races from XML data files.
     */
    void loadData();

private slots:
    /**
     * @brief Callback for the generate button.
     */
    void onGenerate();

    /**
     * @brief Callback when the species is changed, which updates the races available for the species.
     */
    void onChangeSpecies(const int index);

private:
    Ui::NPCGeneratorWidget* m_ui;
    std::vector<Names> m_namelists;
    std::vector<Species> m_species;
};
