#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define INC_TIMER 2  // Time span between each widget creation
#define MAX_AUTO_ROW 500  // Maximum number of empty rows created in background

#include <QFile>
#include <QLabel>
#include <QProgressBar>
#include <QTimer>

#include <vector>

#include "ui_mainwindow.h"

#include "aboutdialog.h"
#include "languagewidget.h"
#include "jsonParser.h"
#include "searchwidget.h"
#include "stringparser.h"
#include "rowbuttonswidget.h"
#include "preferenceswidget.h"
#include "scriptparsewidget.h"
#include "translation.h"


/**
 * @brief The aplication's main window class
 */
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT


public:

    /**
     * @brief Default constructor
     * @param parent	Windows's parent widget
     */
    explicit MainWindow(QWidget *parent = 0);

    /**
     * @brief Destructor
     */
    ~MainWindow();

    /**
     * @brief Returns an instance of the main window currently being used
     * @return
     */
    static MainWindow & instance();

private slots:

    /**
     * @brief Opens an about dialog
     */
    void actionAbout();

    /**
     * @brief Adds a row below the current row
     */
    void actionAddRow();

    /**
     * @brief Import strings from a file
     */
    void actionImport();

    /**
     * @brief Creates a dialog for openning a new file
     */
    void actionOpen();

    /**
     * @brief Shows the preferences panel
     */
    void actionPreferences(bool checked);

    /**
     * @brief Asks for the user confirmation to remove a row
     *
     */
    void actionRemoveRow();

    /**
     * @brief Saves the current file
     */
    void actionSave();

    /**
     * @brief Saves the current file as something else
     */
    void actionSaveAs();

    /**
     * @brief Shows the tools panel
     */
    void actionTools(bool checked);

    /**
     * @brief Will add tr calls in the user source code
     */
    void actionUpdateSourceCode();

    /**
     * @brief Adds an empty row in the table
     * Stops when the MAX_AUTO_ROW is reached
     *
     * @param index   The row will be added bellow the row with the given index
     * If index is not in the correct range, the row is added at the end
     */
    void addTableRow(int index = -1);

    /**
     * @brief Adds content to the last row available
     * If no row is available, a new one is created
     *
     * @param Translation       The translation to add
     */
    void addTableRowContent(Translation);

    /**
     * @brief Asks for a location then saves a translator according to the selection in btn_selectGenerateTranslator (QComboBox)
     */
    void btn_generateTranslator_clicked();

    /**
     * @brief Clears the current search
     */
    void clearSearch();

    /**
     * @brief Clears the table content
     * The clear goes from tableFreeIndex to the last row
     * This allows to modify the rows and to clear only the rows not in use
     */
    void clearTableToTheEnd();

    /**
     * @brief Returns a list of translations currently in the table
     * @return
     */
    std::vector<Translation> getTranslations() const;

#ifndef Q_OS_MAC
    /**
     * @brief Maximize the window
     * Has effect only on Windows and Linux
     */
    void maximize();
#endif

    /**
     * @brief Slot to run when a new application is being translated
     * @param app
     */
    void newApplication(QString app);

    /**
     * @brief Starts a new language translation
     * @param language		Langauge name and code
     */
    void newLanguage(QStringList language);

    /**
     * @brief Adds a new translation to the current translation file
     * @param translation	Translation to add
     */
    void newTranslation(Translation pTr);

    /**
     * @brief Open the given translation file.
     * Has to be a json file.
     */
    void openTranslationFile(QString);

    /**
     * @brief Alerts user that the parsing has failed
     * @param   The error flag(s)
     */
    void parsingFailed(Parser::ParsingErrors);

    /**
     * @brief Marks the end of the  parsing
     */
    void parsingFinished();

    /**
     * @brief Removes a row
     *
     * @param index     The index of the row to remove
     */
    void removeTableRow(int index);

    /**
     * @brief Searches for a given string in the translation file
     * @param s		The string to look for
     */
    void search(QString s);

    /**
     * @brief Sets the appearance of the toolbar
     *
     * @param appearance        0-Text under, 1-Text Beside, 2-Icon only, 3-Text Only
     */
    void setToolBarAppearance(int appearance);

    /**
     * @brief Sets the window in waiting mode, showing a progress bar and a label
     * for use during long operations
     * @param max		The progress bar maximum value
     * @param status	The status to be displayed
     * @param wait      Wether to activate waiting mode or deactivate it
     */
    void setWaiting(bool wait = true, QString status = "", int max = 100 );

    /**
     * @brief Shows the main page (hide preferences, about, etc.)
     */
    void showMainPage();

    /**
     * @brief Start the actual export process for adding tr calls in user source code
     */
    void startExportPorcess(StringParser::TranslationParsingModes);

    /**
     * @brief Start the actual import or merge process
     */
    void startImportPorcess(StringParser::TranslationParsingModes);

    /**
     * @brief Updates the windows stylesheet
     *
     * @param cssFileName   The file name of the CSS to load
     */
    void updateCSS(QString cssFileName);

private:

    /**
     * @brief Adjusts the column sizes to use all available space
     */
    void adjustColumnSizes();

    /**
     * @brief Checks if languages fields are completed
     * @return	True if fields are completed
     */
    bool checkLanguage();

    /**
     * @brief Makes the last adjustments after initializing the application
     */
    void endInit();

    /**
     * @brief Connects required signals and slots
     * Executed on construction only
     */
    void mapEvents();

    /**
     * @brief Drag position
     * Used for drag n drop feature
     */
    QPoint dragPosition;

    /**
     * @brief Timer used to create the table widgets
     */
    QTimer fillTableTimer;

    /**
     * @brief The import merge preferences panel
     */
    ScriptParseWidget *scriptParsePreferences;

    /**
     * @brief Instance of the application
     */
    static MainWindow* instance_;

    /**
     * @brief The Parser
     */
    JsonParser jsonParser;

    /**
     * @brief The language widget
     * Contains the language code and name
     */
    LanguageWidget *languageWidget;

#ifndef Q_OS_MAC
    /**
     * @brief Maximizes window button
     * Only on linux and windows
     */
    QPushButton *maximizeButton;

    /**
     * @brief Minimizes window button
     * Only on linux and windows
     */
    QPushButton *minimizeButton;
#endif

    /**
     * @brief The Preferences panel
     */
    PreferencesWidget *preferences;

    /**
     * @brief A progressbar
     * Used to give feedback on background (only) processes
     * long processes, like opening file, which need to disbale the UI should instead use setWaiting();
     */
    QProgressBar *progressBar;

    /**
     * @brief Quits application button
     */
    QPushButton *quitButton;

    /**
     * @brief The search bar
     */
    SearchWidget *searchWidget;

    /**
     * @brief A status label
     * Used to give feedback to the user
     */
    QLabel *statusLabel;

    /**
     * @brief stringParser
     */
    StringParser stringParser;

    /**
     * @brief The index of the first availabe row in the table
     */
    int tableFreeIndex;

    /**
     * @brief Is the tool bar currently clicked or not
     */
    bool toolBarClicked;

    /**
     * @brief The list of current translations
     */
    std::vector<Translation> translations;

    /**
     * @brief The current translation working file
     */
    QFile workingFile;

protected:

    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void dropEvent(QDropEvent *event);
    bool eventFilter(QObject *obj, QEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
