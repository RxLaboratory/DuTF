#-------------------------------------------------
#
# Project created by QtCreator 2017-01-02T16:34:55
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Dutranslator
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

RESOURCES += \
    resources.qrc
