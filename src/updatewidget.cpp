#include "updatewidget.h"

UpdateWidget::UpdateWidget(QWidget *parent) :
    QWidget(parent)
{
    setupUi(this);
    connect(cancelBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(mergeBtn,SIGNAL(clicked()),this,SLOT(goUpdate()));
}

void UpdateWidget::goUpdate()
{
    FileType fileType;
    OrphansBehavior orphans;

    if(mergeCombo->currentIndex() == 0) fileType = FileType::TrFile;
    else if(mergeCombo->currentIndex() == 1) fileType = FileType::SourceCode;

    if(duplicatesCombo->currentIndex() == 0) orphans = OrphansBehavior::KeepAndComment;
    else if(duplicatesCombo->currentIndex() == 2) orphans = OrphansBehavior::Remove;

    emit updateOptionsSaved(fileType, orphans);
}

void UpdateWidget::cancel()
{
    emit canceled();
}
