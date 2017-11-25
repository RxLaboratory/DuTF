#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define INC_TIMER 2  // Time span between each widget creation
#define MAX_AUTO_ROW 500  // Maximum number of empty rows created in background

#include "ui_mainwindow.h"
#include <QStyleFactory>
#include <QFile>
#include <QLabel>
#include <QFileDialog>
#include <QJsonValue>
#include <QMouseEvent>
#include <QPushButton>
#include <QTextEdit>
#include <QSpinBox>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QMimeData>
#include <QThread>
#include <QTimer>
#include <QProgressBar>
#include <QDesktopWidget>
#include "aboutdialog.h"
#include "languagewidget.h"
#include "jsonParser.h"
#include "stringparser.h"
#include "searchwidget.h"
#include "rowbuttonswidget.h"
#include "preferenceswidget.h"
#include "translation.h"

namespace Ui {
    class MainWindow;
}

/**
 * @brief The aplication's main window class
 */
class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

    static MainWindow* instance_;

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

#ifndef Q_OS_MAC
    /**
     * @brief Maximize the window
     * Has effect only on Windows and Linux
     */
    void maximize();
#endif



    // =======ACTIONS


    /**
     * @brief Creates a dialog for openning a new file
     */
    void actionOpen();

    /**
     * @brief Import strings from a file
     */
    void actionImport();

    /**
     * @brief Saves the current file
     */
    void actionSave();

    /**
     * @brief Saves the current file as something else
     */
    void actionSaveAs();

    /**
     * @brief Opens an about dialog
     */
    void actionAbout();

    /**
     * @brief Shows the preferences panel
     */
    void actionPreferences(bool checked);

    /**
     * @brief Shows the tools panel
     */
    void actionTools(bool checked);


    // =======OTHER USER INTERACTIONS


    /**
     * @brief Searches for a given string in the translation file
     * @param s		The string to look for
     */
    void search(QString s);

    /**
     * @brief Clears the current search
     */
    void clearSearch();

    /**
     * @brief Asks for a location then saves a translator according to the selection in btn_selectGenerateTranslator (QComboBox)
     */
    void btn_generateTranslator_clicked();


    // =======OTHER


    /**
     * @brief Adds a new translation to the current translation file
     * @param translation	Translation to add
     */
    void newTranslation(Translation pTr);

    /**
     * @brief Starts a new language translation
     * @param language		Langauge name and code
     */
    void newLanguage(QStringList language);

    /**
     * @brief Slot to run when a new application is being translated
     * @param app
     */
    void newApplication(QString app);

    /**
     * @brief Marks the end of the  parsing
     */
    void parsingFinished();

    /**
     * @brief Alerts user that the parsing has failed
     * @param   The error flag(s)
     */
    void parsingFailed(Parser::ParsingErrors);

    /**
     * @brief Adds an empty row in the table
     * Stops when the MAX_AUTO_ROW is reached
     *
     * @param index   The row will be added bellow the row with the given index
     * If index is not in the correct range, the row is added at the end
     */
    void addTableRow(int index = -1);

    /**
     * @brief Removes a row
     *
     * @param index     The index of the row to remove
     */
    void removeTableRow(int index);

    /**
     * @brief Adds content to the last row available
     * If no row is available, a new one is created
     *
     * @param Translation       The translation to add
     */
    void addTableRowContent(Translation);

    /**
     * @brief Clears the table content
     * The clear goes from tableFreeIndex to the last row
     * This allows to modify the rows and to clear only the rows not in use
     */
    void clearTableToTheEnd();

    /**
     * @brief Sets the window in waiting mode, showing a progress bar and a label
     * for use during long operations
     * @param max		The progress bar maximum value
     * @param status	The status to be displayed
     * @param wait      Wether to activate waiting mode or deactivate it
     */
    void setWaiting(bool wait = true, QString status = "", int max = 100 );

    /**
     * @brief Asks for the user confirmation to remove a row
     *
     */
    void actionRemoveRow();

    /**
     * @brief Adds a row below the current row
     */
    void actionAddRow();

    /**
     * @brief Shows the main page (hide preferences, about, etc.)
     */
    void showMainPage();

    /**
     * @brief Sets the appearance of the toolbar
     *
     * @param appearance        0-Text under, 1-Text Beside, 2-Icon only, 3-Text Only
     */
    void setToolBarAppearance(int appearance);

    /**
     * @brief Updates the windows stylesheet
     *
     * @param cssFileName   The file name of the CSS to load
     */
    void updateCSS(QString cssFileName);

private:


    // METHODS

    /**
     * @brief Connects required signals and slots
     * Executed on construction only
     */
    void mapEvents();

    /**
     * @brief Checks if languages fields are completed
     * @return	True if fields are completed
     */
    bool checkLanguage();

    /**
     * @brief Opens a js translation file
     * @param fileName	Js file name
     */
    void openJsxinc(QString fileName);

    /**
     * @brief Parses a js translation text
     * @param jsxinc	The text to parse
     */
    void parseJsxinc(QTextStream *jsxinc);

    /**
     * @brief Makes the last adjustments after initializing the application
     */
    void endInit();

    /**
     * @brief Adjusts the column sizes to use all available space
     */
    void adjustColumnSizes();


    // OBJECTS


    /**
     * @brief The current translation working file
     */
    QFile workingFile;

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
     * @brief Is the tool bar currently clicked or not
     */
    bool toolBarClicked;

    /**
     * @brief The index of the first availabe row in the table
     */
    int tableFreeIndex;


    // BUTTONS

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
     * @brief Quits application button
     */
    QPushButton *quitButton;

    /**
     * @brief The language widget
     * Contains the language code and name
     */
    LanguageWidget *languageWidget;

    /**
     * @brief A status label
     * Used to give feedback to the user
     */
    QLabel *statusLabel;

    /**
     * @brief A progressbar
     * Used to give feedback on background (only) processes
     * long processes, like opening file, which need to disbale the UI should instead use setWaiting();
     */
    QProgressBar *progressBar;

    /**
     * @brief The search bar
     */
    SearchWidget *searchWidget;

    /**
     * @brief The Parser
     */
    JsonParser jsonParser;

    /**
     * @brief stringParser
     */
    StringParser stringParser;

    /**
     * @brief The Preferences panel
     */
    PreferencesWidget *preferences;

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
    void resizeEvent(QResizeEvent *event);
};

#endif // MAINWINDOW_H
