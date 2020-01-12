#include "UI/MainWidget.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator app_translator;
    app_translator.load("rpgassistant_" + QLocale::system().name());
    app.installTranslator(&app_translator);

    MainWidget w;
    w.show();
    return app.exec();
}
