# Project file and global configuration for the Role Playing Game Assistant.
#
# Copyright (c) 2019, 2020 Andreas Schneider (andreas underscore schn at web dot de)
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License version 3
# as published by the Free Software Foundation.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

VERSION = 0.0.8git

# Please select here for which roleplaying system and fanatasy world to build.
DEFINES += RPG_SYSTEM_TDE_AVENTURIA


# Required Qt modules.
QT       += core gui svg

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

DEFINES += VERSION=\\\"$$VERSION\\\"

SOURCES += \
    Backend/DataXmlReader.cpp \
    Backend/Diary.cpp \
    Backend/DiceFormula.cpp \
    Backend/Holiday.cpp \
    Backend/HolidaysXmlReader.cpp \
    Backend/Names.cpp \
    Backend/NamesXmlReader.cpp \
    Backend/Person.cpp \
    Backend/PersonsXmlReader.cpp \
    Backend/TDECalendar.cpp \
    Backend/TDEModifer.cpp \
    Backend/Utils.cpp \
    UI/AboutWidget.cpp \
    UI/CalendarWidget.cpp \
    UI/MainWidget.cpp \
    UI/TurnOrderWidget.cpp \
    UI/NPCGeneratorWidget.cpp \
    main.cpp

HEADERS += \
    Backend/Calendar.h \
    Backend/DataXmlReader.h \
    Backend/Diary.h \
    Backend/DiceFormula.h \
    Backend/Holiday.h \
    Backend/HolidaysXmlReader.h \
    Backend/Names.h \
    Backend/NamesXmlReader.h \
    Backend/Person.h \
    Backend/PersonsXmlReader.h \
    Backend/TDECalendar.h \
    Backend/TDEModifer.h \
    Backend/Utils.h \
    UI/AboutWidget.h \
    UI/CalendarWidget.h \
    UI/MainWidget.h \
    UI/NPCGeneratorWidget.h \
    UI/TurnOrderWidget.h \
    UI/Utils.h

FORMS += \
    UI/AboutWidget.ui \
    UI/CalendarWidget.ui \
    UI/MainWidget.ui \
    UI/NPCGeneratorWidget.ui \
    UI/TurnOrderWidget.ui

RESOURCES += \
    graphics.qrc
android {
    RESOURCES += logo-mr.qrc
} else {
    RESOURCES += logo-lr.qrc
}
contains(DEFINES, RPG_SYSTEM_TDE_AVENTURIA) {
    exists(tde.pro) { include(tde.pro) } # optional non-free resources
}

LANGUAGES = de

defineReplace(prependAll) {
 for(a,$$1):result += $$2$${a}$$3
 return($$result)
}
TRANSLATIONS = $$prependAll(LANGUAGES, $$PWD/Translations/rpgassistant_, .ts)

TRANSLATIONS_FILES =
qtPrepareTool(LRELEASE, lrelease)
for(tsfile, TRANSLATIONS) {
 qmfile = $$shadowed($$tsfile)
 qmfile ~= s,.ts$,.qm,
 qmdir = $$dirname(qmfile)
 !exists($$qmdir) {
 mkpath($$qmdir)|error("Aborting.")
 }
 command = $$LRELEASE -removeidentical $$tsfile -qm $$qmfile
 system($$command)|error("Failed to run: $$command")
 TRANSLATIONS_FILES += $$qmfile
}


# Setup 'make install' step
QMAKE_TARGET_BUNDLE_PREFIX = com.github.andreassch
android {
    DATA_PATH=/assets/data
    LOCALE_PATH=/assets/translations
} else:macx {
    DATA_PATH=Contents/Resources
    LOCALE_PATH=Contents/Resources
} else {
    DATA_PATH=.
    LOCALE_PATH=.
}
data.path = $$DATA_PATH
data.files = Data/names.xml Data/persons.xml Data/holidays.xml
data.depends += FORCE
locales.path = $$LOCALE_PATH
locales.files = Translations/rpgassistant_de.qm
locales.depends += FORCE

macx {
    ICON = Graphics/rpgassistant.icns
    QMAKE_BUNDLE_DATA += data locales
} else {
    INSTALLS += data locales
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

android {
   DISTFILES += \
      android/AndroidManifest.xml \
      android/res/drawable-ldpi/icon.png \
      android/res/drawable-mdpi/icon.png \
      android/res/drawable-hdpi/icon.png \
      android/res/drawable-xhdpi/icon.png \
      android/res/drawable-xxhdpi/icon.png \
      android/res/drawable-xxxhdpi/icon.png
}

ANDROID_PACKAGE_SOURCE_DIR = $$PWD/android

QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.14
