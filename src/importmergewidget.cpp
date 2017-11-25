#include "importmergewidget.h"

ImportMergeWidget::ImportMergeWidget(QWidget *parent) : QWidget(parent)
{

    setupUi(this);
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(goButton,SIGNAL(clicked()),this,SLOT(go()));

}

void ImportMergeWidget::go()
{
    StringParser::TranslationParsingModes flags;

    if(parseTrCalls->isChecked()) flags |= StringParser::TranslationParsingMode::ParseTR;
    if(parseSimpleQuotes->isChecked()) flags |= StringParser::TranslationParsingMode::ParseSingleQuote;
    if(parseSimpleQuotes->isChecked()) flags |= StringParser::TranslationParsingMode::ParseDoubleQuotes;
    if(ignoreComments->isChecked()) flags |= StringParser::TranslationParsingMode::IgnoreStringComment;

    emit optionsSaved(flags);
}

void ImportMergeWidget::cancel()
{
    emit canceled();
}
