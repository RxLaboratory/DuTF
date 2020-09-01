#ifndef UPDATEWIDGET_H
#define UPDATEWIDGET_H

#include "ui_updatewidget.h"
#include "parser.h"

class UpdateWidget : public QWidget, private Ui::UpdateWidget
{
    Q_OBJECT

public:
    explicit UpdateWidget(QWidget *parent = nullptr);

    /**
     * @brief Update setting choosen by the user
     */
    enum FileType{
        TrFile,
        SourceCode
    };

    enum OrphansBehavior{
        KeepAndComment,
        Remove
    };

signals:

    void updateOptionsSaved(UpdateWidget::FileType, UpdateWidget::OrphansBehavior);
    void canceled();


private slots:

    /**
     * @brief Create a flag using user settings and tell the
     * sends a signal with those flags
     */
    void goUpdate();

    /**
     * @brief Emit a canceled signal when the user clicks on cancel
     */
    void cancel();

};

#endif // UPDATEWIDGET_H
