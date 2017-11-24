#ifndef TRANSLATION_H
#define TRANSLATION_H

#include <QString>
#include <QCoreApplication>
#include <QJsonObject>

/**
 * @brief A translation
 */
class Translation
{

public:

    /**
     * @brief Original text
     */
    QString source;

    /**
     * @brief Translated text
     */
    QString translated;

    /**
     * @brief Context of the translation
     */
    QString context;

    /**
     * @brief Commment associated to the translation
     */
    QString comment;

    /**
     * @brief A context using an id
     */
    int contextId;

    /**
     * @brief Create a json object with this translation
     * @return
     */
    QJsonObject toJson();

};

Q_DECLARE_METATYPE(Translation)

#endif // TRANSLATION_H
