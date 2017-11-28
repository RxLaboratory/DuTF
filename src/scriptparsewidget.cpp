#include "scriptparsewidget.h"

ScriptParseWidget::ScriptParseWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(cancelImportBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(cancelExportBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(goImportBtn, SIGNAL(clicked()),this,SLOT(goImport()));

    setMode(Mode::ImportMerge);
}

void ScriptParseWidget::setMode(ScriptParseWidget::Mode pMode)
{
    switch (pMode) {
    case ScriptParseWidget::Mode::ImportMerge:
        importWidget->show();
        exportWidget->hide();
        break;
    case ScriptParseWidget::Mode::Export:
        exportWidget->show();
        importWidget->hide();
        break;
    default:
        break;
    }

}

void ScriptParseWidget::goImport()
{
    StringParser::TranslationParsingModes flags(0);

    if(parseTrCalls->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseTR, true);
    if(parseSimpleQuotes->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseSingleQuote, true);
    if(parseDoubleQuotes->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseDoubleQuotes, true);
    if(ignoreComments->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::IgnoreStringComment, true);

    emit importOptionsSaved(flags);
}

void ScriptParseWidget::cancel()
{
    emit canceled();
}
