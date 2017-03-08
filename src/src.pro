QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    languagewidget.cpp \
    aboutdialog.cpp

HEADERS  += mainwindow.h \
    languagewidget.h \
    aboutdialog.h

FORMS    += mainwindow.ui \
    languagewidget.ui \
    aboutdialog.ui

RESOURCES += resources.qrc
