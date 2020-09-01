#ifndef MERGEWIDGET_H
#define MERGEWIDGET_H

#include "ui_mergeWidget.h"
#include "parser.h"
#include <QWidget>

class MergeWidget : public QWidget, private Ui::MergeWidget
{
    Q_OBJECT
public:
    explicit MergeWidget(QWidget *parent = nullptr);

    /**
     * @brief Merge setting choosen by the user
     */
    enum FileType{
        TrFile,
        SourceCode
    };

    enum DuplicateBehavior{
        Ignore,
        NewContext
    };

signals:

    void mergeOptionsSaved(MergeWidget::FileType, MergeWidget::DuplicateBehavior);
    void canceled();

public slots:

private slots:

    /**
     * @brief Create a flag using user settings and tell the
     * sends a signal with those flags
     */
    void goMerge();

    /**
     * @brief Emit a canceled signal when the user clicks on cancel
     */
    void cancel();
};

#endif
