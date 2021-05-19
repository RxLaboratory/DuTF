from DuTF.python.DuTranslator import *

# ************* TEST : save_json() *************

data = {"test": "truc", "une liste": [1, 2, 3]}
file = "file_test"
# save_json(data, file)


# ************* TEST : parse_json() *************

fichier = "E:/temp/test_fr2.json"

# print(parse_json(fichier))


# ************* TEST : generate_translations *************

liste_de_strings_bidons = ["bidon1", "bidon2", "bidon3", "bidon4", "bidon1", "bidon3"]

# print(generate_translations(liste_de_strings_bidons))


# ************* TEST : generate_translation_file *************

# generate_translation_file("file_test_2", liste_de_strings_bidons, "Nom_app", "2.0", "fr", "francais")


# ************* TEST : get_available *************

settings_folder = "E:/temp"

get_available()

print(languages)
