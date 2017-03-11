#include "jsxparser.h"

JsxParser::JsxParser(QObject *parent) : QObject(parent)
{

}

void JsxParser::parseJsxinc(QFile *file)
{
    //open file
    file->open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream in(file);

    //parse file
    parseJsxinc(&in);

    //close file
    file->close();
}

void JsxParser::parseJsxinc(QString *jsxinc)
{
    parseJsxinc(new QTextStream(jsxinc,QIODevice::ReadOnly));
}

void JsxParser::parseJsxinc(QTextStream *jsxinc)
{
    //set to UTF8 by default
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    jsxinc->setCodec(codec);
    //line
    QString line;
    QString rxString = "\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\" *, *(\\d*) *, *\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\" *]\\); *(?:\\/\\/(.*))*";


    while (!jsxinc->atEnd())
    {
        qint64 pos = jsxinc->pos();
        line = jsxinc->readLine().trimmed();
        //if not UTF-8, try locale
        if (line.indexOf("�") > 0)
        {
            QTextCodec *codec = QTextCodec::codecForLocale();
            jsxinc->setCodec(codec);
            jsxinc->seek(pos);
            line = jsxinc->readLine().trimmed();
        }
        //get the string and comment
        if (line.startsWith("DutranslatorArray.push("))
        {
            QRegularExpression rx(rxString);
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch())
            {
                QString original = match.captured(1);
                QString context = match.captured(2);
                QString translated = match.captured(3);
                QString comment = match.captured(4);
                QStringList translation;
                translation << original << context << translated << comment;
                emit newTranslation(translation);
            }
        }
        else if (line.startsWith("Dutranslator.languages.push"))
        {
            //get language code
            QString rxString = "'([^\\\\\\*\\.\\s/]+)','([^\\\\\\*\\.\\s/]+)'";
            QRegularExpression rx(rxString);
            QRegularExpressionMatch match = rx.match(line);
            if (match.hasMatch())
            {
                QString langCode = match.captured(1);
                QString lang = match.captured(2);
                QStringList language;
                language << langCode << lang;
                emit languageFound(language);
            }
        }
    }

    emit parsingFinished();
}
