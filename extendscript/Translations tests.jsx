//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.getAvailable("../examples", "Duik_translations_");

function check(value, required, message){    
    if(value !== required)
    {
        alert(value + " is not equal to " + required + "\n\n" + message);
     }
     else
     {
        alert(value + " is OK!");
     }
}

var value = [];
for (var i = 0 ; i < Dutranslator.languages.length ; i++)
{
    value.push(Dutranslator.languages[i].languageId.toString());
}

//check(value, ['en','de','es','fr','pl','zh_HANS'], "The languages list is not correct");
alert(value);

alert("Tests end.");
