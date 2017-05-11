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




alert(Dutranslator.settings.folder);

alert("Tests end.");
