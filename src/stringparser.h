#ifndef STRINGPARSER_H
#define STRINGPARSER_H

#include "parser.h"
#include "translation.h"

/**
 * @brief Use this parser to extract string and tr calls from a file
 */
class StringParser : public Parser
{
    Q_OBJECT

public:

    enum TranslationParsingMode {
        ParseTR = 0x0,
        ParseSingleQuote = 0x1,
        ParseDoubleQuotes = 0x2,
        IgnoreStringComment = 0x3
    };
    Q_DECLARE_FLAGS(TranslationParsingModes, TranslationParsingMode)

signals:

    /**
     * @brief Emitted when a new translation has been parsed
     */
    void newTranslation(Translation);

protected:

    void parseFile(QString);

    void parseText(QString);

};

Q_DECLARE_OPERATORS_FOR_FLAGS(StringParser::TranslationParsingModes)

#endif // STRINGPARSER_H
