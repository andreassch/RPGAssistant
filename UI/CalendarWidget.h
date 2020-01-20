#pragma once

#include <QWidget>
#include <QTableWidgetItem>

namespace Month {
    enum Month {
        PRAIOS = 1,
        RONDRA = 2,
        EFFERD = 3,
        TRAVIA = 4,
        BORON = 5,
        HESINDE = 6,
        FIRUN = 7,
        TSA = 8,
        PHEX = 9,
        PERAINE = 10,
        INGERIMM = 11,
        RAHJA = 12,
        NAMENLOS = 13
    };
}

namespace Ui {
class CalendarWidget;
}

class CalendarWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CalendarWidget(QWidget *parent = nullptr);
    ~CalendarWidget();

    int day() const;
    int month() const;
    int year() const;
    int yearHal() const;
    int daysInMonth(const int month) const;

private:
    void setupUi();
    int dayBasis(const int day, const int month, const int year_hal);
    int dayOfWeek(const int day_basis);
    int moonPhase(const int day_basis);
    int dayOfWeek(const int day, const int month, const int year_hal);
    int moonPhase(const int day, const int month, const int year_halt);
    void fillMonth(const int month, const int year_hal);

private slots:
    void onChangeMonth(const int index);
    void onChangeYear(const int year);
    void onChangeDay();

private:
    Ui::CalendarWidget *m_ui;
    QString m_moon_phase_texts[4] = {tr("waxing"), tr("full"), tr("waning"), tr("new")};
};
