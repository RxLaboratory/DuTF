#include "preferenceswidget.h"

PreferencesWidget::PreferencesWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void PreferencesWidget::on_backButton_clicked()
{
    emit hidePreferences();
}

void PreferencesWidget::on_styleBox_currentIndexChanged(int index)
{
    if (index == 2)
    {
        updateCSSButton->setEnabled(true);
    }
    else
    {
        updateCSSButton->setEnabled(false);
    }
}

void PreferencesWidget::on_toolBarStyleBox_currentIndexChanged(int index)
{
    changeToolBarAppearance(index);
}
