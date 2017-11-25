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
        ParseTR = 0x0001,
        ParseSingleQuote = 0x0010,
        ParseDoubleQuotes = 0x0100,
        IgnoreStringComment = 0x1000
    };
    Q_DECLARE_FLAGS(TranslationParsingModes, TranslationParsingMode)

    /**
     * @brief Set the mode that will be used when parsing the file
     */
    void setMode(StringParser::TranslationParsingModes);

signals:

    /**
     * @brief Emitted when a new translation has been parsed
     */
    void newTranslation(Translation);

protected:

    void parseFile(QString);

    void parseText(QString);

    StringParser::TranslationParsingModes translationMode_;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(StringParser::TranslationParsingModes)
Q_DECLARE_METATYPE(StringParser::TranslationParsingModes)

#endif // STRINGPARSER_H
