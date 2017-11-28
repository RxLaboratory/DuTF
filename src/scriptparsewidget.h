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

#endif
