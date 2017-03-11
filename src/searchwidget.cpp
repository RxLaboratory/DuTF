#include "searchwidget.h"

SearchWidget::SearchWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void SearchWidget::on_searchButton_clicked()
{
    emit search(searchEdit->text());
}

bool SearchWidget::searchOriginal()
{
    return originalBox->isChecked();
}

bool SearchWidget::searchTranslated()
{
    return translatedBox->isChecked();
}

bool SearchWidget::searchComment()
{
    return commentBox->isChecked();
}

bool SearchWidget::caseSensitive()
{
    return caseBox->isChecked();
}

void SearchWidget::on_clearButton_clicked()
{
    emit clear();
}
