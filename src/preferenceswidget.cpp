#include "preferenceswidget.h"

PreferencesWidget::PreferencesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);

    //UI config
    styleBox->setItemData(0,":/styles/default");
    styleBox->setItemData(1,"");
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
    }
    else if (index == 1)
    {
        updateCSSButton->setEnabled(false);
        styleBox->setToolTip("Current system style");
    }
    else if (index == 2)
    {
        updateCSSButton->setEnabled(true);
        //get file
        QString cssFileName = QFileDialog::getOpenFileName(this,"Open a stylesheet file","","CSS (*.css);;Text files (*.txt);;All files (*.*)");

        QFile checkFile(cssFileName);
        if (checkFile.exists()) styleBox->setItemData(2,cssFileName);
        styleBox->setToolTip("Your own style");
    }

    emit changeCSS(styleBox->currentData().toString());
}

void PreferencesWidget::on_toolBarStyleBox_currentIndexChanged(int index)
{
    changeToolBarAppearance(index);
}

void PreferencesWidget::on_updateCSSButton_clicked()
{
    emit changeCSS(styleBox->currentData().toString());
}
