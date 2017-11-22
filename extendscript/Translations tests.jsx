//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.settings.folder =  File($.fileName).path + "/../examples/";
Dutranslator.settings.prefix =  "Duik_translations_";
Dutranslator.settings.name = "duik";

Dutranslator.getAvailable();





var output = "";
output = "Available languages: " + Dutranslator.languages + "\n\n";

for(var i = 0; i < Dutranslator.languages.length; i++){
    
    var langId = Dutranslator.languages[i];
    output += "Setting lang to " + langId + "\n";
    var status = Dutranslator.setLanguage(langId);
    if(status != 0) output += "Setting the lang to " + langId + " went wrong, error code: " + status + "\n";
    else output += "Translation in " + langId + " of Launch Duik is \n   " + tr("Launch Duik") + "\n";       
    output += "--------\n";
}

output += "\n\nTest ends.";
alert(output);