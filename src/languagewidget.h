#ifndef LANGUAGEWIDGET_H
#define LANGUAGEWIDGET_H

#include "ui_languagewidget.h"

class LanguageWidget : public QWidget, private Ui::LanguageWidget
{
    Q_OBJECT

public:
    explicit LanguageWidget(QWidget *parent = 0);
    void setLanguage(QString l);
    void setCode(QString c);
    void setApp(QString a);
    void setFile(QString f);
    QString getLanguage();
    QString getCode();
    QString getApp();
};

#endif // LANGUAGEWIDGET_H
