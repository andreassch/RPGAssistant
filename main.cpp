#include "UI/MainWidget.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator appTranslator;
    appTranslator.load("rpgassistant_" + QLocale::system().name());
    app.installTranslator(&appTranslator);

    MainWidget w;
    w.show();
    return app.exec();
}
