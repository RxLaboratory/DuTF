#ifndef SCRIPTPARSE_H
#define SCRIPTPARSE_H

#include "ui_scriptParseWidget.h"
#include "stringparser.h"
#include <QWidget>

class ScriptParseWidget : public QWidget, private Ui::ScriptParseWidget
{
    Q_OBJECT
public:
    explicit ScriptParseWidget(QWidget *parent = nullptr);

    /**
     * @brief Mode currently being used
     * ImportMerge means that only options related to import would be shown
     * Export will show only the options related to export of tr inside a script
     */
    enum Mode{
        ImportMerge,
        Export
    };

    /**
     * @brief Set the current mode
     */
    void setMode(ScriptParseWidget::Mode);

signals:

    void importOptionsSaved(StringParser::TranslationParsingModes);
    void canceled();

public slots:

private slots:

    /**
     * @brief Create a flag using user settings and tell the
     * sends a signal with those flags
     */
    void goImport();

    /**
     * @brief Emit a canceled signal when the user clicks on cancel
     */
    void cancel();
};

#endif
