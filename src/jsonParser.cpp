#include "jsonParser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFile>

void JsonParser::parseFile(QString path)
{
    QFile file(path);
    if(!file.open(QIODevice::ReadOnly))
    {
        emit parsingFailed(Parser::ParsingError::FileOpen);
        return;
    }
    QByteArray rawData = file.readAll();
    file.close();

    parseDocument(rawData);
}

void JsonParser::parseText(QString pText)
{
    parseDocument(pText.toUtf8());
}

void JsonParser::parseDocument(QByteArray raw)
{
    QJsonDocument doc = QJsonDocument::fromJson(raw);
    QJsonObject json = doc.object();


    QStringList softs = json.keys();
    QString app = softs[0];

    QJsonArray appTr = json[app].toArray();
    emit applicationFound(app);

    // Look for the language

    QJsonObject details = appTr[0].toObject();

    QString langCode = details["languageId"].toString();
    QString lang = details["languageName"].toString();
    QStringList language;
    language << langCode << lang;
    emit languageFound(language);

    // Look for translations

    int i = 0;
    QJsonArray transList = appTr[1].toObject()["translations"].toArray();
    float incProgress = 100 / float(transList.size());
    for(i = 0; i < transList.size(); i++){
        QJsonObject trans = transList.at(i).toObject();

        Translation newTr;
        newTr.source = trans["source"].toString();
        newTr.translated = trans["translation"].toString();
        newTr.context = trans["context"].toString();
        newTr.comment = trans["comment"].toString();
        newTr.contextId = trans["contextId"].toInt();
        emit newTranslation(newTr);
        emit progress(i * incProgress);
    }
    emit progress(100);

    emit parsingFinished();
}
