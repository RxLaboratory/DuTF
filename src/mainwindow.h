#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#define INC_TIMER 50  // Time span between each widget creation
#define MAX_AUTO_ROW 1000  // Number of maximum created in parallel

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
#include "aboutdialog.h"
#include "languagewidget.h"
#include "jsxparser.h"
#include "searchwidget.h"

namespace Ui {
    class MainWindow;
}

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
     * @brief Create a dialog for openning a new file
     */
    void actionOpen();

    /**
     * @brief Save the current file
     */
    void actionSave();

    /**
     * @brief Save the current file as something else
     */
    void actionSaveAs();

    /**
     * @brief Open an about dialog
     */
    void actionAbout();


    // =======OTHER USER INTERACTIONS


    /**
     * @brief Search for a given string in the translation file
     * @param s		The string to look for
     */
    void search(QString s);

    /**
     * @brief Clear the current search
     */
    void clearSearch();


    // =======OTHER


    /**
     * @brief Add a new translation to the current translation file
     * @param translation	Translation to add
     */
    void newTranslation(QStringList translation);

    /**
     * @brief Start a new language translation
     * @param language		Langauge name and code
     */
    void newLanguage(QStringList language);

    /**
     * @brief Mark the end of the  parsing
     */
    void parsingFinished();

    /**
     * @brief Alert user that the parsing has failed
     */
    void parsingFailed();

    /**
     * @brief Add an empty row in the table
     * Stops when the MAX_AUTO_ROW is reached
     */
    void addTableRow();

    /**
     * @brief Add content to the last row available
     * If any row is availbe, one is created
     *
     * @param content   A list of 4 strings
     * original: [0]
     * context: [1]
     * translated: [2]
     * comment: [3]
     */
    void addTableRowContent(QStringList content);

    /**
     * @brief Clear the table content
     * The clear goes from tableFreeIndex to the last row
     * This allows to modify the rows and only the clear the rows not in use
     */
    void clearTableToTheEnd();

private:


    // METHODS


    /**
     * @brief Update the windows stylesheet
     */
    void updateCSS();

    /**
     * @brief Connect required signals and slots
     * Executed on construction only
     */
    void mapEvents();

    /**
     * @brief Check if languages fields are completed
     * @return	True if fields are completed
     */
    bool checkLanguage();

    /**
     * @brief Open a js translation file
     * @param fileName	Js file name
     */
    void openJsxinc(QString fileName);

    /**
     * @brief Parse a js translation text
     * @param jsxinc	The text to parse
     */
    void parseJsxinc(QTextStream *jsxinc);


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
     * @brief Maximize window button
     * Only on linux and windows
     */
    QPushButton *maximizeButton;

    /**
     * @brief Minimize window button
     * Only on linux and windows
     */
    QPushButton *minimizeButton;
#endif

    /**
     * @brief Quit application button
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
     * Used to give feedback on processes
     */
    QProgressBar *progressBar;

    /**
     * @brief The search bar
     */
    SearchWidget *searchWidget;

    /**
     * @brief The Parser
     */
    JsxParser *jsxParser;

protected:

    // Reimplemented methods
    bool eventFilter(QObject *obj, QEvent *event);
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
};

#endif // MAINWINDOW_H
