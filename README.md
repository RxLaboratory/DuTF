# Dutranslator
Translation framework for Adobe ExtendScript/JavaScript UI

/src/ Contains the translation tool source

/resources/ Contains needed files to run the tools

/examples/ Contains example and test translations

## Contribute to the translations of Dustranlator

All translation files are available under `resources/languagues`.

#### Available translations

| Name                    | File             |
|-------------------------|------------------|
| French                  | `app_fr.ts`      |
| Spanish                 | `app_es.ts`      |
| Chinese                 | `app_zh_cn.ts`   |

##### Updating/Creating translations file

1. Add your language file at the end of the `Dutranslator.pro` file.
2. Update the file from QtCreator. Open the project and do `Tools > Extra > Linguist > lupdate`.
2. Or from the terminal do `lupdate Dutranslator.pro`.

Note: We will take care of adding your language in the interface if it's a new one.

##### Translate

- Open the ts file of your language with QtLinguis.
- If the file is recent, go in `Edit > Translation File Settings` and change the values according to your language.
- Translate.
- Save.
- Commit now or do a release if  you can.

##### Release

Once you have updated your translation file and translated it, you can release it. Like so, we could use the last version in the application.

1. Open the project with QtCreator and do `Tools > Extra > Linguist > lrelease`.
1. Or from the terminal, run `lrelease Dutranslator.pro`.
2. Commit and send a pull request.

## Compile the source code

### From QT Creator

Just open the `Dutranslator.pro` file with Qt Creator and follow the steps.

### From the terminal :v:

1. `cd` into the project directory;
2. run `qmake Dutranslator.pro`, this will generate a Makefille;

	You can run this command only when the pro file has been modified.

3. run `make`, this will compile the code;
4. run `./Dutranslator` to launch the project;
5. use `make clean` to remove all build-related files.

