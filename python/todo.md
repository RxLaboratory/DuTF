# Development in Progress

Feedback & todo en français ;)

## Corrections / à modifier / finir

- [X] virer la déclaration du dict `translation` au début

- [X] generate_translations() :
    Il faut faire une liste de dict et non pas une liste de string

- [X] save_json() :
    Je pense pas que le dict au début de la méthode soit nécessaire

- [ ] remplacer translator_settings par de simples variables, pas beosin de tout mettre dans un dict
    idem pour settings je pense, on verra

- [ ] finir get_available()

## Tests à faire

Créer un test.py commençant par `from DuTranslator import *` et tester individuellement les fonctions en donnant des arguments de test (== unit tests)

Dans l'ordre du "plus bas niveau" au "plus haut niveau"

- [ ] save_json
    créer un dict bidon mais contenant des caractères spéciaux pour donner comme argument `data`, et file est un chemin du fichier qui sera créé
    le dict peut etre par exemple { "test": "truc", "héhéhé": "des accents", "en chinois": "不是", "une liste": [ 1, 2, 3]}

- [ ] parse_json
    tester en donnant le chemin d'un des fichiers de traduction duik

- [ ] generate_translations
    donner une liste de strings bidon, et vérifier qu'on récupère bien une liste de dict avec les valeurs vides par défaut sauf la "source"

- [ ] generate_translation_file()
    tester en donnant comme `translations` une liste de strings bidon
    et `language_id` "fr" par exemple
    et `language_name` "français" par exemple
    vérifier le fichier écrit, qui doit etre semblable à un fichier de traduction de duik (bien que "vide" à part les "sources")

- [ ] get_available()
    créer un dossier avec plusieurs fichiers de traduction (utiliser ceux de duik)
    mettre ce dossier dans translator_settings["settings]["folder"]
    et tester getAvailable() qui renvoie un numéro
    et vérifier après exécution que translator_settings["languages"] contient bien la liste des langues dispo

- [ ] get_pretty_name()
    à partir de là, les fonctions utilisent la liste créée par get_available qu'il faut donc lancer une fois d'abord

- [ ] get_language_id()

- [ ] get_pretty_names()

- [ ] set_language(language_id)
    vérifier après exécution le contenu de
    translator_settings["current"]
    et
    translator_settings["current_name"]
    et
    translator_settings["localized_strings"]

- [ ] set_pretty_language(language_id)
    vérifier après exécution le contenu de
    translator_settings["current"]
    et
    translator_settings["current_name"]
    et
    translator_settings["localized_strings"]

- [ ] et enfin tester tr() qui fonctionnera après avoir fait
    getAvailable()
    set_language("fr")
    (faut donc avoir un fichier "fr" dans le dossier qui contient les fichiers de traduction)
    tester avec une chaine qui existe dans le fichier de traduction et voir si ça renvoie bien la traduction
    tester aussi avec une chaine de caractères bidons, qui ne sera donc pas traduite
    pour le contexte et les args, on verra plus tard ;)
