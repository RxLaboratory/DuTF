#include "aboutdialog.h"

AboutDialog::AboutDialog(QWidget *parent) :
    QDialog(parent)
{
    setupUi(this);
    this->setWindowFlags(Qt::FramelessWindowHint);
}
