QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++1z

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Backend/DataXmlReader.cpp \
    Backend/DiceFormula.cpp \
    Backend/Names.cpp \
    Backend/Race.cpp \
    Backend/RacesXmlReader.cpp \
    UI/MainWidget.cpp \
    UI/TurnOrderWidget.cpp \
    UI/NPCGeneratorWidget.cpp \
    Backend/NamesXmlReader.cpp \
    main.cpp

HEADERS += \
    Backend/DataXmlReader.h \
    Backend/DiceFormula.h \
    Backend/Names.h \
    Backend/Race.h \
    Backend/RacesXmlReader.h \
    Backend/Species.h \
    UI/MainWidget.h \
    UI/NPCGeneratorWidget.h \
    UI/TurnOrderWidget.h \
    Backend/NamesXmlReader.h

FORMS += \
    UI/MainWidget.ui \
    UI/NPCGeneratorWidget.ui \
    UI/TurnOrderWidget.ui

TRANSLATIONS += Translations/rpgassistant_de.ts

# Setup 'make install' step
android {
    DATA_PATH=/assets/data
} else {
    DATA_PATH=.
}
data.path = $$DATA_PATH
data.files = Data/names.xml Data/races.xml
data.depends += FORCE

INSTALLS += data

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
   DISTFILES += \
      android/AndroidManifest.xml
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android
