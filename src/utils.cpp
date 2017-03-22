#include "utils.h"

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
}
