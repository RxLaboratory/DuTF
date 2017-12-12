#include "commentlineedit.h"

CommentLineEdit::CommentLineEdit(QWidget *parent) : QLineEdit(parent)
{
   connect(this, SIGNAL(textChanged(QString)), this, SLOT(updateColor()));
}

void CommentLineEdit::updateColor()
{
    static QString styleNew = "background-color: rgb(115, 115, 115)";
    if(text().startsWith("NEW")) setStyleSheet(styleNew);
    else setStyleSheet("");

}
