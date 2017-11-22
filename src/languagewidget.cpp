#include "languagewidget.h"

LanguageWidget::LanguageWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void LanguageWidget::setLanguage(QString l)
{
    languageEdit->setText(l);
}

void LanguageWidget::setCode(QString c)
{
    codeEdit->setText(c);
}

void LanguageWidget::setApp(QString a)
{
   appEdit->setText(a);
}

void LanguageWidget::setFile(QString f)
{
    fileLabel->setText(f);
}

QString LanguageWidget::getLanguage()
{
    return languageEdit->text();
}

QString LanguageWidget::getCode()
{
    return codeEdit->text();
}

QString LanguageWidget::getApp()
{
   return appEdit->text();
}
