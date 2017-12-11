#ifndef PARSER_H
#define PARSER_H

#include <QThread>
#include <QObject>
#include <QFile>
#include <QFlag>

/**
 * @brief The ParseMode enum
 */
enum ParsingMode{
    ParseFile,
    ParseString,
    ParseStream,
    ParseUndefined
};


/**
 * @brief A Parser of any kind
 */
class Parser : public QThread
{
    Q_OBJECT
public:

    /**
     * @brief Create a parser. Does nothing special.
     * @param parent    The parent of the thread.
     */
    explicit Parser(QObject *parent = nullptr);

    enum ParsingError {
        FileOpen = 0x001,
        ParsingNotImplemented = 0x010,
        ReadingFailed = 0x100
    };
    Q_DECLARE_FLAGS(ParsingErrors, ParsingError)

signals:

    /**
     * @brief Emited when an export operations has been completed
     */
    void exportFinished();

    /**
     * @brief Signal emited when the parsing is over
     */
    void parsingFinished();

    /**
     * @brief Signal emited when the parsing has failed
     */
    void parsingFailed(Parser::ParsingErrors);

    /**
     * @brief The parsing process has stepped forward.
     * Value goes from 0 to 100.
     */
    void progress(int);

public slots:

    /**
     * @brief Start the parsing process
     */
    void run();

    /**
     * @brief Prepare the thread to parse a file
     * @param QString    The path to the file to parse
     */
    void preParseFile(QString);

    /**
     * @brief Prepare the thread to parse a file
     */
    void preParseText(QString);


protected:

    /**
     * @brief Actual parsinc process, different for each parser
     * @param   The mode of parsing
     * @param   The value to be used for parsing
     */
    virtual void parse(ParsingMode);

    virtual void parseFile(QString) = 0;

    virtual void parseText(QString) = 0;

    /**
     * @brief The current mode of the parser
     */
    ParsingMode mode_ = ParseUndefined;

    /**
     * @brief The value that will be parse if that's a file
     */
    QString parseVal_path_;

    /**
     * @brief The value that will be parse if that's a string
     */
    QString parseVal_string_;

};

Q_DECLARE_OPERATORS_FOR_FLAGS(Parser::ParsingErrors)
Q_DECLARE_METATYPE(Parser::ParsingErrors)

#endif // PARSER_H
