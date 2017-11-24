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

bool operator==(const Translation &a, const Translation &b)
{
   return a.source == b.source && a.context == b.context && a.contextId == b.contextId;
}

