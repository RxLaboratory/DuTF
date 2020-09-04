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

    if (mergeCombo->currentIndex() == 1)
        fileType = FileType::SourceCode;
    else
        fileType = FileType::TrFile;

    if (duplicatesCombo->currentIndex() == 1)
        orphans = OrphansBehavior::Remove;
    else
        orphans = OrphansBehavior::KeepAndComment;

    emit updateOptionsSaved(fileType, orphans);
}

void UpdateWidget::cancel()
{
    emit canceled();
}
