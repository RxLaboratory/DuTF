#include <QString>

namespace utils{

    /**
     * @brief unescape foreslashs of a string
     * @param s		String to unescape
     * @return		The unescaped string
     */
    QString unEscape(QString s);

    /**
     * @brief escaped foreslashes of a string
     * @param s		String to escape
     * @return		The escaped string
     */
    QString escape(QString s);

}
