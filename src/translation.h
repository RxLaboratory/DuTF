#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QString>
#include <QCoreApplication>

typedef struct Translation
{
    QString source;
    QString translated;
    QString context;
    QString comment;
    int contextId;
} Translation;

Q_DECLARE_METATYPE(Translation)

#endif // TRANSLATION_H
