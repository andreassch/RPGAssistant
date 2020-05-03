#include "UI/MainWidget.h"
#include "Backend/Utils.h"

#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QCoreApplication::setOrganizationName("andreassch");
    QCoreApplication::setOrganizationDomain("andreassch.github.com");
    QCoreApplication::setApplicationName("rpgassistant");
    QCoreApplication::setApplicationVersion(VERSION);

    QTranslator app_translator;
#ifdef ANDROID
    QDir translation_dir("assets:/translations");
#elif defined(Q_OS_MAC)
    QDir translation_dir = Utils::dataDir();
#else
    QDir translation_dir = QDir("Translations");
#endif
    QString locale_name = translation_dir.filePath("rpgassistant_" + QLocale::system().name());
    app_translator.load(locale_name);
    app.installTranslator(&app_translator);

    MainWidget w;
    w.show();
    return app.exec();
}
