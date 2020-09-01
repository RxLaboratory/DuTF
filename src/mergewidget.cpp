#include "mergewidget.h"

MergeWidget::MergeWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(mergeBtn,SIGNAL(clicked()),this,SLOT(goMerge()));
}

void MergeWidget::goMerge()
{
    FileType fmerge;
    DuplicateBehavior fduplicate;

    if(mergeCombo->currentIndex() == 0) fmerge = FileType::TrFile;
    else if(mergeCombo->currentIndex() == 1) fmerge = FileType::SourceCode;

    if(duplicatesCombo->currentIndex() == 0) fduplicate = DuplicateBehavior::Ignore;
    else if(duplicatesCombo->currentIndex() == 2) fduplicate = DuplicateBehavior::NewContext;

    emit mergeOptionsSaved(fmerge, fduplicate);
}

void MergeWidget::cancel()
{
    emit canceled();
}
