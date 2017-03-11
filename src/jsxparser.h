#ifndef JSXPARSER_H
#define JSXPARSER_H

#include <QObject>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>
#include <QRegularExpression>

class JsxParser : public QObject
{
    Q_OBJECT
public:
    explicit JsxParser(QObject *parent = 0);

signals:
    void newTranslation(QStringList tr);
    void languageFound(QStringList lang);
    void parsingFinished();

public slots:
    void parseJsxinc(QFile *file);
    void parseJsxinc(QString *jsxinc);
private:
    void parseJsxinc(QTextStream *jsxinc);
};

#endif // JSXPARSER_H
