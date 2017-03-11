#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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
#include <QProgressBar>
#include "aboutdialog.h"
#include "languagewidget.h"
#include "jsxparser.h"

class MainWindow : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
private slots:
    // =======WINDOW BUTTONS
#ifndef Q_OS_MAC
    void maximizeButton_clicked();
#endif
    // =======ACTIONS
    void on_actionOpen_triggered();
    void on_actionSave_triggered();
    void on_actionSave_as_triggered();
    void on_actionAbout_triggered();
    // =======OTHER
    void newTranslation(QStringList translation);
    void newLanguage(QStringList language);
    void jsxParsed();

private:
    //METHODS
    void updateCSS();
    QString unEscape(QString s);
    QString escape(QString s);
    bool checkLanguage();
    void openJsxinc(QString fileName);
    void parseJsxinc(QTextStream *jsxinc);
    //OBJECTS
    QFile workingFile;
    //drag main window
    QPoint dragPosition;
    bool toolBarClicked;
    //buttons
#ifndef Q_OS_MAC
    QPushButton *maximizeButton;
#endif
    //language
    LanguageWidget *languageWidget;
    //status
    QLabel *statusLabel;
    QProgressBar *progressBar;
    //parser
    JsxParser *jsxParser;
    QThread parserThread;

protected:
    //events
    bool eventFilter(QObject *obj, QEvent *event);
    //drag and drop events
    void dropEvent(QDropEvent *event);
    void dragEnterEvent(QDragEnterEvent *event);
    void dragMoveEvent(QDragMoveEvent *event);
    void dragLeaveEvent(QDragLeaveEvent *event);
};

#endif // MAINWINDOW_H
