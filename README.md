# Dutranslator
Translation framework for Adobe ExtendScript/JavaScript UI

/src/ Contains the translation tool source

/resources/ Contains needed files to run the tools

/examples/ Contains example and test translations

## Contribute to the translations

All translation files are available under `resources/languagues`.
If your language is not present, just copy another `.ts` file and rename it.

#### Current state

| Name                    | File             | State        |
|-------------------------|------------------|--------------|
| French                  | `app_fr.ts`      | 100% ✅      |
| Spanish                | `app_es.ts`      | 0%           |

#### How to

1. Open your langauge `.ts` file with Qt Linguist.
2. Translate!
3. Save it and commit it.

We will then take care of releasing it. You can update it and release it with Qt Creator if you know how to do it.

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

