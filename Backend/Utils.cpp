#include "Utils.h"
#include <QCoreApplication>

QDir Utils::dataDir()
{
#ifdef ANDROID
    QDir data_dir("assets:/data");
#elif defined(Q_OS_MAC)
    QDir data_dir(QCoreApplication::applicationDirPath());
    data_dir.cdUp();
    data_dir.cd("Resources");
#else
    QDir data_dir(QCoreApplication::applicationDirPath()); // TODO: enable configurable path
#endif
    return data_dir;
}
