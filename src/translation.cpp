#include "translation.h"


QJsonObject Translation::toJson()
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

