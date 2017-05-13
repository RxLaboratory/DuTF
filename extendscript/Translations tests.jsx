//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.settings.folder =  File($.fileName).path + "/../examples/";
Dutranslator.settings.prefix =  "Duik_translations_";

Dutranslator.getAvailable();





var output = "";
output = "Available languages: " + Dutranslator.languages + "\n\n";

for(var i = 0; i < Dutranslator.languages.length; i++){
    
    var langId = Dutranslator.languages[i];
    Dutranslator.setLanguage(langId);
    
    output += "Translation in " + langId + " of Open General Preferences is " + tr("Open General Preferences") + "\n";       
}

output += "\n\nTest ends.";
alert(output);
