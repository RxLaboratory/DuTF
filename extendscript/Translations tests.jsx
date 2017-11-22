//gets the new translation framework
#include Dutranslator.jsxinc

var output = "";


Dutranslator.settings.folder =  File($.fileName).path + "/../examples/";
Dutranslator.settings.prefix =  "Duik_translations_";
Dutranslator.settings.name = "duik";

var start = new Date().getTime();
Dutranslator.getAvailable();
var end = new Date().getTime();
output += "GetAvailable took " + (end- start)+ " ms\n";





var names = Dutranslator.getPrettyNames();
output += "Available languages: \n" + names + "\n";


for(var i = 0; i < names.length; i++)
{
    output += "Setting lang to " + names[i] + "\n";
    start = new Date().getTime();
    var status = Dutranslator.setPrettyLanguage(names[i]);
    end = new Date().getTime();
    output += "setPrettyLanguage took " + (end - start) + " ms\n";
    if(status != 0) output += "Setting the lang to " + names[i] + " went wrong, error code: " + status + "\n";
    else output += "Translation in " + names[i] + " of Launch Duik is \n   " + tr("Launch Duik") + "\n";       
    output += "--------\n";
}

output += "\n\nTest ends.";
alert(output);
