//gets the new translation framework
#include Dutranslator.jsxinc

Dutranslator.settings.folder =  File($.fileName).path + "/../examples/";
Dutranslator.settings.prefix =  "Duik_translations_";
Dutranslator.settings.name = "duik";

Dutranslator.getAvailable();





var output = "";
output = "Available languages: \n";
for(var langId in Dutranslator.languages){   
    output += langId + " " + Dutranslator.languages[langId]["name"] + " " + Dutranslator.languages[langId]["file"]  + "\n";
}
output += "\n\n";

for(var langId in Dutranslator.languages){   
    output += "Setting lang to " + langId + "\n";
    var status = Dutranslator.setLanguage(langId);
    if(status != 0) output += "Setting the lang to " + langId + " went wrong, error code: " + status + "\n";
    else output += "Translation in " + langId + " of Launch Duik is \n   " + tr("Launch Duik") + "\n";       
    output += "--------\n";
}

output += "\n\nTest ends.";
alert(output);