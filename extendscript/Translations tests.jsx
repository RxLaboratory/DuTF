//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.getAvailable("../examples", "Duik_translations_");
alert(Dutranslator.languages);


function check(value, required, message){
    if(value !== required)
    {
        alert(value + " is not equal to " + required + "\n\n" + message);
        throw new Error("Value not equals");
     }
    
}

