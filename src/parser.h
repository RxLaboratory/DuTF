#ifndef PARSER_H
#define PARSER_H

#include <QThread>
#include <QObject>

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

signals:

    /**
     * @brief Signal emited when the parsing is over
     */
    void parsingFinished();

    /**
     * @brief Signal emited when the parsing has failed
     */
    void parsingFailed();

    /**
     * @brief The parsing process has stepped forward.
     * Value goes from 0 to 100.
     */
    void progress(int);

public slots:

    /**
     * @brief Start the parsing process
     */
    virtual void run() = 0;

};

#endif // PARSER_H
