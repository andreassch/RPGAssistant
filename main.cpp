#include "TurnOrderWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    TurnOrderWidget w;
    w.show();
    return a.exec();
}
