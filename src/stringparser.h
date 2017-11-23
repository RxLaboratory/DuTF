#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include "parser.h"

/**
 * @brief Use this parser to extract string and tr calls from a file
 */
class StringParser : public Parser
{
    Q_OBJECT

protected:

    void parseFile(QString);

    void parseText(QString);

};

#endif // STRINGPARSER_H
