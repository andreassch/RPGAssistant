#pragma once

#include <QWidget>
#include <QTableWidgetItem>
#include <QColor>
//#include <QColorConstants>

namespace TurnOrderTableColumn {
    enum TurnOrderTableColumn
    {
        NAME,
        INI,
        INI_MOD,
        LeP,
        LE,
        MOD
    };
}

QT_BEGIN_NAMESPACE
namespace Ui { class TurnOrderWidget; }
QT_END_NAMESPACE

class TurnOrderWidget : public QWidget
{
    Q_OBJECT

public:
    TurnOrderWidget(QWidget *parent = nullptr);
    ~TurnOrderWidget();

private:
    void setupUi();
    template<class T> QTableWidgetItem* createTableWidgetNumericItem(const T number) const;
    void activateEntry(const int number);
    void deactivateEntry(const int number);
    int activeEntry() const;
    int computeModifierTDE(const int lep, const int le) const;

private slots:
    void onNext();
    void onDamage();
    void onAddEntry();
    void onDeleteEntry();
    void onStart();
    void onChangeEntry(const int row, const int column);
    void onLoad();
    void onSave();
    void onClear();

private:
    Ui::TurnOrderWidget *m_ui;
    int current_ini = 0;
    QColor color_active = QColor("green");//QColorConstants::Green;
};
