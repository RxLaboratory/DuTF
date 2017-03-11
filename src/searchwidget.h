#ifndef SEARCHWIDGET_H
#define SEARCHWIDGET_H

#include "ui_searchwidget.h"

class SearchWidget : public QWidget, private Ui::SearchWidget
{
    Q_OBJECT

public:
    explicit SearchWidget(QWidget *parent = 0);
    bool searchOriginal();
    bool searchTranslated();
    bool searchComment();
    bool caseSensitive();
private slots:
    void on_searchButton_clicked();
    void on_clearButton_clicked();

signals:
    void search(QString);
    void clear();
};

#endif // SEARCHWIDGET_H
