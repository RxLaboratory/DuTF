QT       += core gui\
        sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

DEFINES += APPVERSION=\\\"0.6-Beta\\\"

VPATH += ./src

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp \
    parser.cpp \
    jsonParser.cpp \
    searchwidget.cpp \
    src/updatewidget.cpp \
    utils.cpp \
    src/rowbuttonswidget.cpp \
    src/preferenceswidget.cpp \
    src/translation.cpp \
    src/stringparser.cpp \
    src/scriptparsewidget.cpp \
    src/mergewidget.cpp \
    src/commentlineedit.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h \
    parser.h \
    jsonParser.h \
    searchwidget.h \
    src/updatewidget.h \
    utils.h \
    src/rowbuttonswidget.h \
    src/preferenceswidget.h \
    src/translation.h \
    src/stringparser.h \
    src/scriptparsewidget.h \
    src/mergewidget.h \
    src/commentlineedit.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui \
    searchwidget.ui \
    src/rowbuttonswidget.ui \
    src/preferenceswidget.ui \
    src/scriptParseWidget.ui \
    src/mergeWidget.ui \
    src/updatewidget.ui

RESOURCES += resources.qrc

TRANSLATIONS += resources/languages/app_fr.ts \
    resources/languages/app_es.ts \
    resources/languages/app_zh_cn.ts 

# OS Specific configurations
win* {
    RC_ICONS = ./resources/icons/dutr_win.ico
} else:unix {
    # Fix issue with c++ version used to compile Qt in some distros (Ubuntu) with Qt <= 5.12.
    # Need to check the version of c++ used with distros providing Qt > 12
    equals(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 13):QMAKE_CXXFLAGS += "-fno-sized-deallocation"
} else:macx {
    #MAC ICON
    ICON = ./resources/icons/dutr_mac.icns
}
