#include "importmergewidget.h"

ImportMergeWidget::ImportMergeWidget(QWidget *parent) : QWidget(parent)
{

    setupUi(this);
    connect(cancelButton,SIGNAL(clicked()),this,SLOT(cancel()));
    connect(goButton,SIGNAL(clicked()),this,SLOT(go()));

}

void ImportMergeWidget::go()
{
    StringParser::TranslationParsingModes flags(0);

    if(parseTrCalls->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseTR, true);
    if(parseSimpleQuotes->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseSingleQuote, true);
    if(parseDoubleQuotes->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::ParseDoubleQuotes, true);
    if(ignoreComments->isChecked()) flags.setFlag(StringParser::TranslationParsingMode::IgnoreStringComment, true);

    emit optionsSaved(flags);
}

void ImportMergeWidget::cancel()
{
    emit canceled();
}
