#ifndef JSONPARSER_H
#define JSONPARSER_H

#include "translation.h"
#include "parser.h"

#include <QByteArray>

/**
 * @brief Parser for a translation file
 */
class JsonParser : public Parser
{
    Q_OBJECT

signals:
    void newTranslation(Translation);
    void languageFound(QStringList);
    void applicationFound(QString);


protected:

    void parseFile(QString);

    void parseText(QString);

private:

    /**
     * @brief Parse the content of the document
     */
    void parseDocument(QByteArray);
};

#endif // JSONPARSER_H
