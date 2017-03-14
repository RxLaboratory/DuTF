# Dutranslator
Translation framework for Adobe ExtendScript/JavaScript UI

/src/ Contains the translation tool source

/resources/ Contains needed files to run the tools

/examples/ Contains example and test translations

## Compile the source code

### From QT Creator

Just open the `Dutranslator.pro` file from Qt Creator and follow the steps.

### From the terminal :v:

1. `cd` into the project directory;
2. run `qmake Dutranslator.pro`, this will generate a Makefille;

	You can run this command only when the pro file has been modified.

3. run `make`, this will compile the code;
4. run `./Dutranslator` to launch the project;
5. use `make clean` to remove all build-related files.
