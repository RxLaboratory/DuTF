#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
#include <QLocale>
#include <iostream>
#include <QString>
#include "translation.h"
#include "stringparser.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QCoreApplication::setOrganizationName("Rainbox");
    QCoreApplication::setOrganizationDomain("rainboxprod.coop");
    QCoreApplication::setApplicationName("DuTranslator");
    QCoreApplication::setApplicationVersion(APPVERSION);

    QSettings settings;

    qRegisterMetaType<Translation>();
    qRegisterMetaType<Parser::ParsingErrors>();
    qRegisterMetaType<StringParser::TranslationParsingModes>();

    QTranslator tr;

    // Check user local language
    QString appLanguage = settings.value("dutranslator/language", "").toString();

    if(appLanguage == "") // First run
    {
        QString locale = QLocale::system().name();
        if(locale.startsWith("fr"))
            settings.setValue("dutranslator/language",":/lang/fr");
        else if(locale.startsWith("es"))
            settings.setValue("dutranslator/language", ":/lang/es");
        else if(locale.startsWith("zh"))
            settings.setValue("dutranslator/language", ":/lang/zh");
        else
            settings.setValue("dutranslator/language", "default");
        appLanguage = settings.value("dutranslator/language").toString();
    }

    if(appLanguage != "default" && appLanguage != "")
    {
        tr.load(appLanguage);
        a.installTranslator(&tr);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
