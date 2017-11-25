#include "stringparser.h"
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>
#include "translation.h"
#include <vector>

void StringParser::parseFile(QString path)
{

    // https://regexr.com/3h8nt
    // tr\(("|')((?:\\\1|(?:(?!\1).))*)\1(?:\)|(?:\,[ ]?(?:(\d+)|(?:("|')((?:\\\4|(?:(?!\4).))*)\4))))
    static QString patternTrOnly = QByteArray::fromBase64(
                "dHJcKCgifCcpKCg/OlxcXDF8KD86KD8hXDEpLikpKilcMSg/OlwpfCg/OlwsWyBdPyg/OihcZCspfCg/OigifCcpKCg/OlxcXDR8KD86KD8hXDQpLikpKilcNCkpKSk=");
    static QRegularExpression regTrOnly(patternTrOnly);

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit parsingFailed(ParsingError::FileOpen);
        return;
    }

    std::vector<Translation> founds;

    QString line;
    while(!file.atEnd())
    {
        line = file.readLine();
        QRegularExpressionMatchIterator i = regTrOnly.globalMatch(line);
        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            Translation t;
            t.source = match.captured(2);

            //qDebug() << match.lastCapturedIndex() << " -> " << match.capturedTexts() << "\n";
            if(match.lastCapturedIndex() == 3) // We have a context id
            {
                t.contextId = match.captured(3).toInt();
            }
            else if(match.lastCapturedIndex() == 5) // We have a context string
            {
                t.context = match.captured(5);
            }
            if(std::find(founds.begin(), founds.end(), t) == founds.end()) // New translation
            {
                founds.push_back(t);
                emit newTranslation(t);
            }

        }

    }

    file.close();
    emit parsingFinished();
}

void StringParser::parseText(QString)
{
   emit parsingFailed(ParsingError::ParsingNotImplemented);
}
