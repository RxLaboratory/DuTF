from dutranslator import translator, settings

# ************* TEST : save_json() *************

data = {"test": "truc", "une liste": [1, 2, 3]}
file = "file_test"
# translator.save_json(data, file)


# ************* TEST : parse_json() *************

fichier = "E:/temp/test_fr2.json"

# print(translator.parse_json(fichier))


# ************* TEST : generate_translations *************

liste_de_strings_bidons = ["bidon1", "bidon2", "bidon3", "bidon4", "bidon1", "bidon3"]

# print(translator.generate_translations(liste_de_strings_bidons))


# ************* TEST : generate_translation_file *************

# translator.generate_translation_file("file_test_2", liste_de_strings_bidons, "Nom_app", "2.0", "fr", "francais")


# ************* TEST : get_available *************

settings.folder = "/home/rainbox/Documents/RAINBOX/DEV-SRC/RxOT/DuTF/examples"
settings.prefix = "Duik_translations_"
settings.name = 'duik'

translator.get_available()

# print(translator.languages)

# ************* TEST : get_pretty_name *************

# print(translator.get_pretty_name("de"))
# >>> Deutsch  / fr >>> Français  /  en >>> English

# ************* TEST : get_language_id *************

# print(translator.get_language_id("Deutsch"))
# >>> de  /  Français >>> fr  /  English >>> en

# ************* TEST : get_pretty_names *************

# print(translator.get_pretty_names())
# >>> ['Deutsch', 'English', 'Español', 'Français', 'Polski', '简体中文']

# ************* TEST : set_language *************

translator.set_language("fre")

# print(translator.current_language_id)
# print(translator.current_language_name)
# print(translator.localized_strings)

# ************* TEST : set_pretty_language *************

# translator.set_pretty_language("Deutsch")

# print(translator.current_language_id)
# print(translator.current_language_name)
# print(translator.localized_strings)

# ************* TEST : tr *************


# test avec fre : OK
print ( translator.tr("Hello") )
print ( translator.tr("Welcome {#}", -1, ("Paul")) )
print ( translator.tr("The Thing") )
print ( translator.tr("The Thing",1) )
print ( translator.tr("The Thing",2) )
print ( translator.tr("The Thing","Home Page") )
print ( translator.tr("The Thing","Settings UI") )

print("***")
# test avec fr : OK
print ( translator.tr("INSTALLATION_en") )
print ( translator.tr("Next") )
print ( translator.tr("ERROR") )
print ( translator.tr("error") )
print ( translator.tr("Duik - Loading icons") )
print ( translator.tr("Please, check the box called 'Allow Scripts to write files...' in the general preferences of After Effects."))


