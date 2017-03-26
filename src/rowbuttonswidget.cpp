#include "rowbuttonswidget.h"

RowButtonsWidget::RowButtonsWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
}

void RowButtonsWidget::on_removeButton_clicked()
{
    emit removeRow();
}

void RowButtonsWidget::on_addButton_clicked()
{
    emit addRow();
}
