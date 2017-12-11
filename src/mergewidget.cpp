#include "mergewidget.h"

MergeWidget::MergeWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(mergeBtn,SIGNAL(clicked()),this,SLOT(goMerge()));
}

void MergeWidget::goMerge()
{
    if(mergeCombo->currentIndex() == 0) emit mergeOptionsSaved(MergeKind::MergeTrFile);
    else if(mergeCombo->currentIndex() == 1) emit mergeOptionsSaved(MergeKind::MergeSourceCode);
}

void MergeWidget::cancel()
{
    emit canceled();
}
