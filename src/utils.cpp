#include "utils.h"

#include <QStringList>
#include <QFileInfo>

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

    QString fileName(const QString & s)
    {
        QFileInfo info(s);
        return info.fileName();
    }

}
