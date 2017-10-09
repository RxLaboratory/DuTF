#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <iostream>
#include <QString>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTranslator tr;

    // Check user local language
    QString locale = QLocale::system().name();
    if (locale == "fr_FR")
    {
        tr.load(":/lang/fr");
    }

    a.installTranslator(&tr);

    MainWindow w;
    w.show();

    return a.exec();
}
