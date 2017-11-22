#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QString>
#include <QCoreApplication>
#include <QJsonObject>

typedef struct Translation
{
    QString source;
    QString translated;
    QString context;
    QString comment;
    int contextId;

    QJsonObject toJson()
    {
        return QJsonObject
        {
            {"source", source},
            {"translation", translated},
            {"context", context},
            {"comment", comment},
            {"contextId", contextId}
        };
    }

} Translation;

Q_DECLARE_METATYPE(Translation)

#endif // TRANSLATION_H
