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

QDebug operator<<(QDebug  q, const Translation & t)
{
    q << "Translation(" << t.source << " -> " <<
         t.translated << ", context[" <<
         t.contextId << ", " <<
         t.context << "], " <<
         t.comment << ")";
    return q;
}
