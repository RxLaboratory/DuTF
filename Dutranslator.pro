QT       += core gui\
        sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

VPATH += ./src

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp \
    jsxparser.cpp \
    jsonParser.cpp \
    searchwidget.cpp \
    utils.cpp \
    src/rowbuttonswidget.cpp \
    src/preferenceswidget.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h \
    jsonParser.h \
    jsxparser.h \
    searchwidget.h \
    utils.h \
    src/rowbuttonswidget.h \
    src/preferenceswidget.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui \
    searchwidget.ui \
    src/rowbuttonswidget.ui \
    src/preferenceswidget.ui

RESOURCES += resources.qrc

TRANSLATIONS += resources/languages/app_fr.ts \
    resources/languages/app_es.ts \
    resources/languages/app_zh_cn.ts 

#WIN ICON
RC_ICONS = ./resources/icons/dutr_win.ico
#MAC ICON
ICON = ./resources/icons/dutr_mac.icns

