#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QTranslator tr;
    tr.load(":/lang/fr");
    a.installTranslator(&tr);

    MainWindow w;
    w.show();

    return a.exec();
}
