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
    QString source = "";

    /**
     * @brief Translated text
     */
    QString translated = "";

    /**
     * @brief Context of the translation
     */
    QString context = "";

    /**
     * @brief Commment associated to the translation
     */
    QString comment = "";

    /**
     * @brief A context using an id
     */
    int contextId = 0;

    /**
     * @brief Create a json object with this translation
     * @return
     */
    QJsonObject toJson();

    /**
     * @brief Checks if 2 translation are identical.
     * Checks if source, translated, context and contextId are equal
     * @return True of false
     */
    friend bool operator==(const Translation&, const Translation&);

};

Q_DECLARE_METATYPE(Translation)

#endif // TRANSLATION_H
