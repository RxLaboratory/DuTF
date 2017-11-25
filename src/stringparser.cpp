#include "stringparser.h"
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>
#include "translation.h"
#include <vector>

void StringParser::setMode(StringParser::TranslationParsingModes flags)
{
    translationMode_ = flags;
}

void StringParser::parseFile(QString path)
{

    // https://regexr.com/3h8qq
    // (?:("|')((?:\\\1|(?:(?!\1).))*)\1)|(?:tr\(("|')((?:\\\3|(?:(?!\3).))*)\3(?:\)|(?:\,[ ]?(?:(\d+)|(?:("|')((?:\\\6|(?:(?!\6).))*)\6)))))
    static QString pattern = QByteArray::fromBase64(
"KD86KCJ8JykoKD86XFxcMXwoPzooPyFcMSkuKSkqKVwxKXwoPzp0clwoKCJ8JykoKD86XFxcM3woPzooPyFcMykuKSkqKVwzKD86XCl8KD86XCxbIF0/KD86KFxkKyl8KD86KCJ8JykoKD86XFxcNnwoPzooPyFcNikuKSkqKVw2KSkpKSk=");
    static QRegularExpression reg(pattern);

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit parsingFailed(ParsingError::FileOpen);
        return;
    }

    std::vector<Translation> founds;
    QString line;
    int lineNumber = 1;
#ifdef QT_DEBUG
    qDebug() << "Parse tr : " << translationMode_.testFlag(TranslationParsingMode::ParseTR) << "\n";
    qDebug() << "Parse single : " << translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote) << "\n";
    qDebug() << "Parse double : " << translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes) << "\n";
    qDebug() << "Ignore comments : " << translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment) << "\n";
#endif

    bool inComment = false;
    while(!file.atEnd())
    {
        line = file.readLine();
        QRegularExpressionMatchIterator i = reg.globalMatch(line);
#ifdef QT_DEBUG
        qDebug() << "Line " << lineNumber++ << "\n";
#endif
        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            int captLength = match.lastCapturedIndex() + 1;
#ifdef QT_DEBUG
            for(int i = 0; i < captLength; i++)
            {
                QString cap = match.captured(i);
                qDebug() << " CAPTURE AT " << i << " IS " << cap << "\n";
            }
#endif
            Translation t;
            if(captLength == 3) // String only
            {
                if(inComment && translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment))
                    continue; // Ignore the string
                if(match.captured(1) == "\"" && !translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes))
                    continue; // Ignore double quote strings
                if(match.captured(1) == "'" && !translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote))
                    continue; // Ignore single quote strings
                t.source = match.captured(2);
            }
            else
            {
                if(!translationMode_.testFlag(TranslationParsingMode::ParseTR)) continue; // Ignore tr

                t.source = match.captured(4);
                if(captLength == 5) // TR with no arg
                {
                    // Nothing to do
                }
                else if(captLength == 6) // TR with id
                {
                    t.contextId = match.captured(5).toInt();
                }
                else if(captLength == 8) // TR with context string
                {
                    t.context = match.captured(7);
                }

            }

#ifdef QT_DEBUG
            qDebug() << "TR = " << t << "\n";
#endif


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
