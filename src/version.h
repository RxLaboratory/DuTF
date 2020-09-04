#ifndef VERSION_H
#define VERSION_H

#define VERSION_MAJOR 1
#define VERSION_MINOR 0
#define VERSION_BUILD 1
#define VERSION_SUFFIX "Beta"

#define STRINGIFY_VERSION(A, B, C) CONCAT(A, B, C )
#define CONCAT(A, B, C ) STRINGIFY( A##.##B##.##C )
#define STRINGIFY(A) #A

#define STR_VERSION STRINGIFY_VERSION(VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD) "-" VERSION_SUFFIX

#define VER_FILEVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_FILEVERSION STR_VERSION

// Keep the product version as fixed
#define VER_PRODUCTVERSION VERSION_MAJOR,VERSION_MINOR,VERSION_BUILD,0
#define STR_PRODUCTVERSION STR_VERSION

#define STR_COMPANYNAME "Rainbox Laboratory"
#define STR_FILEDESCRIPTION "DuTranslator"
#define STR_INTERNALNAME "DuTranslator"
#define STR_LEGALCOPYRIGHT "Copyright (c) 2017-2020 Rainbox Laboratory, Nicolas Dufresne and contributors"
#define STR_LEGALTRADEMARKS1 "All Rights Reserved"
#define STR_ORIGINALFILENAME "DuTranslator.exe"
#define STR_PRODUCTNAME "DuTranslator"

#define STR_COMPANYDOMAIN "rainboxlab.org"

#define URL_CHAT "htts://chat.rainboxlab.org"
#define URL_FORUM "https://forum.rainboxlab.org"
#define URL_DOC "https://dutranslator-docs.rainboxlab.org"
#define URL_BUGREPORT "https://github.com/Rainbox-dev/Dutranslator/issues"

#endif // VERSION_H
