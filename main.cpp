#include "UI/MainWidget.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QTranslator app_translator;
#ifdef ANDROID
    QString locale_name = "assets:/translations/rpgassistant_" + QLocale::system().name();
#else
    QString locale_name = "rpgassistant_" + QLocale::system().name();
#endif
    app_translator.load(locale_name);
    app.installTranslator(&app_translator);

    MainWidget w;
    w.show();
    return app.exec();
}
