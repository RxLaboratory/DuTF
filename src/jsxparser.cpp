#include "jsxparser.h"

JsxParser::JsxParser(QObject *parent) :
    QThread(parent)
{

}

void JsxParser::run(){
    if(mode == 1){
        //open file
        currentFile->open(QIODevice::ReadOnly | QIODevice::Text);
        QTextStream in(currentFile);

        //parse file
        parseContent(&in);

        //close file
        currentFile->close();

    }else if(mode == 2){
        parseContent(new QTextStream(currentText,QIODevice::ReadOnly));
    }

    if(mode){
        currentFile = 0;
        currentText = 0;
        mode = 0;
    }
}

void JsxParser::parseFile(QFile *file)
{
    currentFile = file;
    mode = 1;
    start();
}

void JsxParser::parseText(QString *jsxinc)
{
    currentText = jsxinc;
    mode = 2;
    start();
}

void JsxParser::parseContent(QTextStream *jsxinc)
{
    //set to UTF8 by default
    QTextCodec *codec = QTextCodec::codecForName("UTF-8");
    jsxinc->setCodec(codec);
    //line
    QString line;
    QString rxString = "\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\" *, *(\\d*) *, *\\\"([^\\\"\\\\]*(?:\\\\.[^\\\"\\\\]*)*?)\\\" *]\\); *(?:\\/\\/(.*))*";
    int nbLines = 0;
    while(!jsxinc->atEnd()){
        nbLines++;
        jsxinc->readLine();
    }
    if(!jsxinc->seek(0)){
        emit parsingFailed();
        return;
    }


    int lineNb = 1;
    int lastProgress = 0, currentProgress = 0;
    while (!jsxinc->atEnd())
    {

        currentProgress = (lineNb * 1.0 / nbLines * 1.0) * 100;
        lineNb++;
        if(currentProgress > lastProgress + 5){
            emit progress(currentProgress);
            lastProgress = currentProgress;
        }

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
