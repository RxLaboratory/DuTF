//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.getAvailable("../examples", "Duik_translations_");

function check(value, required, message){
    if(value !== required)
    {
        alert(value + " is not equal to " + required + "\n\n" + message);
     }    
}

check(Dutranslator.languages, [
    ['en', 'English'],
    ['de', 'Deutsch'],
    ['es', 'Español'],
    ['fr', 'Français'],
    ['pl', 'Polski'],
    ['zh_HANS', '简体中文']
], "The languages list is not correct");

alert("Tests end.");
