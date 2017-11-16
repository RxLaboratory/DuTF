#include "preferenceswidget.h"
#include <QtDebug>
#include <QApplication>
#include <QTranslator>
#include "mainwindow.h"

PreferencesWidget::PreferencesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    //UI config
    styleBox->setItemData(0,":/styles/default");
    styleBox->setItemData(1,"");

    currentLanguage_ = -1;

    //SQLite database
    db = QSqlDatabase::addDatabase("QSQLITE");

    //check if the file already exists, if not, extract it from resources
    QString prefsPath = "";
#ifdef Q_OS_MAC
    prefsPath = QDir::homePath() + "/Dutranslator/prefs.s3db";
#else
    prefsPath = "prefs.s3db";
#endif

    QFile dbFile(prefsPath);

    if (!dbFile.exists())
    {
        QFile dbResource(":/misc/prefs");
        //on mac, we can not write inside the app, so create folder at home
#ifdef Q_OS_MAC
        QDir home = QDir::home();
        home.mkdir("Dutranslator");
#endif
        //copy the default file from the resources
        dbResource.copy(prefsPath);
        QFile::setPermissions(prefsPath,QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);
    }

    //open database
    db.setDatabaseName(prefsPath);
    db.open();

    //get preferences
    QSqlQuery q("SELECT css,toolBar FROM preferences WHERE user='default' ;");
    // TODO fix "not positioned on a valid record" ???
    q.next();
    QString cssFile = q.value(0).toString();
    int toolBar = q.value(1).toInt();
    if (cssFile == "")
    {
        styleBox->setCurrentIndex(1);
    }
    else if (cssFile == ":/styles/default")
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
        QSqlQuery("UPDATE preferences SET css = ':/styles/default' WHERE user = 'default' ;");
    }
    else if (index == 1)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Current system style");
        //update prefs database
        QSqlQuery q("UPDATE preferences SET css = '' WHERE user = 'default' ;");
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
        QSqlQuery("UPDATE preferences SET css = \"" + cssFileName + "\" WHERE user = 'default' ;");
    }

    emit changeCSS(styleBox->currentData().toString());
}

void PreferencesWidget::on_toolBarStyleBox_currentIndexChanged(int index)
{
    emit changeToolBarAppearance(index);
    //update prefs database
    QSqlQuery q("UPDATE preferences SET toolBar = " + QString::number(index) + " WHERE user = 'default' ;");
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
    QTranslator tr;

    switch (index) {
    case 0:
        break;
    case 1:
        tr.load(":/lang/fr");
        break;
    case 2:
        tr.load(":/lang/es");
        break;
    case 3:
        tr.load(":/lang/zh");
        break;
    default:
        break;
    }

    QApplication::instance()->installTranslator(&tr);
    retranslateUi(&(MainWindow::instance()));
}

