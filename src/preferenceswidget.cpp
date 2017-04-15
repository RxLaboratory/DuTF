#include "preferenceswidget.h"
#include <QtDebug>

PreferencesWidget::PreferencesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    //UI config
    styleBox->setItemData(0,":/styles/default");
    styleBox->setItemData(1,"");

    //SQLite database
    db = QSqlDatabase::addDatabase("QSQLITE");

    //check if the file already exists, if not, extract it from resources
    QFile dbFile("prefs.s3db");
    if (!dbFile.exists())
    {
        QFile dbResource(":/misc/prefs");
        dbResource.copy("prefs.s3db");
        QFile::setPermissions("prefs.s3db",QFileDevice::ReadUser | QFileDevice::WriteUser | QFileDevice::ReadGroup | QFileDevice::WriteGroup | QFileDevice::ReadOther | QFileDevice::WriteOther);
    }

    //open database
    db.setDatabaseName("prefs.s3db");
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

    //TODO update UI after loading (need a call from MainWindow)
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
        qDebug() << q.lastError();
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
    qDebug() << q.lastError();
}

void PreferencesWidget::on_updateCSSButton_clicked()
{
    emit changeCSS(styleBox->currentData().toString());
}
