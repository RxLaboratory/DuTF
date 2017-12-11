QT       += core gui\
        sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

VPATH += ./src

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp \
    parser.cpp \
    jsonParser.cpp \
    searchwidget.cpp \
    utils.cpp \
    src/rowbuttonswidget.cpp \
    src/preferenceswidget.cpp \
    src/translation.cpp \
    src/stringparser.cpp \
    src/scriptparsewidget.cpp \
    src/mergewidget.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h \
    parser.h \
    jsonParser.h \
    searchwidget.h \
    utils.h \
    src/rowbuttonswidget.h \
    src/preferenceswidget.h \
    src/translation.h \
    src/stringparser.h \
    src/scriptparsewidget.h \
    src/mergewidget.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui \
    searchwidget.ui \
    src/rowbuttonswidget.ui \
    src/preferenceswidget.ui \
    src/scriptParseWidget.ui \
    src/mergeWidget.ui

RESOURCES += resources.qrc

TRANSLATIONS += resources/languages/app_fr.ts \
    resources/languages/app_es.ts \
    resources/languages/app_zh_cn.ts 

#WIN ICON
RC_ICONS = ./resources/icons/dutr_win.ico
#MAC ICON
ICON = ./resources/icons/dutr_mac.icns

