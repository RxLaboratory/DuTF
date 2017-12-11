#include "stringparser.h"
#include <QByteArray>
#include <QDebug>
#include <QRegularExpression>
#include "translation.h"
#include <vector>

void StringParser::setExportFileName(const QString & pfile)
{
   exportFileName = pfile;
}

void StringParser::setMode(StringParser::TranslationParsingModes flags)
{
    translationMode_ = flags;
}

void StringParser::setTranslations(std::vector<Translation> * plist)
{
   translations = plist;
}

void StringParser::parseFile(QString path)
{

    // https://regexr.com/3h8qq
    // (?:("|')((?:\\\1|(?:(?!\1).))*)\1)|(?:tr\(("|')((?:\\\3|(?:(?!\3).))*)\3(?:\)|(?:\,[ ]?(?:(\d+)|(?:("|')((?:\\\6|(?:(?!\6).))*)\6)))))
    static QString pattern = QByteArray::fromBase64("KD86KCJ8JykoKD86XFxcMXwoPzooPyFcMSkuKSkqKVwxKXwoPzp0clwoKCJ8JykoKD86XFxcM3woPzooPyFcMykuKSkqKVwzKD86XCl8KD86XCxbIF0/KD86KFxkKyl8KD86KCJ8JykoKD86XFxcNnwoPzooPyFcNikuKSkqKVw2KSkpKSk=");
    static QRegularExpression reg(pattern);

    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit parsingFailed(ParsingError::FileOpen);
        return;
    }

    int countLines = 0;
    while(!file.atEnd()){
        countLines++;
        file.readLine();
    }
    if(!file.seek(0)){
        file.close();
        emit parsingFailed(ParsingError::ReadingFailed); // Return to start failed
        return;
    }

    bool doExport = translationMode_.testFlag(TranslationParsingMode::Export);
    QFile exportFile(exportFileName);
    QTextStream * out;
    if(doExport){ // Export file
        if(!exportFile.open(QIODevice::WriteOnly))
        {
            file.close();
            emit parsingFailed(ParsingError::FileOpen);
            return;
        }
        out = new QTextStream(&exportFile);
    }

    std::vector<Translation> founds;
    bool isInlineComment = false;
    bool isMultilineComment = false;
    QString line;
    float incProgress = 100 / float(countLines);
    int lineNumber = 1;

#ifdef QT_DEBUG
    qDebug() << "Parse tr : " << translationMode_.testFlag(TranslationParsingMode::ParseTR) << "\n";
    qDebug() << "Parse single : " << translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote) << "\n";
    qDebug() << "Parse double : " << translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes) << "\n";
    qDebug() << "Ignore comments : " << translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment) << "\n";
#endif

    while(!file.atEnd())
    {
        line = file.readLine();
        int inLineIndex = line.indexOf("//");

        if(!isMultilineComment)
            isMultilineComment = line.startsWith("/*");
        if(isMultilineComment)
            isMultilineComment = !line.endsWith("*/");

        QRegularExpressionMatchIterator i = reg.globalMatch(line);

#ifdef QT_DEBUG
        qDebug() << "Line " << lineNumber++ << "\n";
#endif
        isInlineComment = false;
        int lastStartPos = 0;

        while(i.hasNext())
        {
            QRegularExpressionMatch match = i.next();
            if(doExport)
                *out << line.mid(
                            lastStartPos,
                            match.capturedStart() - lastStartPos
                            ); // The gaps between current and last match

            lastStartPos = match.capturedStart();
            int endPos = match.capturedEnd();

            if(match.lastCapturedIndex() == 2 && translationMode_.testFlag(TranslationParsingMode::IgnoreStringComment)){
                // Check inline for comment
                if(!isInlineComment && inLineIndex >= 0 && match.capturedStart(0) > inLineIndex) isInlineComment = true;
                if(isInlineComment || isMultilineComment)
                {
                    continue; // Ignore the string
                }
            }

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
                t.source = match.captured(2);
                if(doExport && (
                        (match.captured(1) == "\"" && translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes)) ||
                        (match.captured(1) == "'" && translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote))) &&
                        std::find((*translations).begin(), (*translations).end(), t) != (*translations).end())
                {
                    *out << "tr(" << match.captured(0) << ")";
                }
                else if(doExport)
                {
                    *out << match.captured(0);
                }


                if(match.captured(1) == "\"" && !translationMode_.testFlag(TranslationParsingMode::ParseDoubleQuotes))
                {
                    lastStartPos = endPos;
                    continue; // Ignore double quote strings
                }
                if(match.captured(1) == "'" && !translationMode_.testFlag(TranslationParsingMode::ParseSingleQuote))
                {
                    lastStartPos = endPos;
                    continue; // Ignore single quote strings
                }
                t.source = match.captured(2);
                if(t.source == '"' || t.source == "'") // "'"
                {
                    lastStartPos = endPos;
                    continue;
                }
            }
            else
            {
                if(doExport) // Dont need to add tr here
                {
                    *out << line.mid(
                                lastStartPos,
                                match.capturedLength()
                                );
                }

                if(!translationMode_.testFlag(TranslationParsingMode::ParseTR))
                {
                    lastStartPos = endPos;
                    continue; // Ignore tr
                }

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


            if(!doExport && std::find(founds.begin(), founds.end(), t) == founds.end()) // New translation
            {
                founds.push_back(t);
                emit newTranslation(t);
            }
            lastStartPos = endPos;
        }

        if(doExport)
        {
            *out << line.mid(lastStartPos);
        }

        emit progress(lineNumber * incProgress);
    }

    file.close();
    emit progress(100);
    if(doExport)
    {
        // Close export file
        exportFile.close();
        emit exportFinished();
    }
    else
    {
        emit parsingFinished();
    }

}

void StringParser::parseText(QString)
{
    emit parsingFailed(ParsingError::ParsingNotImplemented);
}
