from DuTF.python.DuTranslator import save_json


# TEST : save_json()
data = { "test": "truc", "une liste": [ 1, 2, 3]}

file = "DuTF.python.DuTranslator"

save_json(data, file)
