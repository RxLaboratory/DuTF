#include "preferenceswidget.h"
#include <QtDebug>
#include <QApplication>
#include <QTranslator>
#include <QMessageBox>
#include "mainwindow.h"

PreferencesWidget::PreferencesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    //UI config
    styleBox->setItemData(0,":/styles/default");
    styleBox->setItemData(1,"");

    currentLanguage_ = -1;

    // Language
    QString appLanguage = settings_.value("dutranslator/language").toString();
    if(appLanguage == "/lang/fr")
        languageBox->setCurrentIndex(1);
    else if(appLanguage == ":/lang/es")
        languageBox->setCurrentIndex(2);
    else if(appLanguage == ":/lang/zh")
        languageBox->setCurrentIndex(3);

    //get preferences
    QString cssFile = settings_.value("dutranslator/css", "").toString();
    int toolBar = settings_.value("dutranslator/toolBar", 0).toInt();

    if (cssFile == "default")
    {
        styleBox->setCurrentIndex(1);
    }
    else if (cssFile == ":/styles/default" || cssFile == "")
    {
        styleBox->setCurrentIndex(0);
    }
    else
    {
        styleBox->setCurrentIndex(2);
        styleBox->setItemData(2,cssFile);
    }


    toolBarStyleBox->setCurrentIndex(toolBar);
}

void PreferencesWidget::on_backButton_clicked()
{
    emit hidePreferences();
}

void PreferencesWidget::on_styleBox_currentIndexChanged(int index)
{
    if (index == 0)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Dutranslator default style");
        //update prefs database
        settings_.setValue("dutranslator/css", ":/styles/default");
    }
    else if (index == 1)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Current system style");
        //update prefs database
        settings_.setValue("dutranslator/css", "default");

    }
    else if (index == 2)
    {
        updateCSSButton->setEnabled(true);
        //get file
        QString cssFileName = QFileDialog::getOpenFileName(this,"Open a stylesheet file","","CSS (*.css);;Text files (*.txt);;All files (*.*)");

        QFile checkFile(cssFileName);
        if (checkFile.exists()) styleBox->setItemData(2,cssFileName);
        styleBox->setToolTip("Your own style");
        //update prefs database
        settings_.setValue("dutranslator/css", cssFileName);
    }

    emit changeCSS(styleBox->currentData().toString());
}

void PreferencesWidget::on_toolBarStyleBox_currentIndexChanged(int index)
{
    emit changeToolBarAppearance(index);
    //update prefs database
    settings_.setValue("dutranslator/toolBar", index);
}

void PreferencesWidget::on_updateCSSButton_clicked()
{
    emit changeCSS(styleBox->currentData().toString());
}

QString PreferencesWidget::getCSS()
{
    return styleBox->currentData().toString();
}

int PreferencesWidget::getToolBarStyle()
{
    return toolBarStyleBox->currentIndex();
}

void PreferencesWidget::on_languageBox_currentIndexChanged(int index)
{

    if(index == currentLanguage_) return;
    currentLanguage_ = index;

    QString lang = "default";
    switch (index) {
    case 1:
        lang = ":/lang/fr";
        break;
    case 2:
        lang = ":/lang/es";
        break;
    case 3:
        lang = ":/lang/zh";
        break;
    default:
        break;
    }

    if(settings_.value("dutranslator/language").toString() == lang) return;

    settings_.setValue("dutranslator/language", lang);

    QMessageBox mb(QMessageBox::Information,
                   tr("Dutranslator"), tr("You will need to restart the application to load the new language."),
                   QMessageBox::Ok,
                   this,
                   Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint | Qt::FramelessWindowHint);
    mb.exec();
}
