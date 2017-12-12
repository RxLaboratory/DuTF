#include "mergewidget.h"

MergeWidget::MergeWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(mergeBtn,SIGNAL(clicked()),this,SLOT(goMerge()));
}

void MergeWidget::goMerge()
{
    MergeKind fmerge;
    DuplicateBehavior fduplicate;

    if(mergeCombo->currentIndex() == 0) fmerge = MergeKind::MergeTrFile;
    else if(mergeCombo->currentIndex() == 1) fmerge = MergeKind::MergeSourceCode;

    if(duplicatesCombo->currentIndex() == 0) fduplicate = DuplicateBehavior::Ignore;
    else if(duplicatesCombo->currentIndex() == 1) fduplicate = DuplicateBehavior::NewContext;

    emit mergeOptionsSaved(fmerge, fduplicate);
}

void MergeWidget::cancel()
{
    emit canceled();
}
