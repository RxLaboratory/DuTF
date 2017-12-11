#include "scriptparsewidget.h"

ScriptParseWidget::ScriptParseWidget(QWidget *parent) : QWidget(parent)
{
    setupUi(this);
    connect(cancelImportBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(cancelExportBtn,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(goImportBtn, SIGNAL(clicked()),this,SLOT(goImport()));
    connect(goExportBtn, SIGNAL(clicked()),this,SLOT(goExport()));

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

void ScriptParseWidget::goExport()
{
    StringParser::TranslationParsingModes flags(0);

    if(replaceSimpleStrings->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseSingleQuote, true);
    if(replaceDoubleStrings->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseDoubleQuotes, true);
    if(replaceIgnoreComments->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::IgnoreStringComment, true);
    flags.setFlag(StringParser::TranslationParsingMode::Export);

    emit exportOptionsSaved(flags);
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
