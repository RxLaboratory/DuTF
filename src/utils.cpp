#include "utils.h"

#include <QStringList>

namespace utils{
    QString unEscape(QString s)
    {
        s = s.replace("\\n","\n");
        s = s.replace("\\\"","\"");
        return s;
    }

    QString escape(QString s)
    {
        s = s.replace("\n","\\n");
        s = s.replace("\"","\\\"");
        return s;
    }

    QString basename(QString s)
    {
        QStringList pieces = s.split("/");
        return pieces.back();
    }

}
