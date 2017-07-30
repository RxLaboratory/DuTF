#include "jsonParser.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QtDebug>

JsonParser::JsonParser(QObject *parent) :
    QThread(parent)
{

}

void JsonParser::run(){
    if(mode == 1){
        //open file
        currentFile->open(QIODevice::ReadOnly);
        QByteArray rawData = currentFile->readAll();
        //close file
        currentFile->close();

        //parse file
        parseDocument(QJsonDocument::fromJson(rawData));


    }else if(mode == 2){
        parseDocument(QJsonDocument::fromJson(currentText->toUtf8()));
    }

    if(mode){
        currentFile = 0;
        currentText = 0;
        mode = 0;
    }
}

void JsonParser::parseFile(QFile *file)
{
    currentFile = file;
    mode = 1;
    start();
}

void JsonParser::parseText(QString *json)
{
    currentText = json;
    mode = 2;
    start();
}

void JsonParser::parseDocument(QJsonDocument doc)
{
    QJsonObject json = doc.object();

    QStringList softs = json.keys();
    QString app = softs[0];
    QJsonObject appTr = json[app].toObject();

    // Look for the language

    QString langCode = appTr["languageId"].toString();
    QString lang = appTr["languageName"].toString();
    QStringList language;
    language << langCode << lang;
    emit languageFound(language);

    // Look for translations


    int i = 0;
    QJsonArray transList = appTr["translations"].toArray();
    QJsonObject trans;
    for(i = 0; i < transList.size(); i++){
        trans = transList.takeAt(i).toObject();
        QString original = trans["source"].toString();
        QString context = QString::number(trans["contextId"].toInt());
        QString translated = trans["translation"].toString();
        QString comment = trans["comment"].toString();
        QStringList translation;
        translation << original << context << translated << comment;
        emit newTranslation(translation);
    }


    emit parsingFinished();
}
