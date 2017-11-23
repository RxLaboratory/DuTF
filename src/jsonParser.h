#ifndef JSONPARSER_H
#define JSONPARSER_H

#include <QObject>
#include <QThread>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegularExpression>
#include <QJsonDocument>
#include "translation.h"
#include "parser.h"

class JsonParser : public Parser
{
    Q_OBJECT

signals:
    void newTranslation(Translation);
    void languageFound(QStringList);
    void applicationFound(QString);

public slots:
    void parseFile(QFile *file);
    void parseText(QString *jsxinc);

protected:

    void run();

private:

    /**
     * @brief The current text behing parsed
     */
    QString * currentText;

    /**
     * @brief Current file being parsed
     */
    QFile * currentFile;

    /**
     * @brief Parsing mode
     * 0 : nothing
     * 1 : Parse the current file
     * 2 : Parse the current string
     */
    int mode;

    void parseDocument(QJsonDocument);
};

#endif // JSONPARSER_H
