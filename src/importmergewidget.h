#ifndef IMPORTMERGEWIDGET_H
#define IMPORTMERGEWIDGET_H

#include "ui_importMergeWidget.h"
#include "stringparser.h"
#include <QWidget>

class ImportMergeWidget : public QWidget, private Ui::ImportMergeWidget
{
    Q_OBJECT
public:
    explicit ImportMergeWidget(QWidget *parent = nullptr);

signals:

    void optionsSaved(StringParser::TranslationParsingModes);
    void canceled();

public slots:

private slots:

    /**
     * @brief Create a flag using user settings and tell the
     * sends a signal with those flags
     */
    void go();

    /**
     * @brief Emit a canceled signal when the user clicks on cancel
     */
    void cancel();
};

#endif // IMPORTMERGEWIDGET_H
