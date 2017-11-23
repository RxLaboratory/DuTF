#include "parser.h"

Parser::Parser(QObject *parent) : QThread(parent)
{

}

void Parser::run()
{
    parse(mode_);
    mode_ = ParseUndefined;

}

void Parser::parse(ParsingMode pMode)
{
    if(pMode == ParseFile)
    {
        parseFile(parseVal_path_);
    }
    else
    {
        parseText(parseVal_string_);
    }
}

void Parser::preParseFile(QString path)
{
    mode_ = ParseFile;
    parseVal_path_ = path;
    this->start();
}

void Parser::preParseText(QString  text)
{
    mode_ = ParseString;
    parseVal_string_ = text;
    this->start();
}
