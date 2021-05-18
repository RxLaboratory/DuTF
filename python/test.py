from DuTF.python.DuTranslator import *


# ************* TEST : save_json() *************

data = { "test": "truc", "une liste": [ 1, 2, 3]}
file = "file_test"
# save_json(data, file)


# ************* TEST : parse_json() *************

fichier = "E:/temp/test_fr2.json"

print(parse_json(fichier))


# ************* TEST : generate_translations *************

liste_de_strings_bidons = ["bidon1", "bidon2", "bidon3", "bidon4"]

#print(generate_translations(liste_de_strings_bidons))
