# DuTranslatorLib
# Translator Framework Library
# Copyright (c) 2017,2018 Nicolas Dufresne, Rainbox Productions
# https://rainboxprod.coop
#
# _Contributors:_
# Nicolas Dufresne - Lead developer
#
# This file is part of DuAEF.
#
# DuAEF is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# DuAEF is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with DuAEF. If not, see <http://www.gnu.org/licenses/>.

import json, os, re
from . import settings, utils

current_language_id = ""
"""The current language id (fr, en, ..)."""

current_language_name = ""
"""The current language name."""

languages = {}
"""A dict containing available languages.
There is a language name and a file name for each language id.
languages["fr_FR"] ["name"] is "Francais" for example.
Will be filled when executing getAvailable()."""

localized_strings = ()
"""The translated strings  of the current language.
An array of compounds containing the source, the translation and the context."""

def get_available():
    """
    Load the list of available languages.
    If the language id and or the language name can't be found in the file, the file name will be used
    to determine the language id and the name will be set as the id.
    Returns: A success code
        0	Success
        1	One of the file haven't been correctly opened
    """

    global languages
    languages = {}
    # Add original language
    languages[settings.original_language_id] = {"name": settings.original_language_name, "file": None}

    # Languages are all files we can find in the folder containing translations
    dir_list = os.listdir(settings.folder)

    for file_name in dir_list:

        if not file_name.startswith(settings.prefix) or not file_name.endswith(settings.suffix):
            continue

        lang_id = lang_name = ""

        file_path = utils.build_path((
            settings.folder,
            file_name
        ))

        with open( file_path , 'r') as file:
            lang_name = ""
            lang_id = ""
            line = " "

            # Use a regular expression to check for the Language Id and Language Name keys
            language_id_re = re.compile( '\\s*"languageId"\\s*:\\s*"(.{2,})"' )
            language_name_re = re.compile( '\\s*"languageName"\\s*:\\s*"(.{2,})"' )

            while (lang_name == "") or (lang_id == "") and line != "":
                line = file.readline()

                # Check id only if it is not found yet (improves perf)
                if (lang_id == ""):
                    match = re.match( language_id_re, line )
                    if match:
                        lang_id = match.group(1)
                        continue

                # Check name only if it is not found yet (improves perf)
                if (lang_name == ""):
                    match = re.match( language_name_re, line )
                    if match:
                        lang_name = match.group(1)
                        continue

        if lang_id == "":
            lang_id = file_name.replace(settings.prefix, "").replace(settings.suffix, "")
        if lang_name == "":
            lang_name = lang_id

        if lang_id == "":
            continue

        languages[lang_id] = {"name": lang_name, "file": file_path}


def get_pretty_name(lang_id):
    """
    Returns the pretty name of a given language.
    Args:
        lang_id: (str) The id of the request language

    Returns:

    """
    if not lang_id in languages:
        return ""
    return languages[lang_id]["name"]


def get_language_id(pretty_name):
    """
    Returns the language id of a given language name
    Args:
        pretty_name: (str) The pretty name of the request language

    Returns:

    """
    for lang_id in languages:
        if languages[lang_id]["name"] == pretty_name:
            return lang_id
    return ""


def get_pretty_names():
    """
    Returns a list containing pretty names of all languages
    """
    res = []
    for lang_id in languages:
        res.append(languages[lang_id]["name"])

    # Order the list by name
    res = sorted(res)
    return res


def set_language(language_id):
    """
    Set the current language
    Args:
        language_id: (str) The id of the language to set.

    Returns:
        success code
        0	Everything went ok
        1	The file linked to the given id can't be opened
        2	The json content doesn't match a translation file
    """

    global localized_strings
    global current_language_id
    global current_language_name

    for lang_id in languages:
        if lang_id == language_id:
            current_language_id = language_id
            current_language_name = get_pretty_name(language_id)

            if language_id == settings.original_language_id:
                return 0  # Default language, no translation

            # Parse process
            f_path = languages[lang_id]["file"]
            
            localized_strings = parse_json(f_path)

            return 0


def set_pretty_language(language_name):
    """
    Set the current language with a given pretty name
    Args:
        language_name: (str) The pretty name of the language to set.

    Returns:
        success code
        0	Everything went ok
        -1   Can't find any language with the given pretty name
        >0  Call to setLanguage(languageId) failed
    """
    lang_id = get_language_id(language_name)
    if lang_id == 1:
        return -1

    return set_language(lang_id)


def generate_translations(strings):
    """
    Converts a list of strings to a list of empty translations (dicts)
    Args:
        {string[]} strings - The base strings to convert.

    Returns:
        {Translation[]} The empty translations
    """
    strings = remove_duplicates(strings)

    translations = []

    for i in range(0, len(strings)):
        translation = {}
        translation["source"] = strings[i]
        translation["translations"] = ""
        translation["comment"] = ""
        translation["context"] = ""
        translation["context_id"] = 0

        translations.append(translation)

    return translations


def generate_translation_file(file, translations=localized_strings, app_name="dutranslator",
                              version="0.0",
                              language_id=current_language_id,
                              language_name=current_language_name):
    """
    Creates a file for translation with the given base strings.
    Args:
        file: (file or str) The file or URI
        translations: (translation[] or str) The translations or source strings to be included in the translation file.
                [translations=localizedStrings]
        app_name: (str) A name for the app using this translation file. [appName="dutranslator"]
        version: (str) A version (as a string) for this translation file or app. [version="0.0"]
        language_id: (str) A version (as a string) for this translation file or app. [languageId=current]
        language_name: (str) A version (as a string) for this translation file or app. [languageName=currentName]
    """

    app_name = app_name.lower()
    if len(translations) == 0:
        return None

    # if translations is an array of strings, convert to translations
    if isinstance(translations[0], str):
        translations = generate_translations(translations)

    data = {}
    metadata = {}
    translations_object = {}

    metadata["language_id"] = language_id
    metadata["language_name"] = language_name
    metadata["version"] = version

    translations_object["translations"] = translations

    data[app_name] = [metadata, translations_object]

    save_json(data, file)


def remove_duplicates(l):
    newList = []
    for i in l:
        if not i in newList:
            newList.append(i)
    return newList


def parse_json(file):
    """Open a file, loads the contents and with json, extract infos"""

    if file != None:
        with open(file, "r") as read_file:
            read_string = read_file.read()
            # avoid JSONDecodeError: Unexpected UTF-8 BOM (decode using utf-8-sig) :
            read_string = read_string.encode().decode('utf-8-sig')

            file_dict = json.loads(read_string)

            if not settings.name in file_dict:
                return []

            if len( file_dict[settings.name]) != 2:
                return []

            if not 'translations' in file_dict[settings.name][1]:
                return []

            return file_dict[settings.name][1]["translations"]


def save_json(data, file):
    """From a dict, with json, save in file"""

    with open(file, "w") as written_file:
        json.dump(data, written_file, indent=4, ensure_ascii=False)


def tr(string, context=-1, args=()):
    """
    Translate a given string based on the current setted language.
    see {@link DuAEF.Dutranslator} for more details about the translation framework.
    Args:
        string: (str) The text to be translated.
        context: (int or str) Can be an integer or a string which is related to contextId or context in a translation file.
        args: (str[]) Args to format into the translated string, default is [].
        For example, when calling tr("Welcome {#}", -1, "Paul"), the output will be "Welcome Paul".
        If too many args are given, there are ignored.
        If not enough args are given, the {#} are replaced with ?

    Returns:
        (str) The translated text or s if nothing is set or available.
    """

    global language_number
    global localized_strings

    # Default args
    use_context_id = True

    if not isinstance(string, str):
        raise TypeError(string, "str", "String", "DuTranslator::tr()")

    if isinstance(context, str):
        use_context_id = False
    if isinstance(context, int):
        if context is None:
            context = -1
        elif context < 0:
            context = -1
        if args is None:
            args = []
        if isinstance(args, str):
            args = [args]


    res = string

    # If a language is set, search for the translation
    if current_language_id != settings.original_language_id:

        # Get the translation
        for i in range(len(localized_strings)):
            ls = localized_strings[i]
            test_string = ls["source"]

            if test_string == string:
                # Check context
                if use_context_id and (context > -1):
                    if ls["contextId"] == context:
                        string_number = i
                elif not use_context_id:
                    if ls["context"] == context:
                        string_number = i
                else: 
                    string_number = i
            else:
                string_number = -1

            # If a translation is found, set it to res
            if string_number > -1:
                res = localized_strings[string_number]["translation"]
            if res is None:
                res = string
            if res == "":
                res = string

    # Args process
    while res.find("{#}") != -1:

        # While there is stuff to format
        if len(args) < 1:
            # If no more args, replace with ?
            res = res.replace("{#}", "?")  # Will replace the first occurence

        else:
            arg = args.pop(0)  # Take the first arg and remove it
            res = res.replace("{#}", arg)

    return res